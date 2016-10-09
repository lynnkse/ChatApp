#include "../inc/groupDB.h"
#include "../inc/userDB.h"
#include "../inc/HashMap.h"
#include "../inc/logmngr.h"
#include "../inc/list.h"
#include "../inc/list_itr.h"
#include "../inc/list_functions.h"
#include <string.h>
#include <stdlib.h>


#define SERIALIZATION_FILE "serialization_groups"
#define LAST_BYTE 16
#define BASE_MULTICAST_IP "224.0.0.15"

struct GroupDB_t
{
	HashMap* m_groups;	
};

struct Group_t
{
	char m_groupname[MAX_GROUPNAME_LENGTH];
	char* m_admin;
	char m_IP[IP_ADDR_LEN];
	List* m_members;
};

/*---Static functions declarations---*/
static int HashFunc(char *str);
static int IsKeysEqual(char* _key1, char* _key2);
static void DestroyVal(void* _val);
static Group_t* FindGroup(GroupDB_t* _groupDB, UserInterface* _ui);
static Group_t* CreateGroup(UserInterface* _ui);
static void DestroyGroup(Group_t* _group);
static ListItr IsUserMemberOfTheGroup(Group_t* _group, UserInterface* _ui);
static int SerizlizeItem(void* _key, Group_t* _value, FILE* _fp);
static void ReadDBfromFile(HashMap* _map, FILE* _fp);
static int PrintOutName(void* _key, Group_t* _group, void* _context);
static int SerializeString(char* _string, FILE* _fp);

/*---API functions defenitions---*/
GroupDB_t* CreateGroupDB(size_t _groupsCap)
{
	GroupDB_t* groupDB;
	Zlog* traceZlog;
	Zlog* errorZlog;
	FILE* fp = NULL;
	
	traceZlog = ZlogGet("trace");
	errorZlog = ZlogGet("error");
	
	groupDB = (GroupDB_t*) malloc(sizeof(GroupDB_t));
	if(!groupDB)
	{
		ZLOG_SEND(errorZlog, LOG_ERROR, "Failed to create group DB, %d",1);
		return NULL;
	}
	
	groupDB->m_groups = HashMap_Create(_groupsCap, (HashFunction) HashFunc, (EqualityFunction) IsKeysEqual);
	if(!groupDB)
	{
		ZLOG_SEND(errorZlog, LOG_ERROR, "Failed to create group DB, %d",1);
		free(groupDB);
		return NULL;
	}

	fp = fopen(SERIALIZATION_FILE, "r");
	if(fp)
	{
		ReadDBfromFile(groupDB->m_groups, fp);
		fclose(fp);	
	}	
	return groupDB;
}

void DestroyGroupDB(GroupDB_t* _groupDB)
{
	HashMap_Destroy(&_groupDB->m_groups, NULL, DestroyVal);
	free(_groupDB);
}

ChatRes GroupDB_CreateNewGroup(GroupDB_t* _groupDB, UserInterface* _ui)
{
	Group_t* group;
	Zlog* traceZlog;
	Zlog* errorZlog;
	
	traceZlog = ZlogGet("trace");
	errorZlog = ZlogGet("error");

	group = CreateGroup(_ui);
	if(!group)
	{
		ZLOG_SEND(errorZlog, LOG_ERROR, "Failed to create group, %d",1);
		_ui->m_choice = GROUP_CREATE_FAILURE;
		return FAILURE;
	}
	
	if(HashMap_Insert(_groupDB->m_groups, group->m_groupname, group) == MAP_SUCCESS)
	{
		ZLOG_SEND(traceZlog, LOG_TRACE, "New group created, %d",1);
		_ui->m_choice = GROUP_CREATE_SUCCESS;
		return SUCCESS;
	}
	else
	{
		ZLOG_SEND(errorZlog, LOG_ERROR, "Failed creating a group(insert fail), %d",1);
		_ui->m_choice = GROUP_CREATE_FAILURE;
		return FAILURE;
	}
}
 
ChatRes GroupDB_DeleteGroup(GroupDB_t* _groupDB, UserInterface* _ui)
{
	Group_t* group = NULL;
	Zlog* traceZlog;
	Zlog* errorZlog;
	
	traceZlog = ZlogGet("trace");
	errorZlog = ZlogGet("error");
	
	group = FindGroup(_groupDB, _ui);
	if(!group)
	{
		ZLOG_SEND(errorZlog, LOG_ERROR, "Group doesn't exist, %d",1);
		_ui->m_choice = GROUP_DELETE_FAILURE;
		return FAILURE;
	}
	
	if(IsUserMemberOfTheGroup(group, _ui))
	{
		HashMap_Remove(_groupDB->m_groups, _ui->m_groupname, (void**) &group);
		DestroyGroup(group);
		ZLOG_SEND(traceZlog, LOG_TRACE, "Group was deleted, %d",1);
		_ui->m_choice = GROUP_DELETE_SUCCESS;
		return SUCCESS;
	}
	else
	{
		ZLOG_SEND(errorZlog, LOG_ERROR, "User can't delete group, %d",1);
		_ui->m_choice = GROUP_DELETE_FAILURE;
		return FAILURE;
	}
}
 
ChatRes GroupDB_UserJoinGroup(GroupDB_t* _groupDB, UserInterface* _ui)
{
	Group_t* group = NULL;
	char* username;
	Zlog* traceZlog;
	Zlog* errorZlog;
	
	traceZlog = ZlogGet("trace");
	errorZlog = ZlogGet("error");
	
	username = (char*) malloc(MAX_USERNAME_LENGTH);
	
	group = FindGroup(_groupDB, _ui);
	if(!group)
	{
		ZLOG_SEND(errorZlog, LOG_ERROR, "Group doesn't exist, %d",1);
		_ui->m_choice = GROUP_JOIN_FAILURE;
		return FAILURE;
	}
	
	if(!IsUserMemberOfTheGroup(group, _ui))
	{
		strcpy(username, _ui->m_username);
		List_PushHead(group->m_members, username);
		ZLOG_SEND(traceZlog, LOG_TRACE, "User joined group, %d",1);
		_ui->m_choice = GROUP_JOIN_SUCCESS;
		return SUCCESS;
	}
	else
	{
		ZLOG_SEND(errorZlog, LOG_ERROR, "User is already member of the group, %d",1);
		_ui->m_choice = GROUP_JOIN_FAILURE;
		return FAILURE;
	}
}
 
ChatRes GroupDB_UserLeaveGroup(GroupDB_t* _groupDB, UserInterface* _ui)
{
	Group_t* group = NULL;
	char* username;
	Zlog* traceZlog;
	Zlog* errorZlog;
	ListItr user;
	
	traceZlog = ZlogGet("trace");
	errorZlog = ZlogGet("error");
	
	group = FindGroup(_groupDB, _ui);
	if(!group)
	{
		ZLOG_SEND(errorZlog, LOG_ERROR, "Group doesn't exist, %d",1);
		_ui->m_choice = GROUP_LEAVE_FAILURE;
		return FAILURE;
	}
	
	if((user = IsUserMemberOfTheGroup(group, _ui)) != NULL)
	{
		ListItr_Remove(user);
		ZLOG_SEND(traceZlog, LOG_TRACE, "User left group, %d",1);
		_ui->m_choice = GROUP_LEAVE_SUCCESS;
		return SUCCESS;
	}
	else
	{
		ZLOG_SEND(errorZlog, LOG_ERROR, "User is not a member of the group, %d",1);
		_ui->m_choice = GROUP_JOIN_FAILURE;
		return FAILURE;
	}
}

ChatRes GroupDB_StartChat(GroupDB_t* _groupDB, UserInterface* _ui)
{
	Zlog* traceZlog;
	Zlog* errorZlog;
	Group_t* group = NULL;
	ListItr user;
	
	traceZlog = ZlogGet("trace");
	errorZlog = ZlogGet("error");
	
	group = FindGroup(_groupDB, _ui);
	if(!group)
	{
		ZLOG_SEND(errorZlog, LOG_ERROR, "Group doesn't exist, %d",1);
		_ui->m_choice = START_CHAT_FAILURE;
		return FAILURE;
	}
	
	if((user = IsUserMemberOfTheGroup(group, _ui)) != NULL)
	{
		ZLOG_SEND(traceZlog, LOG_TRACE, "User found and can start chat, %d",1);
		_ui->m_choice = START_CHAT_SUCCESS;
		strcpy(_ui->m_groupname, group->m_groupname);
		strcpy(_ui->m_IP, group->m_IP);
		return SUCCESS;
	}
	else
	{
		ZLOG_SEND(errorZlog, LOG_ERROR, "User is not a member of the group, %d",1);
		_ui->m_choice = START_CHAT_FAILURE;
		return FAILURE;
	}
}

ChatRes GroupDB_Save(GroupDB_t* _groupDB, UserInterface* _ui)
{
	FILE* fp = NULL;
	ZLOGS_INITIALIZATION;

	fp = fopen(SERIALIZATION_FILE, "w");
	if(!fp)
	{
		ZLOG_SEND(errorZlog, LOG_ERROR, "Unable to open file for serialization, %d",1);
		_ui->m_choice = SAVE_DB_FAILURE;
		return FAILURE;
	}
		
	HashMap_ForEach(_groupDB->m_groups, (KeyValueActionFunction) SerizlizeItem, NULL);

	fclose(fp);

	_ui->m_choice = SAVE_DB_SUCCESS;

	return SUCCESS;
}

ChatRes GroupDB_PrintOutGroups(GroupDB_t* _groupDB, UserInterface* _ui)
{
	HashMap_ForEach(_groupDB->m_groups, (KeyValueActionFunction) PrintOutName, NULL);
	_ui->m_choice = PRINT_OUT_GROUPS_SUCCESS;	
	return SUCCESS;
}

/*---Static functions definitions---*/
static void ReadDBfromFile(HashMap* _map, FILE* _fp)
{
	Group_t* group; 
	size_t res = 0;	
	int numOfMembers, memberIndex;
	char* username;

	do
	{
		group = (Group_t*) malloc(sizeof(Group_t));		
		username = (char*) malloc(MAX_USERNAME_LENGTH);

		res = fread(group, sizeof(Group_t), 1, _fp);

		group->m_members = List_Create();

		fread(&numOfMembers, sizeof(size_t), 1, _fp);
		for(memberIndex = 0; memberIndex < numOfMembers; ++memberIndex)
		{
			fread(username, MAX_USERNAME_LENGTH, 1, _fp);
			List_PushHead(group->m_members, username);
		}
	
		HashMap_Insert(_map, group->m_groupname, group);
	}
	while(res);
}

static int PrintOutName(void* _key, Group_t* _group, void* _context)
{
	printf("%s\n", _group->m_groupname);
	return 1;
}

static int SerizlizeItem(void* _key, Group_t* _value, FILE* _fp)
{
	ListItr begin;
	ListItr end;
	int size = (int) List_Size(_value->m_members);
	
	fwrite(_value, sizeof(Group_t), 1, _fp);
	fwrite(&size, sizeof(size_t), 1, _fp);

	begin = ListItr_Begin(_value->m_members);
	end = ListItr_End(_value->m_members);
	ListItr_ForEach(begin, end, (ListActionFunction) SerializeString, _fp);
	return 1;
}

static int SerializeString(char* _string, FILE* _fp)
{
	fwrite(_string, MAX_USERNAME_LENGTH, 1, _fp);
	return 1;
}

static ListItr IsUserMemberOfTheGroup(Group_t* _group, UserInterface* _ui)
{
	ListItr begin, end;
	begin = ListItr_Begin(_group->m_members);
	end = ListItr_End(_group->m_members);
	
	begin = ListItr_FindFirst(begin, end, (PredicateFunction) IsKeysEqual, _ui->m_username);
	if(begin != end)
	{
		return begin;
	}
	else
	{
		return NULL;
	}
}

static Group_t* FindGroup(GroupDB_t* _groupDB, UserInterface* _ui)
{
	Group_t* group = NULL;
	HashMap_Find(_groupDB->m_groups, _ui->m_groupname, (void**) &group);
	return group;
}

static Group_t* CreateGroup(UserInterface* _ui)
{
	Group_t* group;
	static char multicastIP[] = BASE_MULTICAST_IP;
	static int multicastIPlastByte  = LAST_BYTE;
	
	group = (Group_t*) malloc(sizeof(Group_t));
	if(!group)
	{
		return NULL;
	}
	
	strcpy(group->m_groupname, _ui->m_groupname);
	strcpy(group->m_IP, multicastIP);
	
	printf("%s\n", group->m_IP);
	
	/*update multicast address to be given to the next group created*/
	sprintf(multicastIP + 8, "%d", multicastIPlastByte);
	++multicastIPlastByte;
	
	group->m_members = List_Create();
	if(!group->m_members)
	{
		free(group);
		return NULL;
	}
	
	return group;
}

static void DestroyGroup(Group_t* _group)
{
		List_Destroy(&_group->m_members, DestroyVal);
		free(_group);
}

static int HashFunc(char *str)
{
	int hash = 5381;
	int c;

	while (c = *(str++))
	hash = ((hash << 5) + hash) + c; 

	return hash;
}

static int IsKeysEqual(char* _key1, char* _key2)
{
	printf("key1: %s, key2: %s\n", _key1, _key2);
	return strcmp(_key1, _key2) == 0;
}

static void DestroyVal(void* _val)
{
	free(_val);
}

