#ifndef __DBMANAGER_H__
#define __DBMANAGER_H__

#include "../inc/internal.h"
#include "../inc/userDB.h"

typedef struct DBmanager_t DBmanager_t;

DBmanager_t* CreateDBManager(size_t _cap);
void DestroyDBmanager(DBmanager_t* _DBmanager);
ChatRes DBman_RergisterNewUser(DBmanager_t* _DBmanager, UserInterface* _ui); 
ChatRes DBman_DeleteUser(DBmanager_t* _DBmanager, UserInterface* _ui); 
ChatRes DBman_BanUser(DBmanager_t* _DBmanager, UserInterface* _ui); 
ChatRes DBman_UnBanUser(DBmanager_t* _DBmanager, UserInterface* _ui); 
ChatRes DBman_LookUpUser(DBmanager_t* _DBmanager, UserInterface* _ui);
ChatRes DBman_LogoutUser(DBmanager_t* _DBmanager, UserInterface* _ui);

#endif
