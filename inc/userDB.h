#ifndef __USERDB_H__
#define __USERDB_H__

#include "../inc/internal.h"
#include <stddef.h>

typedef struct UserDB_t UserDB_t;
typedef struct User_t User_t;

UserDB_t* CreateUserDB(size_t _cap);
void DestroyUserDB(UserDB_t* _userDB); 

ChatRes DB_RergisterNewUser(UserDB_t* _userDB, UserInterface* _ui); 
ChatRes DB_DeleteUser(UserDB_t* _userDB, UserInterface* _ui); 
ChatRes DB_BanUser(UserDB_t* _userDB, UserInterface* _ui); 
ChatRes DB_UnBanUser(UserDB_t* _userDB, UserInterface* _ui); 
ChatRes DB_LookUpUser(UserDB_t* _userDB, UserInterface* _ui);

#endif
