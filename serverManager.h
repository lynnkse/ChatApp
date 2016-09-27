#ifndef __SERVERMANAGER_H__
#define __SERVERMANAGER_H__


#include "internal.h"

typedef struct ServerManager_t ServerManager_t;
typedef struct ServerData ServerData;

ServerManager_t* CreateServerManager(ServerData _ data);
void DestroyServerManager(ServerManager_t* _serverManager);

ChatRes RergisterNewUser(Details_t* _details); 
ChatRes DeleteUser(Details_t* _details); 
ChatRes LoginExistingUser(Details_t* _details); 
ChatRes LogoutExistingUser(Details_t* _details); 
ChatRes CreateNewGroup(Details_t* _details); 
ChatRes DeleteGroup(Details_t* _details); 
ChatRes JoinGroup(Details_t* _details); 
ChatRes BanUser(Details_t* _details); 
ChatRes UnBanUser(Details_t* _details); 
ChatRes BanFromGroup(Details_t* _details); 
ChatRes UnBanFromGroup(Details_t* _details); 

#endif /*__SERVERMANAGER_H__*/
