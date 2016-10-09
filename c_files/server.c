#include "../inc/server.h"
#include "../inc/HashMap.h"
#include "../inc/logmngr.h"
#include "../inc/read_config.h"
#include "../inc/internal.h"
#include "../inc/serverManager.h"

#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/select.h> 
#include <arpa/inet.h>

#define IP_ADDR "127.0.0.1" /*for testing*/
#define PORT_NUM 1041 /*for testing*/
#define BUFF_SIZE 1024 /*for testing*/
#define HASHMAP_CAP 1000
#define KEY_LEN 128
#define NUM_OF_CONNECTIONS_WAITING 1
#define MAX_NUMBER_OF_CONNECTIONS 50
/*#define TIMEOUT 1*/

int g_isAlive = 1;

typedef struct Connection_t
{
	int m_socket;
	time_t m_time;
} Connection_t;

struct Server_t
{
	int m_serverSocket;
	HashMap* m_clientSockets;
	fd_set m_rfds;
	struct sockaddr_in m_serverAddr;
	int m_currSocket;
	int m_maxSocket;
	char* m_currSocketKey;
	int m_port;
	char* m_IP;
	CallbackFunc_t m_callbackNew;
	CallbackFunc_t m_callbackExisting;
	CallbackFunc_t m_callbackDeletion;
	void* m_buffer;
	int m_buffSize;
	int m_maxNumOfClients;
	int m_timeout;
	ServerManager_t* m_serverManager;
};

static void sHandler(int _sigNum, siginfo_t* _sigInfo, char* _sigContext)
{
	Zlog* zlog;
	zlog = ZlogGet("trace");
	ZLOG_SEND(zlog, LOG_TRACE, "Server stopped by user %d", 1);
	g_isAlive = 0;
}

static void SaveNewClientSocket(Server_t* _server)
{
	
	socklen_t len;
	struct sockaddr_storage addr;
	char ipstr[INET6_ADDRSTRLEN];
	int port;
	Connection_t* connection;
	Zlog* zlog;

	zlog = ZlogGet("error");

	connection = (Connection_t*) malloc(sizeof(Connection_t));
	if(NULL == connection)
	{
		ZLOG_SEND(zlog, LOG_ERROR, "couldn't allocate memory for new connection %d", 1);
	}

	char* key = (char*) malloc(KEY_LEN * sizeof(char)); 

	len = sizeof(addr);
	getpeername(_server->m_currSocket, (struct sockaddr*)&addr, &len);

	if(addr.ss_family == AF_INET)
	{
		struct sockaddr_in *s = (struct sockaddr_in *) &addr;
		port = ntohs(s->sin_port);
		inet_ntop(AF_INET, &s->sin_addr, ipstr, sizeof(ipstr));
	}
	else
	{
		struct sockaddr_in6 *s = (struct sockaddr_in6 *) &addr;
		port = ntohs(s->sin6_port);
		inet_ntop(AF_INET6, &s->sin6_addr, ipstr, sizeof(ipstr));
	}

	sprintf(key, "%s%d%s%d", ipstr, port, _server->m_IP, _server->m_port);
	
	connection->m_socket = _server->m_currSocket;
	connection->m_time = time(NULL);
		
	HashMap_Insert(_server->m_clientSockets, key, connection);
}

static int HashFunc(char *str)
{
	int hash = 5381;
	int c;

	while (c = *(str++))
	hash = ((hash << 5) + hash) + c; 

	return hash;
}

static void KeyDestraction(void* _key)
{
	free(_key);
}

static void ValDestraction(Connection_t* _val)
{
	close(_val->m_socket);	
	free(_val);
}

static int FdSetFunc(char* _key, Connection_t* _value, Server_t* _server)
{
	FD_SET(_value->m_socket, &_server->m_rfds);

	return 1;
}

static int FdIsSetFunc(char* _key, Connection_t* _value, Server_t* _server)
{
	time_t currTime;
	if(FD_ISSET(_value->m_socket, &_server->m_rfds) == 0)
	{
		time(&currTime);
		_value->m_time = currTime;
		return 1;
	}
	else
	{
		_server->m_currSocket = _value->m_socket;
		_server->m_currSocketKey = _key;
		return 0;
	}
}

static int KeyEq(char* _str1, char* _str2)
{
	return (strcmp(_str1, _str2) == 0);
}

Server_t* ServerCreate(ServerArguments _arguments)
{
	Server_t* server;
	Config* configs;
	HashMap* configMap;
	char* sMaxNumOfClients;
	char* sTimeout;
	char* sBuffSize;
	char* IP;
	char* sPort;
	struct sigaction sAction;	

	Zlog* zlog = ZlogGet("error");
	
	server = (Server_t*) malloc(sizeof(Server_t));
	if(NULL == server)
	{
		ZLOG_SEND(zlog, LOG_ERROR, "couldn't allocate server %d", 1);
		exit(-1);
	}
	
	sAction.sa_sigaction = (void(*)(int, siginfo_t*, void*))sHandler;
	sAction.sa_flags = SA_SIGINFO;
	sigaction(SIGINT, &sAction, NULL);

	configs = ReadConfig(_arguments.m_configFile);
	configMap = GetNextConfig(configs);	
	HashMap_Remove(configMap, "MaxNumOfClients", (void**) &sMaxNumOfClients);
	HashMap_Remove(configMap, "TimeoutSec", (void**) &sTimeout);
	HashMap_Remove(configMap, "BuffSize", (void**) &sBuffSize);
	HashMap_Remove(configMap, "IP", (void**) &IP);
	HashMap_Remove(configMap, "Port", (void**) &sPort);
	
	server->m_port = atoi(sPort);
	server->m_IP = IP;
	server->m_currSocket = 0;
	server->m_callbackNew = _arguments.m_callbackNew;
	server->m_callbackExisting = _arguments.m_callbackExisting;
	server->m_callbackDeletion = _arguments.m_callbackDeletion;
	server->m_buffSize = atoi(sBuffSize);
	server->m_maxNumOfClients = atoi(sMaxNumOfClients);
	server->m_timeout = atoi(sTimeout);
	server->m_serverManager = _arguments.m_serverManager;

	server->m_clientSockets = HashMap_Create(server->m_maxNumOfClients, (HashFunction) HashFunc, (EqualityFunction) KeyEq);
	
	server->m_buffer = malloc(server->m_buffSize);
	if(NULL == server->m_buffer)
	{
		free(server);
		ZLOG_SEND(zlog, LOG_ERROR, "couldn't allocate server %d", 1);
		exit(-1);
	}

	if((server->m_serverSocket = socket(PF_INET, SOCK_STREAM, 0)) == -1)
	{
		free(server->m_buffer);
		free(server);
		ZLOG_SEND(zlog, LOG_ERROR, "couldn't open socket %d", 1);
		exit(-1);
	}
	
	server->m_maxSocket = server->m_serverSocket;

	
	server->m_serverAddr.sin_family = AF_INET;
	server->m_serverAddr.sin_port = htons(server->m_port);
	server->m_serverAddr.sin_addr.s_addr = htonl(INADDR_ANY);
	
	memset(server->m_serverAddr.sin_zero, 0, sizeof(server->m_serverAddr.sin_zero));
	
	if(bind(server->m_serverSocket, (struct sockaddr *) &server->m_serverAddr, sizeof(server->m_serverAddr)) == -1)
	{
		ZLOG_SEND(zlog, LOG_ERROR, "couldn't open socket %d", 1);
		exit(-1);
	}
	
	FD_ZERO(&server->m_rfds);
	FD_SET(server->m_serverSocket, &server->m_rfds);
	
	return server;	
}

void ServerDestroy(Server_t* _server)
{
	Zlog* zlog;
	zlog = ZlogGet("trace");
	ZLOG_SEND(zlog, LOG_TRACE, "Server destroyed %d", 1);
	
	close(_server->m_serverSocket);
	HashMap_Destroy(&_server->m_clientSockets, KeyDestraction, (void(*)(void*))ValDestraction);
	free(_server->m_buffer);
	free(_server);
	
	exit(0);
}

static void NewClientConnection(Server_t* _server)
{
	socklen_t addrSize = sizeof(struct sockaddr);	

	_server->m_currSocket = accept(_server->m_serverSocket, (struct sockaddr*) &_server->m_serverAddr, &addrSize);
	SaveNewClientSocket(_server);
	if(_server->m_currSocket > _server->m_maxSocket)
	{
		_server->m_maxSocket = _server->m_currSocket;
	}
}

static void DeleteConnection(Server_t* _server)
{
	void* valToDestroy;	

	Zlog* zlog = ZlogGet("debug");

	close(_server->m_currSocket);
	HashMap_Remove(_server->m_clientSockets, _server->m_currSocketKey, &valToDestroy);
	ValDestraction(valToDestroy);
	ZLOG_SEND(zlog, LOG_TRACE, "Socket deleted %d", 1);
}

static void ServerIteration(Server_t* _server)
{
	int result = 0;
	int readBytesNum;

	Zlog* zlog = ZlogGet("error");
	Zlog* zlogTrace = ZlogGet("trace");

	FD_ZERO(&_server->m_rfds);
	FD_SET(_server->m_serverSocket, &_server->m_rfds);
	
	HashMap_ForEach(_server->m_clientSockets, (KeyValueActionFunction) FdSetFunc, _server);
	
	if(select(_server->m_maxSocket + 1, &_server->m_rfds, NULL, NULL, NULL) == -1)
	{
		ZLOG_SEND(zlog, LOG_ERROR, "select() failed %d", 1);
	}
	
	if(FD_ISSET(_server->m_serverSocket, &_server->m_rfds) == 1)
	{
		NewClientConnection(_server);	
		ZLOG_SEND(zlogTrace, LOG_TRACE, "New connection created, %d", 1);
		_server->m_callbackNew(_server, NULL);				
	}
	else
	{
		HashMap_ForEach(_server->m_clientSockets, (KeyValueActionFunction) FdIsSetFunc, _server);
		readBytesNum = read(_server->m_currSocket, _server->m_buffer, _server->m_buffSize);
		
		if(readBytesNum == 0)
		{
			_server->m_callbackDeletion(&_server->m_currSocket, _server);
			DeleteConnection(_server);	
			ZLOG_SEND(zlogTrace, LOG_TRACE, "Connection deleted. FIN %d", 1);				
		}
		else if(readBytesNum > 0)
		{
			ZLOG_SEND(zlogTrace, LOG_TRACE, "Server recieved message from socket %d", _server->m_currSocket);			
			_server->m_callbackExisting(_server->m_buffer, _server);		
		}
		else
		{
			_server->m_callbackDeletion(&_server->m_currSocket, _server);
			DeleteConnection(_server);
			ZLOG_SEND(zlogTrace, LOG_TRACE, "Connection deleted. RST %d", 1);
		}
	}
	/* FIXME
	if(HashMap_Size(_server->m_clientSockets) > MAX_NUMBER_OF_CONNECTIONS)
	{
		HashMap_ForEach(_server->m_clientSockets, (KeyValueActionFunction) DeleteDeadConnection, _server);
	}*/
}

void ServerRun(Server_t* _server)
{	
	listen(_server->m_serverSocket, NUM_OF_CONNECTIONS_WAITING);
	
	while(g_isAlive)
	{
		ServerIteration(_server);
	}
}

int GetCurrentSocket(Server_t* _server)
{
	return _server->m_currSocket;
}

void SendToCurrentClient(Server_t* _server, void* _data, size_t _dataSize)
{
	write(_server->m_currSocket, _data, _dataSize);
}

ServerManager_t* GetServerManager(Server_t* _server)
{
	return _server->m_serverManager;
}

/****************************TEST*****************************/

void PrintFunc(char* _str, void* _context)
{
	UserInterface* userInterface;
	
	userInterface = (UserInterface*) _str;
	
	printf("Username: %s\nPassword: %s\nCommand: %u\n", userInterface->m_username, userInterface->m_password, userInterface->m_choice);
}

void* OnCreateFunc(void* _arg1, void* _arg2)
{
	char string[] = "hi client";
	SendToCurrentClient((Server_t*) _arg1, (void*) string, strlen(string) + 1);
	return NULL;
}

void* OnDeleteFunc(void* _arg1, void* _arg2)
{
	printf("On delete\n");
	return NULL;
}


/*
int main() 
{	
	Server_t* server;
	struct sigaction sAction;
	ServerArguments args = {OnCreateFunc, (CallbackFunc_t) PrintFunc, OnDeleteFunc, "server_config"};

	ZlogInit("log_config");
	
	sAction.sa_sigaction = (void(*)(int, siginfo_t*, void*))sHandler;
	sAction.sa_flags = SA_SIGINFO;
	
	sigaction(SIGINT, &sAction, NULL);
	
	server = ServerCreate(args);
	ServerRun(server);
	ServerDestroy(server);

	printf("asjksdfsdfsdf");

	return 0;
}*/



















