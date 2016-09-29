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
			printf("Welcome!\n");
			printf("1. Register\n2. Login\n");
			scanf("%u", &_userInterface->m_choice);
			break;
			/*TODO check for correct input*/
		case REGISTER:
			printf("Enter username:\n");
			scanf("%s", (char*)&_userInterface->m_userName);
			printf("Enter password:\n");
			scanf("%s", (char*)&_userInterface->m_password);
			break;
		case LOGIN:
			printf("Enter username:\n");
			scanf("%s", (char*)&_userInterface->m_userName);
			printf("Enter password:\n");
			scanf("%s", (char*)&_userInterface->m_password);
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


