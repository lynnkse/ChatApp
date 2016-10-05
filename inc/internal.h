#ifndef __INTERNAL_H__
#define __INTERNAL_H__

#define SUCCESS 0
#define FAILURE 1
#define ERROR -1

typedef int ChatRes;
typedef enum UserChoise {STARTUP = 0, REGISTER, LOGIN, LOGOUT, DELETE_USER, LOGIN_SUCCESS, LOGIN_FAIL, REGISTER_SUCCESS, REGISTER_FAIL, LOGOUT_SUCCESS, DELETE_SUCCESS} UserChoise;
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

typedef struct UserInterface
{
	char m_username[128];
	char m_password[128];
	UserChoise m_choice;
	ChatRes m_result;
}UserInterface;

#endif /*__INTERNAL_H__*/
