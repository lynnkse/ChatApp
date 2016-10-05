#include "../inc/DBmanager.h"
#include "../inc/logmngr.h"
#include "../inc/userDB.h"
#include <stdlib.h>

struct DBmanager_t
{
	UserDB_t* m_userDB;
};

DBmanager_t* CreateDBManager(size_t _cap)
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

	return DBmanager;
}

void DestroyDBmanager(DBmanager_t* _DBmanager)
{
	DestroyUserDB(_DBmanager->m_userDB);
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

















