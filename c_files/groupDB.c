#include "../inc/groupDB.h"
#include "../inc/HashMap.h"
#include "../inc/list.h"
#include "../inc/list_itr.h"
#include "../inc/list_functions.h"
#include <string.h>

struct UserDB_t
{
	HashMap* m_groups;	
};

struct Group_t
{
	char m_groupname[MAX_USERNAME_LENGTH];
	List* m_members;
};

/*---Static functions declarations---*/
static int HashFunc(char *str);
static int IsKeysEqual(char* _key1, char* _key2);
static void DestroyVal(void* _val);
static Group_t* FindGroup(GroupDB_t* _groupDB, UserInterface* _ui);
static Group_t* CreateGroup(UserInterface _ui);
static DestroyGroup(Group_t* _group);

/*---API functions defenitions---*/
GroupDB_t* CreateGroupDB(size_t _groupsCap)
{
	GroupDB_t* groupDB;
	Zlog* traceZlog;
	Zlog* errorZlog;
	
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
	
	return groupDB;
}

void DeleteGroupDB(GroupDB_t* _groupDB)
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
		DestroyGroup(group);
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
		HashMap_Remove(_groupDB, _ui->m_groupname, (void**) &group);
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
	
	traceZlog = ZlogGet("trace");
	errorZlog = ZlogGet("error");
	
	group = FindGroup(_groupDB, _ui);
	if(!group)
	{
		ZLOG_SEND(errorZlog, LOG_ERROR, "Group doesn't exist, %d",1);
		_ui->m_choice = GROUP_LEAVE_SUCCESS;
		return FAILURE;
	}
	
	if(IsUserMemberOfTheGroup(group, _ui))
	{
		/*FIXME */
	}
	else
	{
		ZLOG_SEND(errorZlog, LOG_ERROR, "User is already member of the group, %d",1);
		_ui->m_choice = GROUP_JOIN_FAILURE;
		return FAILURE;
	}
}

/*---Static functions definitions---*/
static int IsUserMemberOfTheGroup(Group_t* _group, UserInterface* _ui)
{
	ListItr begin, end;
	begin = ListItr_Begin(_group->m_members);
	end = ListItr_End(_group->m_members);
	
	begin = ListItr_FindFirst(begin, end, (PredicateFunction) IsKeysEqual, _ui->m_username);
	if(begin != end)
	{
		return 1;
	}
	else
	{
		return 0;
	}
}

static Group_t* FindGroup(_groupDB, _ui)
{
	Group_t* group = NULL;
	HashMap_Find(_groupDB->m_groups, _ui->m_groupname, (void**) &group);
	return group;
}

static Group_t* CreateGroup(UserInterface* _ui)
{
	Group_t* group;
	
	group = (Group_t*) malloc(sizeof(Group_t));
	if(!group)
	{
		return NULL;
	}
	
	strcmp(group->m_groupname, _ui->m_groupname);
	
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
		List_Destroy(*_group->m_members, DestroyVal);
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
	return strcmp(_key1, _key2) == 0;
}

static void DestroyVal(void* _val)
{
	free(_val);
}

