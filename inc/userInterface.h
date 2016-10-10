#ifndef __USERINTERFACE_H__
#define __USERINTERFACE_H__

#include "../inc/internal.h"

int RunUserInterface(UserInterface* _userInterface);
int RunSuperUserInterface(UserInterface* _userInterface);
void EncryptPassword(char* _pass);


UserInterface userInterface;




#endif /*__USERINTERFACE_H__*/
