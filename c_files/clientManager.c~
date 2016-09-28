#include "../inc/internal.h"
#include "../inc/clientManager.h"
#include "../inc/client.h"
#include "/home/student/c/logger/logmngr.h"
#include <errno.h>
#include <stdlib.h>
#include <stdio.h>

/*#define NDEBUG*/
#define STDIN_FD 0
#define LOG_CONFIG_FILE "log_config"

#define CONFIG_FILE config.txt

/*-------Static Funcs Declarations-----*/

static ConfigStruct* ReadConfigFile();

/*-----------------MAIN----------------*/

int main()
{
	ConfigStruct* configStruct = NULL;
	ClientManager_t* clientManager;
	int usedFileDesc;
	Zlog* traceZlog;
	Zlog* errorZlog;
	
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
	}
	
	ZLOG_SEND(errorZlog, LOG_ERROR, "\n\n%d",1);
	ZLOG_SEND(traceZlog, LOG_TRACE, "\n\n%d",1);
	LogManagerDestroy();
	
	return 0;
}

/*-----------Static functions----------*/

ConfigStruct* ReadConfigFile()
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
	configStruct->m_port = 8888;
	/*---------*/
	
	return configStruct;
}

























