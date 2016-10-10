#include "../inc/userInterface.h"
#include "../inc/internal.h"
#include <stdio.h>
#include <ctype.h>
#include <crypt.h>

static void GetUsername(char* _username);
static void SuperUserMenu(SuperUserChoice* _choice);
static void GetMenuOption(UserChoice* _choice);
static void GetUsernameAndPass(char* _username, char* _pass);
static void GetGroupName(char* _groupname);
static void StartUpMenu(UserChoice* _choice);

/*----API functions defenitions------*/
void EncryptPassword(char* _pass)
{
	int hash = 5381;
	int c;

	while (c = *(_pass++))
	hash = ((hash << 5) + hash) + c; 

	hash *= 1234567;

	sprintf(_pass, "%d", hash);
}

int RunUserInterface(UserInterface* _userInterface)
{	
	int runAgain = (_userInterface->m_choice >= REGISTER && _userInterface->m_choice <= PRINT_OUT_USERS) ? 0 : 1;

	switch(_userInterface->m_choice)
	{
		case STARTUP:
			StartUpMenu(&_userInterface->m_choice);
			break;

		case REGISTER:
		case LOGIN:
			GetUsernameAndPass(_userInterface->m_username, _userInterface->m_password);
			break;

		case LOGOUT:
			_userInterface->m_choice = LOGOUT;
			break;

		case DELETE_USER:
			_userInterface->m_choice = DELETE_USER;
			break;

		case CREATE_GROUP:
		case DELETE_GROUP:
		case JOIN_GROUP:
		case LEAVE_GROUP:
		case START_CHAT:
			GetGroupName(_userInterface->m_groupname);
			break;

		case SAVE_DB:
		case PRINT_OUT_GROUPS:
		case PRINT_OUT_USERS:
			break;

		case REGISTER_SUCCESS:
			printf("Registered successfully!\n");
			_userInterface->m_choice = STARTUP;
			break;
		case REGISTER_FAIL:
			printf("Registered failed!\n");
			_userInterface->m_choice = STARTUP;
			break;
		case LOGIN_SUCCESS:
			printf("Logged in successfully!\n");
			GetMenuOption(&_userInterface->m_choice);
			break;
		case LOGIN_FAIL:
			printf("Login failed!\n");
			_userInterface->m_choice = STARTUP;
			break;
		case LOGOUT_SUCCESS:
			printf("Logged out successfully!\n");
			_userInterface->m_choice = STARTUP;
			break;
		case DELETE_SUCCESS:
			printf("Deleted successfully!\n");
			_userInterface->m_choice = STARTUP;
			break;
		case GROUP_CREATE_SUCCESS:
			printf("Group created successfully! You are now admin of %s\n", userInterface.m_groupname);
			GetMenuOption(&_userInterface->m_choice);
			break;
		case GROUP_CREATE_FAILURE:
			printf("Failed to create group!\n");
			GetMenuOption(&_userInterface->m_choice);
			break;
		case GROUP_JOIN_SUCCESS:
			printf("Joined group successfully!\n");
			GetMenuOption(&_userInterface->m_choice);
			break;
		case GROUP_JOIN_FAILURE:
			printf("Failed to join group!\n");
			GetMenuOption(&_userInterface->m_choice);
			break;
		case GROUP_LEAVE_SUCCESS:
			printf("Group left successfully!\n");
			GetMenuOption(&_userInterface->m_choice);
			break;
		case GROUP_LEAVE_FAILURE:
			printf("Failed to leave group!\n");
			GetMenuOption(&_userInterface->m_choice);
			break;
		case GROUP_DELETE_SUCCESS:
			printf("Group deleted successfully!\n");
			GetMenuOption(&_userInterface->m_choice);
			break;
		case GROUP_DELETE_FAILURE:
			printf("Failed to delete group!\n");
			GetMenuOption(&_userInterface->m_choice);
			break;
		case START_CHAT_SUCCESS:
			printf("Chat started successfully!\n");
			GetMenuOption(&_userInterface->m_choice);
			break;
		case START_CHAT_FAILURE:
			printf("Unable start chat!\n");
			GetMenuOption(&_userInterface->m_choice);
			break;
		case SAVE_DB_SUCCESS:
			printf("Saved successfully!\n");
			GetMenuOption(&_userInterface->m_choice);
			break;
		case SAVE_DB_FAILURE:
			printf("Unable to save!\n");
			GetMenuOption(&_userInterface->m_choice);
			break;
		case PRINT_OUT_GROUPS_SUCCESS:
			printf("Groups:\n");
			GetMenuOption(&_userInterface->m_choice);
			break;
		case PRINT_OUT_USERS_SUCCESS:
			printf("Users:\n");
			GetMenuOption(&_userInterface->m_choice);
			break;
	}
	
	return runAgain;
}

int RunSuperUserInterface(UserInterface* _userInterface)
{
	int ranAgain = (_userInterface->m_SUchoice >= BAN_USER || _userInterface->m_SUchoice <= LOGOUT) ? 0 : 1;
	
	switch(_userInterface->m_SUchoice)
	{
		case SU_STARTUP:
			SuperUserMenu(&_userInterface->m_SUchoice);
			break;
		case BAN_USER:
			GetUsername(_userInterface->m_SUdata);
			break;
		case SU_LOGOUT:
			break;
		case BAN_USER_SUCCESS:
			printf("Banned successfully!\n");
			_userInterface->m_SUchoice = SU_STARTUP;
			break;
		case BAN_USER_FAILURE:
			printf("Cannot ban!\n");
			_userInterface->m_SUchoice = SU_STARTUP;
			break;
		case SU_LOGOUT_SUCCESS:
			_userInterface->m_SUchoice = SU_STARTUP;
			_userInterface->m_choice = STARTUP;
			printf("Logged out\n");
			break;
	}

	return ranAgain;
}

static void GetUsername(char* _username)
{
	printf("Enter username:\n");
	scanf("%s", _username);
}

static void SuperUserMenu(SuperUserChoice* _choice)
{
	unsigned int num;

	printf("1. Ban user\n2. Logout");
	scanf("%u", &num);

	while(num < 1 || num > 2)
	{
		printf("Incorrect choice... Try again\n");
		scanf("%u", &num);
	}
	
	*_choice = num;
}

static void GetMenuOption(UserChoice* _choice)
{
	unsigned int num;

	printf("1. Logout\n2. Delete user\n3. Create new group\n4. Delete group\n5. Join group\n6. Leave group\n7. Start chat\n8. Save DB\n9. Print out all groups\n10. Print out all users\n");
	scanf("%u", &num);
	
	while(num < 1 || num > 10)
	{
		printf("Invalid choice. Try again...\n");
		scanf("%u", &num);
	}
	
	num += 2;

	*_choice = (UserChoice) num;
}

static void GetUsernameAndPass(char* _username, char* _pass)
{
	printf("Enter username:\n");
	scanf("%s", _username);
	printf("Enter password:\n");
	scanf("%s", _pass);
	EncryptPassword(_pass);
}

static void GetGroupName(char* _groupname)
{
	printf("Enter group name\n");
	scanf("%s", _groupname);
}

static void StartUpMenu(UserChoice* _choice)
{
	unsigned int num;

	printf("1. Register\n2. Login\n");
	scanf("%u", &num);
	
	while(num < 1 || num >2)
	{
		printf("Invalid choice... Try again.\n");
		scanf("%u", &num);
	}
	
	*_choice = (UserChoice) num;
}


































