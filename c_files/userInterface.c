#include "../inc/userInterface.h"
#include <stdio.h>

/*---Static functions declarations---*/
/*static void WelcomeScreen();*/

/*----API functions defenitions------*/
void RunUserInterface(UserInterface* _userInterface)
{	
	switch(_userInterface->m_choice)
	{
		case STARTUP:
			/*printf("Welcome!\n");*/
			printf("1. Register\n2. Login\n");
			scanf("%u", (unsigned int*)&_userInterface->m_choice);
			break;
		case REGISTER:
			printf("Enter username:\n");
			scanf("%s", (char*)&_userInterface->m_username);
			printf("Enter password:\n");
			scanf("%s", (char*)&_userInterface->m_password);
			break;
		case LOGIN:
			printf("Enter username:\n");
			scanf("%s", (char*)&_userInterface->m_username);
			printf("Enter password:\n");
			scanf("%s", (char*)&_userInterface->m_password);
			break;
		case LOGOUT:
			_userInterface->m_choice = LOGOUT;
			break;
		case DELETE_USER:
			_userInterface->m_choice = DELETE_USER;
			break;
		case CREATE_GROUP:
			printf("Enter group name\n");
			scanf("%s", (char*)_userInterface->m_groupname);
			printf("%s\n", _userInterface->m_groupname);
			break;
		case DELETE_GROUP:
			printf("Enter group name\n");
			scanf("%s", (char*)_userInterface->m_groupname);
			printf("%s\n", _userInterface->m_groupname);
			break;
		case JOIN_GROUP:
			printf("Enter group name\n");
			scanf("%s", (char*)_userInterface->m_groupname);
			break;
		case LEAVE_GROUP:
			printf("Enter group name\n");
			scanf("%s", (char*)_userInterface->m_groupname);
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
			printf("3. Logout\n4. Delete user\n5. Create new group\n6. Delete group\n7. Join group\n8. Leave group\n");
			scanf("%u", (unsigned int*)&_userInterface->m_choice);
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
			printf("Group created successfully!\n");
			printf("3. Logout\n4. Delete user\n5. Create new group\n6. Delete group\n7. Join group\n8. Leave group\n");
			scanf("%u", (unsigned int*)&_userInterface->m_choice);
			break;
		case GROUP_CREATE_FAILURE:
			printf("Failed to create group!\n");
			printf("3. Logout\n4. Delete user\n5. Create new group\n6. Delete group\n7. Join group\n8. Leave group\n");
			scanf("%u", (unsigned int*)&_userInterface->m_choice);
			break;
		case GROUP_JOIN_SUCCESS:
			printf("Joined group successfully!\n");
			printf("3. Logout\n4. Delete user\n5. Create new group\n6. Delete group\n7. Join group\n8. Leave group\n");
			scanf("%u", (unsigned int*)&_userInterface->m_choice);
			break;
		case GROUP_JOIN_FAILURE:
			printf("Failed to join group!\n");
			printf("3. Logout\n4. Delete user\n5. Create new group\n6. Delete group\n7. Join group\n8. Leave group\n");
			scanf("%u", (unsigned int*)&_userInterface->m_choice);
			break;
		case GROUP_LEAVE_SUCCESS:
			printf("Group left successfully!\n");
			printf("3. Logout\n4. Delete user\n5. Create new group\n6. Delete group\n7. Join group\n8. Leave group\n");
			scanf("%u", (unsigned int*)&_userInterface->m_choice);
			break;
		case GROUP_LEAVE_FAILURE:
			printf("Failed to leave group!\n");
			printf("3. Logout\n4. Delete user\n5. Create new group\n6. Delete group\n7. Join group\n8. Leave group\n");
			scanf("%u", (unsigned int*)&_userInterface->m_choice);
			break;
		case GROUP_DELETE_SUCCESS:
			printf("Group deleted successfully!\n");
			printf("3. Logout\n4. Delete user\n5. Create new group\n6. Delete group\n7. Join group\n8. Leave group\n");
			scanf("%u", (unsigned int*)&_userInterface->m_choice);
			break;
		case GROUP_DELETE_FAILURE:
			printf("Failed to delete group!\n");
			printf("3. Logout\n4. Delete user\n5. Create new group\n6. Delete group\n7. Join group\n8. Leave group\n");
			scanf("%u", (unsigned int*)&_userInterface->m_choice);
			break;
	}
}

/*---Static functions defenition---*/
/*static int WelcomeScreen()
{
	UserChoise userChoise = 0;

	while(userChoise != REGISTER && userChoise != LOGIN)
	{
		printf("Welcome!\n");
		printf("1. Register\n2. Login");
		scanf("%d", &userChoise);

		if(userChoise != REGISTER && userChoise != LOGIN)
		{
			printf("Incorrect choise. Try again...\n");
		}
	}
	
	return userChoise;
}*/


