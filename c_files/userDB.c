#include "../inc/userDB.h"
#include "../inc/HashMap.h"
#include "../inc/logmngr.h"
#include <string.h>
#include <stdlib.h>

#define MAX_USERNAME_LENGTH 32
#define MAX_PASSWORD_LENGTH 32
#define SERIALIZATION_FILE "serialization_users"

struct UserDB_t
{
	HashMap* m_users;	
};

struct User_t
{
	char m_username[MAX_USERNAME_LENGTH];
	char m_password[MAX_PASSWORD_LENGTH];
	int m_isBanned;
	int m_isActive;
	int m_isLoggedIn;
};

/*---Static functions declarations---*/
static int HashFunc(char *str);
static int IsKeysEqual(char* _key1, char* _key2);
static void DestroyVal(void* _val);
static int SerizlizeItem(void* _key, void* _value, FILE* _fp);
static void ReadUserDBfromFile(HashMap* _map, FILE* _fp);
static int PrintOutName(void* _key, User_t* _user, void* _context);

/*---API functions defenition---*/
UserDB_t* CreateUserDB(size_t _cap)
{
	Zlog* traceZlog;
	Zlog* errorZlog;
	UserDB_t* userDB;
	FILE* fp = NULL;

	traceZlog = ZlogGet("trace");
	errorZlog = ZlogGet("error");

	userDB = (UserDB_t*) malloc(sizeof(UserDB_t));
	if(!userDB)
	{
		ZLOG_SEND(errorZlog, LOG_ERROR, "Failed to create user DB, %d",1);
		return NULL;
	}

	userDB->m_users = HashMap_Create(_cap, (HashFunction) HashFunc, (EqualityFunction) IsKeysEqual);
	if(!userDB->m_users)
	{
		ZLOG_SEND(errorZlog, LOG_ERROR, "Failed to create users HashMap, %d",1);
		free(userDB);
		return NULL;
	}

	fp = fopen(SERIALIZATION_FILE, "r");
	if(fp)
	{
		ReadUserDBfromFile(userDB->m_users, fp);
		fclose(fp);
	}

	ZLOG_SEND(traceZlog, LOG_TRACE, "User DB was created, %d",1);

	return userDB;
}


void DestroyUserDB(UserDB_t* _userDB)
{
	HashMap_Destroy(&_userDB->m_users, NULL, DestroyVal);
	free(_userDB);
}

ChatRes DB_RergisterNewUser(UserDB_t* _userDB, UserInterface* _ui)
{
	User_t* user;
	Zlog* traceZlog;
	Zlog* errorZlog;

	traceZlog = ZlogGet("trace");
	errorZlog = ZlogGet("error");

	user = (User_t*) malloc(sizeof(User_t));
	if(!user)
	{
		ZLOG_SEND(errorZlog, LOG_ERROR, "Failed to create users HashMap, %d",1);
		_ui->m_choice = REGISTER_FAIL;
		return FAILURE;
	}

	strcpy(user->m_username, _ui->m_username);
	strcpy(user->m_password, _ui->m_password);
	user->m_isBanned = 0;
	user->m_isActive = 1;

	if(HashMap_Insert(_userDB->m_users, user->m_username, user) == MAP_SUCCESS)
	{
		ZLOG_SEND(traceZlog, LOG_TRACE, "New user was created, %d",1);
		_ui->m_choice = REGISTER_SUCCESS;
		return SUCCESS;
	}
	else
	{
		ZLOG_SEND(errorZlog, LOG_ERROR, "Fail to insert new user, %d",1);
		free(user);
		_ui->m_choice = REGISTER_FAIL;
		return FAILURE;
	}
}

ChatRes DB_DeleteUser(UserDB_t* _userDB, UserInterface* _ui)
{
	User_t* user;
	Zlog* traceZlog;
	Zlog* errorZlog;

	traceZlog = ZlogGet("trace");
	errorZlog = ZlogGet("error");
	
	if(HashMap_Remove(_userDB->m_users, _ui->m_username, (void**) &user) == MAP_SUCCESS)
	{
		user->m_isActive = 0;
		ZLOG_SEND(traceZlog, LOG_TRACE, "User deleted, %d",1);
		_ui->m_choice = DELETE_SUCCESS;
		return SUCCESS;
	}
	else
	{
		ZLOG_SEND(errorZlog, LOG_ERROR, "Fail to delete user, %d",1);
		return FAILURE;
	}
}

ChatRes DB_BanUser(UserDB_t* _userDB, UserInterface* _ui)
{
	User_t* user;
	Zlog* traceZlog;
	Zlog* errorZlog;

	traceZlog = ZlogGet("trace");
	errorZlog = ZlogGet("error");
	
	if(HashMap_Find(_userDB->m_users, _ui->m_username, (void**) &user) == MAP_SUCCESS)
	{
		user->m_isBanned = 1;
		ZLOG_SEND(traceZlog, LOG_TRACE, "User banned, %d",1);
		return SUCCESS;
	}
	else
	{
		ZLOG_SEND(errorZlog, LOG_ERROR, "Fail to ban user, %d",1);
		return FAILURE;
	}	
} 

ChatRes DB_UnBanUser(UserDB_t* _userDB, UserInterface* _ui)
{
	User_t* user;
	Zlog* traceZlog;
	Zlog* errorZlog;

	traceZlog = ZlogGet("trace");
	errorZlog = ZlogGet("error");
	
	if(HashMap_Find(_userDB->m_users, _ui->m_username, (void**) &user) == MAP_SUCCESS)
	{
		user->m_isBanned = 0;
		ZLOG_SEND(traceZlog, LOG_TRACE, "User unbanned, %d",1);
		return SUCCESS;
	}
	else
	{
		ZLOG_SEND(errorZlog, LOG_ERROR, "Fail to unban user, %d",1);
		return FAILURE;
	}	
}

ChatRes DB_LookUpUser(UserDB_t* _userDB, UserInterface* _ui)
{
	User_t* user;
	Zlog* traceZlog;
	Zlog* errorZlog;

	traceZlog = ZlogGet("trace");
	errorZlog = ZlogGet("error");
	
	if(HashMap_Find(_userDB->m_users, _ui->m_username, (void**) &user) == MAP_SUCCESS)
	{
		if(user->m_isActive == 1 && user->m_isBanned == 0 && strcmp(_ui->m_password, user->m_password) == 0)
		{
			ZLOG_SEND(traceZlog, LOG_TRACE, "User found, %d",1);
			user->m_isLoggedIn = 1;
			_ui->m_choice = LOGIN_SUCCESS;
			return SUCCESS;
		}
		else
		{
			_ui->m_choice = LOGIN_FAIL;
			return FAILURE;
		}
	}
	else
	{
		ZLOG_SEND(traceZlog, LOG_TRACE, "User not found, %d",1);
		_ui->m_choice = LOGIN_FAIL;
		return FAILURE;
	}	
}

ChatRes DB_LogoutUser(UserDB_t* _userDB, UserInterface* _ui)/*logout failure should never happen*/
{
	User_t* user;
	Zlog* traceZlog;
	Zlog* errorZlog;

	traceZlog = ZlogGet("trace");
	errorZlog = ZlogGet("error");
	
	if(HashMap_Find(_userDB->m_users, _ui->m_username, (void**) &user) == MAP_SUCCESS)
	{
		ZLOG_SEND(traceZlog, LOG_TRACE, "User found, %d",1);
		if(user->m_isLoggedIn)
		{
			user->m_isLoggedIn = 0;
			_ui->m_choice = LOGOUT_SUCCESS;
			return SUCCESS;
		}
		else
		{
			ZLOG_SEND(traceZlog, LOG_TRACE, "User isn't logged in, %d",1);
			return FAILURE;
		}
	}
	else
	{
		ZLOG_SEND(traceZlog, LOG_TRACE, "User not found, %d",1);
		return FAILURE;
	}	
}

ChatRes DB_Save(UserDB_t* _userDB, UserInterface* _ui)
{
	FILE* fp;
	ZLOGS_INITIALIZATION;

	fp = fopen(SERIALIZATION_FILE, "w");
	if(!fp)
	{
		ZLOG_SEND(errorZlog, LOG_ERROR, "Unable to open file for serialization, %d",1);
		_ui->m_choice = SAVE_DB_FAILURE;
		return FAILURE;
	}
		
	HashMap_ForEach(_userDB->m_users, (KeyValueActionFunction) SerizlizeItem, (void*) fp);

	fclose(fp);
	
	_ui->m_choice = SAVE_DB_SUCCESS;

	return SUCCESS;
}

ChatRes DB_PrintOutUsers(UserDB_t* _userDB, UserInterface* _ui)
{
	HashMap_ForEach(_userDB->m_users, (KeyValueActionFunction) PrintOutName, NULL);
	_ui->m_choice = PRINT_OUT_USERS_SUCCESS;
	return SUCCESS;
}

/*---Static functions definitions---*/
static int PrintOutName(void* _key, User_t* _user, void* _context)
{
	printf("%s\n", _user->m_username);
	return 1;
}

static void ReadUserDBfromFile(HashMap* _map, FILE* _fp)
{
	User_t* user;	
	size_t res = 0;	
	/*TODO change to read the whole file at once*/
	do
	{
		user = (User_t*) malloc(sizeof(User_t));		
		res = fread(user, sizeof(User_t), 1, _fp);
		HashMap_Insert(_map, user->m_username, user);
	}
	while(res);
}

static int SerizlizeItem(void* _key, void* _value, FILE* _fp)
{
	fwrite(_value, sizeof(User_t), 1, _fp);
	return 1;
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




















