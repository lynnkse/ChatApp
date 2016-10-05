#ifndef __INTERNAL_H__
#define __INTERNAL_H__

#define SUCCESS 0
#define FAILURE 1
#define ERROR -1

#define MAX_GROUPNAME_LENGTH 32
#define MAX_USERNAME_LENGTH 32
#define MAX_PASSWORD_LENGTH 32

typedef int ChatRes;
typedef enum UserChoise {STARTUP = 0, REGISTER, LOGIN, LOGOUT, DELETE_USER, CREATE_GROUP, DELETE_GROUP, JOIN_GROUP, LEAVE_GROUP, LOGIN_SUCCESS, LOGIN_FAIL, REGISTER_SUCCESS, REGISTER_FAIL, LOGOUT_SUCCESS, DELETE_SUCCESS, GROUP_CREATE_SUCCESS, GROUP_CREATE_FAILURE, GROUP_JOIN_SUCCESS, GROUP_JOIN_FAILURE, GROUP_DELETE_SUCCESS, GROUP_DELETE_FAILURE, GROUP_LEAVE_SUCCESS, GROUP_LEAVE_FAILURE} UserChoise;
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
	char m_username[MAX_USERNAME_LENGTH];
	char m_password[MAX_PASSWORD_LENGTH];
	char m_groupname[MAX_GROUPNAME_LENGTH];
	UserChoise m_choice;
	ChatRes m_result;
}UserInterface;

#endif /*__INTERNAL_H__*/
