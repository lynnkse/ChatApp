#ifndef __USERDB_H__
#define __USERDB_H__

#include "../inc/internal.h"

typedef struct UserDB_t UserDB_t;

UserDB_t* CreateUserDB(size_t _cap);
void DestroyUserDB(UserDB_t* _userDB); 

ChatRes RergisterNewUser(UserInterface* _ui); 
ChatRes DeleteUser(UserInterface* _ui); 
ChatRes BanUser(UserInterface* _ui); 
ChatRes UnBanUser(UserInterface* _ui); 
ChatRes LookUpUser(UserInterface* _ui);

#endif
