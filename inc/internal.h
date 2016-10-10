#ifndef __INTERNAL_H__
#define __INTERNAL_H__

#include <stddef.h>

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

typedef struct ConfigStruct
{
	char* m_IPaddress;
	int m_port;
	size_t m_buffSize;
	char m_superuser[MAX_USERNAME_LENGTH];
	char m_SUpassword[MAX_USERNAME_LENGTH];
} ConfigStruct; 

/*1 - 9 are options to be selected by user, the rest are types of responce received from server, on which UI will react*/
typedef enum UserChoice {STARTUP = 0, REGISTER, LOGIN, LOGOUT, DELETE_USER, CREATE_GROUP, DELETE_GROUP, JOIN_GROUP, LEAVE_GROUP, START_CHAT, SAVE_DB, PRINT_OUT_GROUPS, PRINT_OUT_USERS, LOGIN_SUCCESS, LOGIN_FAIL, REGISTER_SUCCESS, REGISTER_FAIL, LOGOUT_SUCCESS, DELETE_SUCCESS, GROUP_CREATE_SUCCESS, GROUP_CREATE_FAILURE, GROUP_JOIN_SUCCESS, GROUP_JOIN_FAILURE, GROUP_DELETE_SUCCESS, GROUP_DELETE_FAILURE, GROUP_LEAVE_SUCCESS, GROUP_LEAVE_FAILURE, START_CHAT_SUCCESS, START_CHAT_FAILURE, SAVE_DB_SUCCESS, SAVE_DB_FAILURE, PRINT_OUT_GROUPS_SUCCESS, PRINT_OUT_USERS_SUCCESS} UserChoice;

typedef enum SuperUserChoice {SU_STARTUP = 0, BAN_USER, SU_LOGOUT, BAN_USER_SUCCESS, BAN_USER_FAILURE, SU_LOGOUT_SUCCESS} SuperUserChoice;

typedef int SocketDesc;

typedef struct UserInterface
{
	char m_username[MAX_USERNAME_LENGTH];
	char m_password[MAX_PASSWORD_LENGTH];
	char m_groupname[MAX_GROUPNAME_LENGTH];
	UserChoice m_choice;
	SuperUserChoice m_SUchoice;
	char m_SUdata[MAX_USERNAME_LENGTH];
	char m_IP[16];
	ChatRes m_result;
}UserInterface;

#endif /*__INTERNAL_H__*/
