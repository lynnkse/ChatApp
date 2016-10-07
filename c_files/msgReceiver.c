#include <sys/types.h>  
#include <sys/socket.h> 
#include <arpa/inet.h>  
#include <stdio.h>      
#include <stdlib.h>     
#include <string.h>     
#include <unistd.h> 
#include "../inc/internal.h"
#include "../inc/logmngr.h"
#include <netinet/in.h>
#include <fcntl.h>



#define LOG_CONFIG_FILE "log_config"
#define PORT 2222
#define MSG_LEN 1024

int main(int argc, char* argv[])/*argument: IP*/
{
	char* username;
	char* multicastIP;
	int port;
	int soc;
	int optVal = 1;
	int res;
	struct sockaddr_in addr; 
	struct ip_mreq mreq;
	char msg[MSG_LEN];
	int addrLen;
	struct sockaddr_in fromAddr;
	Zlog* traceZlog;
	Zlog* errorZlog;
	
	ZlogInit(LOG_CONFIG_FILE);
	traceZlog = ZlogGet("trace");
	errorZlog = ZlogGet("error");
	
	multicastIP = argv[1];
	port = atoi(argv[2]);
	
	soc = socket(PF_INET, SOCK_DGRAM, IPPROTO_UDP);
	if(soc == ERROR)
	{
		ZLOG_SEND(errorZlog, LOG_ERROR, "Couldn't create socket, %d",1);
		exit(ERROR);
	}
	printf("Socket: %d\n", soc);
	
	res = setsockopt(soc, SOL_SOCKET, SO_REUSEADDR, &optVal, (socklen_t) sizeof(optVal));
	if(res == ERROR)
	{
		ZLOG_SEND(errorZlog, LOG_ERROR, "Setsockopt() failed, %d",1);
		perror("setsockopt() failed");
		exit(ERROR);
	}
	
	memset(&addr, 0, sizeof(addr));
	addr.sin_family      = AF_INET;
	addr.sin_addr.s_addr = htonl(INADDR_ANY);
	addr.sin_port        = htons(port);
	
	res = bind(soc, (struct sockaddr*)&addr, sizeof(addr));
	if(res == ERROR)
	{
		ZLOG_SEND(errorZlog, LOG_ERROR, "Bind() failed, %d",1);
		perror("Bind() failed");
		exit(ERROR);
	}
	
	mreq.imr_multiaddr.s_addr = inet_addr(multicastIP);
	mreq.imr_interface.s_addr = htonl(INADDR_ANY);
	
	printf("IP: %s\n", multicastIP);
	
	res = setsockopt(soc, IPPROTO_IP, IP_ADD_MEMBERSHIP, (void*) &mreq, sizeof(mreq));
	if(res == ERROR)
	{
		ZLOG_SEND(errorZlog, LOG_ERROR, "Setsockopt() failed, %d",1);
		perror("second setsockopt() failed");
		exit(ERROR);
	}

	addrLen = sizeof(fromAddr);	
	memset(&fromAddr, 0, addrLen);
	
	printf("I'm receiver\n");
	
	while(1)
	{
		recvfrom(soc, msg, sizeof(msg), 0, (struct sockaddr*) &fromAddr, &addrLen);
		/*read(soc,  msg,  sizeof(msg));*/
		printf("%s\n", msg);
	}
	
	return 0;
}


































