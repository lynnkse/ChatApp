#ifndef __SERVERMANAGER_H__
#define __SERVERMANAGER_H__


#include "../inc/internal.h"
#include <stddef.h>

typedef struct ServerManager_t ServerManager_t;

ServerManager_t* CreateServerManager(size_t _cap, size_t _groupsCap);
void DestroyServerManager(ServerManager_t* _serverManager);

/*typedef struct ServerData ServerData;*/

ChatRes OperateServer();


#endif /*__SERVERMANAGER_H__*/
