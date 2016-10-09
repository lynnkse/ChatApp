#ifndef __GROUP_DB__
#define __GROUP_DB__

#include "../inc/internal.h"
#include <stddef.h>

typedef struct GroupDB_t GroupDB_t;
typedef struct Group_t Group_t;

GroupDB_t* CreateGroupDB(size_t _groupsCap);
void DestroyGroupDB(GroupDB_t* _groupDB);

ChatRes GroupDB_CreateNewGroup(GroupDB_t* _groupDB, UserInterface* _ui); 
ChatRes GroupDB_DeleteGroup(GroupDB_t* _groupDB, UserInterface* _ui); 
ChatRes GroupDB_UserJoinGroup(GroupDB_t* _groupDB, UserInterface* _ui); 
ChatRes GroupDB_UserLeaveGroup(GroupDB_t* _groupDB, UserInterface* _ui); 
ChatRes GroupDB_StartChat(GroupDB_t* _groupDB, UserInterface* _ui); 
ChatRes GroupDB_Save(GroupDB_t* _groupDB, UserInterface* _ui); 
ChatRes GroupDB_PrintOutGroups(GroupDB_t* _groupDB, UserInterface* _ui); 

#endif
