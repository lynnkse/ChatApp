#include "../inc/serverManager.h"
#include "../inc/internal.h"
#include "../inc/logmngr.h"
#include "../inc/server.h"
#include <string.h>

/*---Static functions declarations---*/
static ChatRes RergisterNewUser(UserInterface* _ui); 
static ChatRes DeleteUser(UserInterface* _ui); 
static ChatRes LoginExistingUser(UserInterface* _ui); 
static ChatRes LogoutExistingUser(UserInterface* _ui); 
static ChatRes CreateNewGroup(UserInterface* _ui); 
static ChatRes DeleteGroup(UserInterface* _ui); 
static ChatRes JoinGroup(UserInterface* _ui); 
static ChatRes BanUser(UserInterface* _ui); 
static ChatRes UnBanUser(UserInterface* _ui); 
static ChatRes BanFromGroup(UserInterface* _ui); 
static ChatRes UnBanFromGroup(UserInterface* _ui); 
static void ProcessMessage(UserInterface* _ui, void* _context);
static void* OnNewConnection(void* _arg1, void* _arg2);
void* OnDisconnect(void* _arg1, void* _arg2);

int main()
{
	ZlogInit("log_config");
	
	return OperateServer();
}

/*---API functions definitions---*/
ChatRes OperateServer()
{
	ServerArguments args;
	Server_t* server;

	args.m_callbackNew = (CallbackFunc_t) OnNewConnection;	
	args.m_callbackExisting = (CallbackFunc_t) ProcessMessage;	
	args.m_callbackDeletion = (CallbackFunc_t) OnDisconnect;	
	args.m_configFile = "server_config";

	server = ServerCreate(args);

	ServerRun(server);
	ServerDestroy(server);

	return SUCCESS;
}

/*---Static functions defenitions---*/
static void ProcessMessage(UserInterface* _ui, void* _context)
{
	ChatRes (*userFunctions[11])(UserInterface* _ui) = {RergisterNewUser, DeleteUser, LoginExistingUser, LogoutExistingUser, CreateNewGroup, DeleteGroup, JoinGroup, BanUser, UnBanUser, BanFromGroup, UnBanFromGroup};

	userFunctions[(int)_ui->m_choice - 1](_ui);
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

static ChatRes RergisterNewUser(UserInterface* _ui)
{
	Zlog* traceZlog;
	Zlog* errorZlog;

	traceZlog = ZlogGet("trace");
	errorZlog = ZlogGet("error");	

	#ifndef NDEBUG
		ZLOG_SEND(traceZlog, LOG_TRACE, "Registering new user, %d",1);
		printf("Registering new user\n");
	#endif

	return SUCCESS;
}

static ChatRes DeleteUser(UserInterface* _ui)
{
	Zlog* traceZlog;
	Zlog* errorZlog;

	traceZlog = ZlogGet("trace");
	errorZlog = ZlogGet("error");	

	#ifndef NDEBUG
		ZLOG_SEND(traceZlog, LOG_TRACE, "Deleting new user, %d",1);
		printf("Deleting new user\n");
	#endif

	return SUCCESS;
}

static ChatRes LoginExistingUser(UserInterface* _ui)
{
	Zlog* traceZlog;
	Zlog* errorZlog;

	traceZlog = ZlogGet("trace");
	errorZlog = ZlogGet("error");	

	#ifndef NDEBUG
		ZLOG_SEND(traceZlog, LOG_TRACE, "Logging in new user, %d",1);
		printf("Logging in new user\n");
	#endif

	return SUCCESS;
}

static ChatRes LogoutExistingUser(UserInterface* _ui)
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
}

static ChatRes CreateNewGroup(UserInterface* _ui)
{
	Zlog* traceZlog;
	Zlog* errorZlog;

	traceZlog = ZlogGet("trace");
	errorZlog = ZlogGet("error");	

	#ifndef NDEBUG
		ZLOG_SEND(traceZlog, LOG_TRACE, "Creating new group, %d",1);
		printf("Creating new group\n");
	#endif

	return SUCCESS;
}

static ChatRes DeleteGroup(UserInterface* _ui)
{
	Zlog* traceZlog;
	Zlog* errorZlog;

	traceZlog = ZlogGet("trace");
	errorZlog = ZlogGet("error");	

	#ifndef NDEBUG
		ZLOG_SEND(traceZlog, LOG_TRACE, "Deleting group, %d",1);
		printf("Deleting group\n");
	#endif

	return SUCCESS;
}

static ChatRes JoinGroup(UserInterface* _ui)
{
	Zlog* traceZlog;
	Zlog* errorZlog;

	traceZlog = ZlogGet("trace");
	errorZlog = ZlogGet("error");	

	#ifndef NDEBUG
		ZLOG_SEND(traceZlog, LOG_TRACE, "Joining group, %d",1);
		printf("Joining group\n");
	#endif

	return SUCCESS;
}

static ChatRes BanUser(UserInterface* _ui)
{
	Zlog* traceZlog;
	Zlog* errorZlog;

	traceZlog = ZlogGet("trace");
	errorZlog = ZlogGet("error");	

	#ifndef NDEBUG
		ZLOG_SEND(traceZlog, LOG_TRACE, "Banning user, %d",1);
		printf("Banning user\n");
	#endif

	return SUCCESS;
}

static ChatRes UnBanUser(UserInterface* _ui)
{
	Zlog* traceZlog;
	Zlog* errorZlog;

	traceZlog = ZlogGet("trace");
	errorZlog = ZlogGet("error");	

	#ifndef NDEBUG
		ZLOG_SEND(traceZlog, LOG_TRACE, "Unbanning user, %d",1);
		printf("Unbanning user\n");
	#endif

	return SUCCESS;
}

static ChatRes BanFromGroup(UserInterface* _ui)
{
	Zlog* traceZlog;
	Zlog* errorZlog;

	traceZlog = ZlogGet("trace");
	errorZlog = ZlogGet("error");	

	#ifndef NDEBUG
		ZLOG_SEND(traceZlog, LOG_TRACE, "Banning user from group, %d",1);
		printf("Banning user from group\n");
	#endif

	return SUCCESS;
}

static ChatRes UnBanFromGroup(UserInterface* _ui)
{
	Zlog* traceZlog;
	Zlog* errorZlog;

	traceZlog = ZlogGet("trace");
	errorZlog = ZlogGet("error");	

	#ifndef NDEBUG
		ZLOG_SEND(traceZlog, LOG_TRACE, "Unbanning user from group, %d",1);
		printf("Unbanning user from group\n");
	#endif

	return SUCCESS;
}

























































