#include "../inc/serverManager.h"
#include "../inc/internal.h"
#include "../inc/logmngr.h"
#include "../inc/server.h"
#include "../inc/DBmanager.h"
#include <string.h>
#include <stdlib.h>

#define USERS_CAP 100
#define GROUPS_CAP 100

struct ServerManager_t
{
	DBmanager_t* m_DBmanager;
};

/*---Static functions declarations---*/
static ChatRes RergisterNewUser(DBmanager_t* _DBmanager, UserInterface* _ui); 
static ChatRes DeleteUser(DBmanager_t* _DBmanager, UserInterface* _ui); 
static ChatRes LoginExistingUser(DBmanager_t* _DBmanager, UserInterface* _ui); 
/*static ChatRes LogoutExistingUser(DBmanager_t* _DBmanager, UserInterface* _ui); */
static ChatRes CreateNewGroup(DBmanager_t* _DBmanager, UserInterface* _ui); 
static ChatRes DeleteGroup(DBmanager_t* _DBmanager, UserInterface* _ui); 
static ChatRes JoinGroup(DBmanager_t* _DBmanager, UserInterface* _ui); 
static ChatRes BanUser(DBmanager_t* _DBmanager, UserInterface* _ui); 
static ChatRes UnBanUser(DBmanager_t* _DBmanager, UserInterface* _ui); 
static ChatRes BanFromGroup(DBmanager_t* _DBmanager, UserInterface* _ui); 
static ChatRes UnBanFromGroup(DBmanager_t* _DBmanager, UserInterface* _ui); 
static ChatRes LogoutUser(DBmanager_t* _DBmanager, UserInterface* _ui);
static ChatRes LeaveGroup(DBmanager_t* _DBmanager, UserInterface* _ui);
static ChatRes StartChat(DBmanager_t* _DBmanager, UserInterface* _ui);
static ChatRes SaveDB(DBmanager_t* _DBmanager, UserInterface* _ui);
static ChatRes PrintOutGroups(DBmanager_t* _DBmanager, UserInterface* _ui);
static ChatRes PrintOutUsers(DBmanager_t* _DBmanager, UserInterface* _ui);
static void ProcessMessage(UserInterface* _ui, void* _context);
static void* OnNewConnection(void* _arg1, void* _arg2);
void* OnDisconnect(void* _arg1, void* _arg2);

/*=======================================*/
int main()
{
	ServerManager_t* serverManager;
	
	ZlogInit("log_config");
	
	serverManager = CreateServerManager(USERS_CAP, GROUPS_CAP);
	
	return OperateServer(serverManager);
}
/*=======================================*/

/*---API functions definitions---*/
ServerManager_t* CreateServerManager(size_t _cap, size_t _groupsCap)
{
	ServerManager_t* serverManager;
	Zlog* traceZlog;
	Zlog* errorZlog;

	traceZlog = ZlogGet("trace");
	errorZlog = ZlogGet("error");
	
	serverManager = (ServerManager_t*) malloc(sizeof(ServerManager_t));
	if(!serverManager)
	{
		ZLOG_SEND(errorZlog, LOG_TRACE, "couldn't create server manager, %d",1);
		return NULL;
	}

	serverManager->m_DBmanager = CreateDBManager(_cap, _groupsCap);
	if(!serverManager->m_DBmanager)
	{
		ZLOG_SEND(errorZlog, LOG_TRACE, "couldn't create DB manager, %d",1);
		free(serverManager);
		return NULL;
	}

	return serverManager;
}

void DestroyServerManager(ServerManager_t* _serverManager)
{
	DestroyDBmanager(_serverManager->m_DBmanager);
	free(_serverManager);
}

ChatRes OperateServer(ServerManager_t* _serverManager)
{
	ServerArguments args;
	Server_t* server;

	args.m_callbackNew = (CallbackFunc_t) OnNewConnection;	
	args.m_callbackExisting = (CallbackFunc_t) ProcessMessage;	
	args.m_callbackDeletion = (CallbackFunc_t) OnDisconnect;	
	args.m_configFile = "server_config";
	args.m_serverManager = _serverManager;

	server = ServerCreate(args);

	ServerRun(server);
	ServerDestroy(server);
	
	DestroyServerManager(_serverManager);

	return SUCCESS;
}

/*---Static functions defenitions---*/
static void ProcessMessage(UserInterface* _ui, void* _context)
{
	ChatRes (*userFunctions[16])(DBmanager_t* _DBmanager, UserInterface* _ui) = {RergisterNewUser, LoginExistingUser, LogoutUser, DeleteUser, CreateNewGroup, DeleteGroup, JoinGroup, LeaveGroup, StartChat, SaveDB, PrintOutGroups, PrintOutUsers, BanUser, UnBanUser, BanFromGroup, UnBanFromGroup};

	_ui->m_result = userFunctions[(int)_ui->m_choice - 1](GetServerManager((Server_t*)_context)->m_DBmanager, _ui);
	
	SendToCurrentClient((Server_t*)_context, _ui, sizeof(UserInterface));
}

void* OnNewConnection(void* _arg1, void* _arg2)
{
	Zlog* traceZlog;
	Zlog* errorZlog;

	traceZlog = ZlogGet("trace");
	errorZlog = ZlogGet("error");

	ZLOG_SEND(traceZlog, LOG_TRACE, "New connection created, %d",1);

	char string[] = "Welcome to Chat\n";
	SendToCurrentClient((Server_t*) _arg1, (void*) string, strlen(string) + 1);
	return NULL;
} 

void* OnDisconnect(void* _arg1, void* _arg2)
{
	return NULL;
}

static ChatRes PrintOutGroups(DBmanager_t* _DBmanager, UserInterface* _ui)
{
	Zlog* traceZlog;
	Zlog* errorZlog;

	traceZlog = ZlogGet("trace");
	errorZlog = ZlogGet("error");	

	#ifndef NDEBUG
		ZLOG_SEND(traceZlog, LOG_TRACE, "Saving DB on disk, %d",1);
	#endif

	return DBman_PrintOutGroups(_DBmanager, _ui); 
}

static ChatRes PrintOutUsers(DBmanager_t* _DBmanager, UserInterface* _ui)
{
	Zlog* traceZlog;
	Zlog* errorZlog;

	traceZlog = ZlogGet("trace");
	errorZlog = ZlogGet("error");	

	#ifndef NDEBUG
		ZLOG_SEND(traceZlog, LOG_TRACE, "Saving DB on disk, %d",1);
	#endif

	return DBman_PrintOutUsers(_DBmanager, _ui); 
}

static ChatRes SaveDB(DBmanager_t* _DBmanager, UserInterface* _ui)
{
	Zlog* traceZlog;
	Zlog* errorZlog;

	traceZlog = ZlogGet("trace");
	errorZlog = ZlogGet("error");	

	#ifndef NDEBUG
		ZLOG_SEND(traceZlog, LOG_TRACE, "Saving DB on disk, %d",1);
	#endif

	return DBman_Save(_DBmanager, _ui); 
}

static ChatRes RergisterNewUser(DBmanager_t* _DBmanager, UserInterface* _ui)
{
	Zlog* traceZlog;
	Zlog* errorZlog;

	traceZlog = ZlogGet("trace");
	errorZlog = ZlogGet("error");	

	ZLOG_SEND(traceZlog, LOG_TRACE, "Registering new user, %d",1);

	return DBman_RergisterNewUser(_DBmanager, _ui); 
}

static ChatRes DeleteUser(DBmanager_t* _DBmanager, UserInterface* _ui)
{
	Zlog* traceZlog;
	Zlog* errorZlog;

	traceZlog = ZlogGet("trace");
	errorZlog = ZlogGet("error");	


	ZLOG_SEND(traceZlog, LOG_TRACE, "Deleting user, %d",1);


	return DBman_DeleteUser(_DBmanager, _ui);
}

static ChatRes LoginExistingUser(DBmanager_t* _DBmanager, UserInterface* _ui)
{
	Zlog* traceZlog;
	Zlog* errorZlog;

	traceZlog = ZlogGet("trace");
	errorZlog = ZlogGet("error");	


	ZLOG_SEND(traceZlog, LOG_TRACE, "Logging in new user, %d",1);

	return DBman_LookUpUser(_DBmanager, _ui);
}

/*static ChatRes LogoutExistingUser(DBmanager_t* _DBmanager, UserInterface* _ui)
{
	Zlog* traceZlog;
	Zlog* errorZlog;

	traceZlog = ZlogGet("trace");
	errorZlog = ZlogGet("error");	

	#ifndef NDEBUG
		ZLOG_SEND(traceZlog, LOG_TRACE, "Logging out new user, %d",1);
		printf("Logging out new user\n");
	#endif

	return SUCCESS;
}*/

static ChatRes CreateNewGroup(DBmanager_t* _DBmanager, UserInterface* _ui)
{
	Zlog* traceZlog;
	Zlog* errorZlog;

	traceZlog = ZlogGet("trace");
	errorZlog = ZlogGet("error");	

	ZLOG_SEND(traceZlog, LOG_TRACE, "Creating new group, %d",1);


	return DBman_CreateNewGroup(_DBmanager, _ui);
}

static ChatRes DeleteGroup(DBmanager_t* _DBmanager, UserInterface* _ui)
{
	Zlog* traceZlog;
	Zlog* errorZlog;

	traceZlog = ZlogGet("trace");
	errorZlog = ZlogGet("error");	

	ZLOG_SEND(traceZlog, LOG_TRACE, "Deleting group, %d",1);


	return DBman_DeleteGroup(_DBmanager, _ui);
}

static ChatRes JoinGroup(DBmanager_t* _DBmanager, UserInterface* _ui)
{
	Zlog* traceZlog;
	Zlog* errorZlog;

	traceZlog = ZlogGet("trace");
	errorZlog = ZlogGet("error");	


	ZLOG_SEND(traceZlog, LOG_TRACE, "Joining group, %d",1);


	return DBman_UserJoinGroup(_DBmanager, _ui);
}

static ChatRes LeaveGroup(DBmanager_t* _DBmanager, UserInterface* _ui)
{
	Zlog* traceZlog;
	Zlog* errorZlog;

	traceZlog = ZlogGet("trace");
	errorZlog = ZlogGet("error");	

	ZLOG_SEND(traceZlog, LOG_TRACE, "Leaving group, %d",1);

	
	return DBman_UserLeaveGroup(_DBmanager, _ui);
}

static ChatRes BanUser(DBmanager_t* _DBmanager, UserInterface* _ui)
{
	Zlog* traceZlog;
	Zlog* errorZlog;

	traceZlog = ZlogGet("trace");
	errorZlog = ZlogGet("error");	

	ZLOG_SEND(traceZlog, LOG_TRACE, "Banning user, %d",1);


	return DBman_BanUser(_DBmanager, _ui);
}

static ChatRes UnBanUser(DBmanager_t* _DBmanager, UserInterface* _ui)
{
	Zlog* traceZlog;
	Zlog* errorZlog;

	traceZlog = ZlogGet("trace");
	errorZlog = ZlogGet("error");	

	ZLOG_SEND(traceZlog, LOG_TRACE, "Unbanning user, %d",1);

	return DBman_UnBanUser(_DBmanager, _ui);
}

static ChatRes BanFromGroup(DBmanager_t* _DBmanager, UserInterface* _ui)
{
	Zlog* traceZlog;
	Zlog* errorZlog;

	traceZlog = ZlogGet("trace");
	errorZlog = ZlogGet("error");	

	ZLOG_SEND(traceZlog, LOG_TRACE, "Banning user from group, %d",1);

	return SUCCESS;
}

static ChatRes UnBanFromGroup(DBmanager_t* _DBmanager, UserInterface* _ui)
{
	Zlog* traceZlog;
	Zlog* errorZlog;

	traceZlog = ZlogGet("trace");
	errorZlog = ZlogGet("error");	

	ZLOG_SEND(traceZlog, LOG_TRACE, "Unbanning user from group, %d",1);

	return SUCCESS;
}

static ChatRes LogoutUser(DBmanager_t* _DBmanager, UserInterface* _ui)
{
	Zlog* traceZlog;
	Zlog* errorZlog;

	traceZlog = ZlogGet("trace");
	errorZlog = ZlogGet("error");	

	ZLOG_SEND(traceZlog, LOG_TRACE, "Logging out user, %d",1);

	return DBman_LogoutUser(_DBmanager, _ui);
}

static ChatRes StartChat(DBmanager_t* _DBmanager, UserInterface* _ui)
{
	Zlog* traceZlog;
	Zlog* errorZlog;

	traceZlog = ZlogGet("trace");
	errorZlog = ZlogGet("error");
	
	ZLOG_SEND(traceZlog, LOG_TRACE, "Starting new chat, %d",1);
	
	return DBman_StartChat(_DBmanager, _ui);
}

























































