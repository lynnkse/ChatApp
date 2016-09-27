#ifndef __CLIENTMANAGER_H__
#define __CLIENTMANAGER_H__

#define SUCCESS 0
#define FAILURE 1

typedef int ClientManagerResult;
typedef struct ClientManager_t ClientManager_t;

ClientManager_t* ClientManagerCreate();
void ClientManagerDestroy(ClientManager_t* _clientManager);



#endif /*__CLIENTMANAGER_H__*/
