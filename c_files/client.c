#include "../inc/client.h"
#include <sys/socket.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>
#include <sys/select.h>
#include "/home/student/c/logger/logmngr.h"


#define ERROR -1

SocketDesc InitializeConnectionWithTCPserver(ConfigStruct* _configStruct)
{
	struct sockaddr_in serverAddr;
	SocketDesc socketDesc;
	Zlog* traceZlog;
	Zlog* errorZlog;
	
	traceZlog = ZlogGet("trace");
	errorZlog = ZlogGet("error");
	
	printf("InitializeConnectionWithTCPserver\n");
	
	if((socketDesc = socket(AF_INET, SOCK_STREAM, 0)) == -1)
	{
		ZLOG_SEND(errorZlog, LOG_ERROR, "Couldn't open socket, %d",1);
		#ifndef NDEBUG
			perror("Couldn't open socket\n");
		#endif
		return -1;
	}
	ZLOG_SEND(traceZlog, LOG_TRACE, "Socket obtained: %d",socketDesc);
	
	serverAddr.sin_family = AF_INET;
	serverAddr.sin_port = htons(_configStruct->m_port);
	serverAddr.sin_addr.s_addr = inet_addr(_configStruct->m_IPaddress);
	
	memset(serverAddr.sin_zero, '\0', sizeof(serverAddr.sin_zero));
	
	
	if(connect(socketDesc, (struct sockaddr *) &serverAddr, (socklen_t) sizeof(serverAddr)) == -1)
	{
		ZLOG_SEND(errorZlog, LOG_ERROR, "Couldn't establish connection with the server, %d",1);
		#ifndef NDEBUG
			perror("Couldn't establish connection with the server");
		#endif
		return -1;
	}
	
	return socketDesc;
}

int GoToSelectFunc(ClientManager_t* _clientManager)
{
	/*Zlog* traceZlog;*/
	Zlog* errorZlog;
	fd_set rfds;
	
	/*traceZlog = ZlogGet("trace");*/
	errorZlog = ZlogGet("error");
	
	FD_ZERO(&rfds);
	FD_SET(_clientManager->m_userInputFD, &rfds);	
	FD_SET(_clientManager->m_socketDesc, &rfds);
	
	if((select(FD_SETSIZE, &rfds, NULL, NULL, NULL) == ERROR) && (errno != EINTR))
	{
		ZLOG_SEND(errorZlog, LOG_ERROR, "Select() failed, %d",1);
		#ifndef NDEBUG
			perror("Select failed\n");
		#endif
		return ERROR;
	}
	
	return (FD_ISSET(_clientManager->m_userInputFD, &rfds) == 1 ? _clientManager->m_userInputFD : _clientManager->m_socketDesc);
}




































