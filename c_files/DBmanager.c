#include "../inc/DBmanager.h"
#include "../inc/logmngr.h"
#include "../inc/userDB.h"
#include "../inc/groupDB.h"
#include <stdlib.h>

struct DBmanager_t
{
	UserDB_t* m_userDB;
	GroupDB_t* m_groupDB;
};

DBmanager_t* CreateDBManager(size_t _cap, size_t _groupsCap)
{
	DBmanager_t* DBmanager;
	Zlog* traceZlog;
	Zlog* errorZlog;

	traceZlog = ZlogGet("trace");
	errorZlog = ZlogGet("error");

	DBmanager = (DBmanager_t*) malloc(sizeof(DBmanager));
	if(!DBmanager)
	{
		ZLOG_SEND(errorZlog, LOG_ERROR, "Couldn't create DB manager, %d",1);
	}

	DBmanager->m_userDB = CreateUserDB(_cap);
	if(!DBmanager->m_userDB)
	{
		ZLOG_SEND(errorZlog, LOG_ERROR, "Couldn't create userDB, %d",1);
		free(DBmanager);
		return NULL;
	}
	
	DBmanager->m_groupDB = CreateGroupDB(_groupsCap);
	if(!DBmanager->m_groupDB)
	{
		ZLOG_SEND(errorZlog, LOG_ERROR, "Couldn't create group DB, %d",1);
		free(DBmanager);
		free(DBmanager->m_userDB);
		return NULL;
	}

	return DBmanager;
}

void DestroyDBmanager(DBmanager_t* _DBmanager)
{
	DestroyUserDB(_DBmanager->m_userDB);
	DestroyGroupDB(_DBmanager->m_groupDB);
	free(_DBmanager); 	
}

ChatRes DBman_RergisterNewUser(DBmanager_t* _DBmanager, UserInterface* _ui)
{
	return DB_RergisterNewUser(_DBmanager->m_userDB, _ui);
}

ChatRes DBman_DeleteUser(DBmanager_t* _DBmanager, UserInterface* _ui)
{
	return DB_DeleteUser(_DBmanager->m_userDB, _ui);
}
 
ChatRes DBman_BanUser(DBmanager_t* _DBmanager, UserInterface* _ui)
{
	return DB_BanUser(_DBmanager->m_userDB, _ui);
}
 
ChatRes DBman_UnBanUser(DBmanager_t* _DBmanager, UserInterface* _ui)
{
	return DB_UnBanUser(_DBmanager->m_userDB, _ui);
}
 
ChatRes DBman_LookUpUser(DBmanager_t* _DBmanager, UserInterface* _ui)
{
	return DB_LookUpUser(_DBmanager->m_userDB, _ui);
}

ChatRes DBman_LogoutUser(DBmanager_t* _DBmanager, UserInterface* _ui)
{
	return DB_LogoutUser(_DBmanager->m_userDB, _ui);
}

ChatRes DBman_CreateNewGroup(DBmanager_t* _DBmanager, UserInterface* _ui)
{
	return GroupDB_CreateNewGroup(_DBmanager->m_groupDB, _ui);
}

ChatRes DBman_DeleteGroup(DBmanager_t* _DBmanager, UserInterface* _ui)
{
	return GroupDB_DeleteGroup(_DBmanager->m_groupDB, _ui);
}

ChatRes DBman_UserJoinGroup(DBmanager_t* _DBmanager, UserInterface* _ui)
{
	return GroupDB_UserJoinGroup(_DBmanager->m_groupDB, _ui);
}

ChatRes DBman_UserLeaveGroup(DBmanager_t* _DBmanager, UserInterface* _ui)
{
	return GroupDB_UserLeaveGroup(_DBmanager->m_groupDB, _ui);
}

ChatRes DBman_StartChat(DBmanager_t* _DBmanager, UserInterface* _ui)
{
	return GroupDB_StartChat(_DBmanager->m_groupDB, _ui);
}
















