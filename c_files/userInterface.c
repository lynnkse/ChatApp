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
			printf("3. Logout\n4. Delete user\n");
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


