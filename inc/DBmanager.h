#ifndef __DBMANAGER_H__
#define __DBMANAGER_H__

#include "../inc/internal.h"


/*---API functions defenitions---*/
ChatRes RergisterNewUser(UserInterface* _ui); 
ChatRes DeleteUser(UserInterface* _ui); 
ChatRes BanUser(UserInterface* _ui); 
ChatRes UnBanUser(UserInterface* _ui); 
ChatRes LookUpUser(UserInterface* _ui);

#endif
