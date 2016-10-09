#include "../inc/userInterface.h"
#include "../inc/internal.h"
#include "../inc/clientManager.h"
#include "../inc/client.h"
#include "../inc/logmngr.h"
#include "../inc/read_config.h"
#include <errno.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>

#define STDIN_FD 0
#define LOG_CONFIG_FILE "log_config"
#define MAX_COMMAND_LEN 128

#define CONFIG_FILE "client_config"
#define PORT_USED 2222
#define DATA_BUFF_SIZE 512

#define CONFIG_FILE "client_config"

struct ClientManager_t
{
	SocketDesc m_socketDesc;
	int m_userInputFD;
};

ClientManager_t* clientManager;

/*-------Static Funcs Declarations-----*/

static ConfigStruct* ReadConfigFile();
static void StartChat(UserInterface* _ui);
static void sHandler(int _sigNum, siginfo_t* _sigInfo, char* _sigContext);

/*-----API functions definitions-------*/
void CreateClientManager()
{
	ConfigStruct* configStruct = NULL;
	
	clientManager = (ClientManager_t*) malloc(sizeof(ClientManager_t));
	if(!clientManager)
	{
		return;
	}
	
	configStruct = ReadConfigFile();
	if(!configStruct)
	{
		free(clientManager);
		return;
	}
	
	if((clientManager->m_socketDesc = InitializeConnectionWithTCPserver(configStruct)) == ERROR)
	{
		free(clientManager);
		free(configStruct);
		return;
	}
	
	clientManager->m_userInputFD = STDIN_FD;
	free(configStruct);
}

void OperateClient()
{
	UserInterface userInterface;
	void* dataBuffer;

	ZLOGS_INITIALIZATION;

	dataBuffer = malloc(DATA_BUFF_SIZE);

	CreateClientManager();
	if(!clientManager)
	{
		ZLOG_SEND(errorZlog, LOG_ERROR, "Couldn't create client manager, %d", 1);
		return;
	}
	
	/*get greeting message from server*/
	ReceiveMessage(clientManager->m_socketDesc, (void*) dataBuffer, sizeof(userInterface));
	printf("%s\n", (char*) dataBuffer);

	/*set user interface in initial state*/
	userInterface.m_choice = STARTUP;
	
	while(RunUserInterface(&userInterface))
	{}
	
	while(1)
	{		
		SendMessage(clientManager->m_socketDesc, &userInterface, sizeof(userInterface));
		ReceiveMessage(clientManager->m_socketDesc, (void*) &userInterface, sizeof(userInterface));

		if(userInterface.m_choice == START_CHAT_SUCCESS)
		{
			StartChat(&userInterface);
		}

		do
		{}
		while(RunUserInterface(&userInterface));
	}
	
	free(dataBuffer);
}


/*-----------------MAIN----------------*/

int main()
{		
	ZlogInit(LOG_CONFIG_FILE);	
	OperateClient();
	
	return 0;
}

/*-----------Static functions----------*/
static ConfigStruct* ReadConfigFile()
{
	ConfigStruct* configStruct;
	Config* configs;
	HashMap* configMap;
	char* sBuffSize;
	char* IP;
	char* sPort;
	
	configStruct = (ConfigStruct*) malloc(sizeof(ConfigStruct));
	if(NULL == configStruct)
	{
		return NULL;
	}

	configs = ReadConfig(CONFIG_FILE);
	configMap = GetNextConfig(configs);	
	HashMap_Remove(configMap, "BuffSize", (void**) &sBuffSize);
	HashMap_Remove(configMap, "IP", (void**) &IP);
	HashMap_Remove(configMap, "Port", (void**) &sPort);

	
	configStruct->m_IPaddress = IP;
	configStruct->m_port = atoi(sPort);
	configStruct->m_buffSize = (size_t) atoi(sBuffSize);

	
	return configStruct;
}

static void StartChat(UserInterface* _ui)
{
	char command[MAX_COMMAND_LEN];
	int res;
	
	res = fork();
	if(res == 0)
	{
		sprintf(command, "gnome-terminal -x sh -c \"./msgReceiver %s %d; bash\"", _ui->m_IP, PORT_USED);
		system(command);
	}
	else
	{
		res = fork();
		if(res == 0)
		{
			sprintf(command, "gnome-terminal -x sh -c \"./msgSender %s %d %s; bash\"", _ui->m_IP, PORT_USED, _ui->m_username);
			system(command);
		}
	}
}


























