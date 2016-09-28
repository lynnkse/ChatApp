#ifndef __INTERNAL_H__
#define __INTERNAL_H__

#define SUCCESS 0
#define FAILURE 1
#define ERROR -1

typedef int ChatRes;
typedef enum Action {SOME_ACTION} Action;
typedef int SocketDesc;

typedef struct ConfigStruct
{
	char* m_IPaddress;
	int m_port;
} ConfigStruct; 

typedef struct ClientManager_t
{
	SocketDesc m_socketDesc;
	int m_userInputFD;/*this will be STDIN in our case*/
} ClientManager_t;

#endif /*__INTERNAL_H__*/
