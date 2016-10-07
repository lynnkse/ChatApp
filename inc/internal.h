#ifndef __INTERNAL_H__
#define __INTERNAL_H__

#define SUCCESS 0
#define FAILURE 1
#define ERROR -1

#define MAX_GROUPNAME_LENGTH 32
#define MAX_USERNAME_LENGTH 32
#define MAX_PASSWORD_LENGTH 32
#define IP_ADDR_LEN 16

#define ZLOGS_INITIALIZATION 	Zlog* traceZlog;\
								Zlog* errorZlog;\
								traceZlog = ZlogGet("trace");\
								errorZlog = ZlogGet("error");\

typedef int ChatRes;
typedef enum UserChoise {STARTUP = 0, REGISTER, LOGIN, LOGOUT, DELETE_USER, CREATE_GROUP, DELETE_GROUP, JOIN_GROUP, LEAVE_GROUP, START_CHAT, LOGIN_SUCCESS, LOGIN_FAIL, REGISTER_SUCCESS, REGISTER_FAIL, LOGOUT_SUCCESS, DELETE_SUCCESS, GROUP_CREATE_SUCCESS, GROUP_CREATE_FAILURE, GROUP_JOIN_SUCCESS, GROUP_JOIN_FAILURE, GROUP_DELETE_SUCCESS, GROUP_DELETE_FAILURE, GROUP_LEAVE_SUCCESS, GROUP_LEAVE_FAILURE, START_CHAT_SUCCESS, START_CHAT_FAILURE} UserChoise;

typedef int SocketDesc;

typedef struct ConfigStruct
{
	char* m_IPaddress;
	int m_port;
} ConfigStruct; 

typedef struct UserInterface
{
	char m_username[MAX_USERNAME_LENGTH];
	char m_password[MAX_PASSWORD_LENGTH];
	char m_groupname[MAX_GROUPNAME_LENGTH];
	UserChoise m_choice;
	char m_IP[16];
	ChatRes m_result;
}UserInterface;

#endif /*__INTERNAL_H__*/
