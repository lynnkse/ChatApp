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

/*#define NDEBUG*/
#define STDIN_FD 0
#define LOG_CONFIG_FILE "log_config"

#define CONFIG_FILE config.txt

/*-------Static Funcs Declarations-----*/

static ConfigStruct* ReadConfigFile();

/*-----API functions definitions-------*/



/*-----------------MAIN----------------*/

int main()
{
	ConfigStruct* configStruct = NULL;
	ClientManager_t* clientManager;
	int usedFileDesc;
	Zlog* traceZlog;
	Zlog* errorZlog;
	char dataBuffer[1024];/*FIXME change this*/
	UserInterface userInterface;
	
	ZlogInit(LOG_CONFIG_FILE);
	
	traceZlog = ZlogGet("trace");
	errorZlog = ZlogGet("error");

	clientManager = (ClientManager_t*) malloc(sizeof(ClientManager_t));
	if(NULL == clientManager)
	{
		ZLOG_SEND(errorZlog, LOG_ERROR, "Malloc for client manager failed, %d",1);
		#ifndef NDEBUG
			perror("Malloc for client manager failed\n");
		#endif
		return ERROR;
	}
	
	configStruct = ReadConfigFile();
	
	printf("%p\n", (void*)configStruct);/*TODO delete this*/
	
	if((clientManager->m_socketDesc = InitializeConnectionWithTCPserver(configStruct)) == ERROR)
	{
		ZLOG_SEND(errorZlog, LOG_ERROR, "Connection with TCP server failed, %d",1);
		#ifndef NDEBUG
			perror("Connection with TCP server failed\n");
		#endif
		free(clientManager);
		return ERROR;
	}
	
	clientManager->m_userInputFD = STDIN_FD;
	
	userInterface.m_choice = STARTUP;
	RunUserInterface(&userInterface);
	
	while(1)/*TODO change this*/
	{
		usedFileDesc = GoToSelectFunc(clientManager);
		if(ERROR == usedFileDesc)
		{
			free(clientManager);
			ZLOG_SEND(errorZlog, LOG_ERROR, "GoToSelectFunc failed, %d",1);
			#ifndef NDEBUG
				perror("GoToSelectFunc failed\n");
			#endif
			return ERROR;
		}
		
		memset(dataBuffer, 0, sizeof(dataBuffer));
		ReceiveMessage(usedFileDesc, (void*) dataBuffer, sizeof(dataBuffer));
		
		if(usedFileDesc == clientManager->m_userInputFD)
		{
			userInterface.m_choice = atoi(dataBuffer);
			RunUserInterface(&userInterface);
			SendMessage(clientManager->m_socketDesc, &userInterface, sizeof(userInterface));
			ReceiveMessage(clientManager->m_socketDesc, (void*) dataBuffer, sizeof(dataBuffer));
			printf("Result from server: %d\n", ((UserInterface*)dataBuffer)->m_result);
		}
		else
		{
			printf("Result from server: %d\n", ((UserInterface*)dataBuffer)->m_result);	
		}
			
		/*!!!FOR TESTING ONLY!!!*/
		
		/*strcpy(dataBuffer, "some string to be sent");
		SendMessage(usedFileDesc, dataBuffer, strlen(dataBuffer) + 1);
		break;*/
		/*------------------*/
	}
	
	ZLOG_SEND(errorZlog, LOG_ERROR, "\n\n%d",1);
	ZLOG_SEND(traceZlog, LOG_TRACE, "\n\n%d",1);
	
	return 0;
}

/*-----------Static functions----------*/

static ConfigStruct* ReadConfigFile()
{
	ConfigStruct* configStruct;
	
	configStruct = (ConfigStruct*) malloc(sizeof(ConfigStruct));
	if(NULL == configStruct)
	{
		/*TODO logger*/
		#ifndef NDEBUG
			perror("Malloc failed\n");
		#endif
		return NULL;
	}
	
	/*hardcoded*/
	configStruct->m_IPaddress = "127.0.0.1";
	configStruct->m_port = 1341;
	/*---------*/
	
	return configStruct;
}

























