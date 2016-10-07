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
#include <netinet/ip.h>
#include <errno.h>

#define LOG_CONFIG_FILE "log_config"
#define PORT 2222
#define TTL 1
#define MAX_LEN 128

int main(int argc, char* argv[]) /*arguments: IP, username*/
{
	char* multicastIP;
	char* username;
	int port;
	int soc;
	int res;
	struct sockaddr_in addr; 
	u_char ttl = TTL;
	char msg[128];
	Zlog* traceZlog;
	Zlog* errorZlog;
	
	ZlogInit(LOG_CONFIG_FILE);
	traceZlog = ZlogGet("trace");
	errorZlog = ZlogGet("error");
	
	soc = socket(PF_INET, SOCK_DGRAM, IPPROTO_UDP);
	if(soc == ERROR)
	{
		ZLOG_SEND(errorZlog, LOG_ERROR, "Couldn't create socket, %d",1);
		exit(ERROR);
	}
	printf("Socket: %d\n", soc);
	
	multicastIP = argv[1];
	port = atoi(argv[2]);
	username = argv[3];
	
	res = setsockopt(soc, IPPROTO_IP, IP_MULTICAST_TTL, (void*)&ttl, sizeof(ttl));
	if(res == ERROR)
	{
		ZLOG_SEND(errorZlog, LOG_ERROR, "Setsockopt() failed, %d",1);
		perror("setsockopt failed");
		exit(ERROR);
	}
	
	memset(&addr, 0, sizeof(addr));
	addr.sin_family = AF_INET;
	addr.sin_addr.s_addr = inet_addr(multicastIP);
	addr.sin_port = htons(port);
	
	while(1)
	{
		fgets(msg, MAX_LEN, stdin);
		/*snprintf(msgToSend, MAX_LEN+NAME_LENGTH, "%s|%s", name, getSendString);
		sendLen = strlen(getSendString);
		/*scanf("%s", getSendString);*/
	
		res = sendto(soc, msg, strlen(msg) + 1, 0, (struct sockaddr *) &addr, sizeof(addr));
	}
	
	return 0;
}














































