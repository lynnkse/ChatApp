#include "../inc/userInterface.h"
#include "../inc/internal.h"
#include "../inc/clientManager.h"
#include "../inc/client.h"
#include "../inc/logmngr.h"
#include <errno.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define STDIN_FD 0
#define LOG_CONFIG_FILE "log_config"
#define MAX_COMMAND_LEN 128

#define CONFIG_FILE "client_config"
#define PORT_USED 2222
#define DATA_BUFF_SIZE 512

struct ClientManager_t
{
	SocketDesc m_socketDesc;
	int m_userInputFD;
};

int g_isAlive = 1;

/*-------Static Funcs Declarations-----*/

static ConfigStruct* ReadConfigFile();
static void StartChat(UserInterface* _ui);

/*-----API functions definitions-------*/
ClientManager_t* CreateClientManager()
{
	ClientManager_t* clientManager;
	ConfigStruct* configStruct = NULL;
	
	clientManager = (ClientManager_t*) malloc(sizeof(ClientManager_t));
	if(!clientManager)
	{
		return NULL;
	}
	
	configStruct = ReadConfigFile();
	if(!configStruct)
	{
		free(clientManager);
		return NULL;
	}
	
	if((clientManager->m_socketDesc = InitializeConnectionWithTCPserver(configStruct)) == ERROR)
	{
		free(clientManager);
		free(configStruct);
		return NULL;
	}
	
	clientManager->m_userInputFD = STDIN_FD;
	free(configStruct);
	
	return clientManager;
}

void OperateClient()
{
	
	ClientManager_t* clientManager;
	UserInterface userInterface;
	void* dataBuffer;

	ZLOGS_INITIALIZATION;

	dataBuffer = malloc(DATA_BUFF_SIZE);

	clientManager = CreateClientManager();
	if(!clientManager)
	{
		ZLOG_SEND(errorZlog, LOG_ERROR, "Couldn't create client manager, %d", 1);
		return;
	}
	
	/*get greeting message from server*/
	ReceiveMessage(clientManager->m_socketDesc, (void*) dataBuffer, sizeof(dataBuffer));
	
	/*set user interface in initial state*/
	userInterface.m_choice = STARTUP;
	
	while(RunUserInterface(&userInterface))
	{}
	printf("91\n");
	
	while(g_isAlive)
	{		
		SendMessage(clientManager->m_socketDesc, &userInterface, sizeof(userInterface));
		ReceiveMessage(clientManager->m_socketDesc, (void*) &userInterface, sizeof(userInterface));
		
		if(userInterface.m_choice == START_CHAT_SUCCESS)
		{
			StartChat(&userInterface);
		}
		printf("102\n");
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
	
	configStruct = (ConfigStruct*) malloc(sizeof(ConfigStruct));
	if(NULL == configStruct)
	{
		return NULL;
	}
	
	/*hardcoded*/
	configStruct->m_IPaddress = "127.0.0.1";
	configStruct->m_port = 1351;
	/*---------*/
	
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

























