#ifndef __CLIENTMANAGER_H__
#define __CLIENTMANAGER_H__


#include "internal.h"
#include "protocol.h"
#include <stddef.h>

/*typedef int ClientResult;
typedef struct Client_t Client_t;*/

/*void ProcessStructAndSendPacket();*/
SocketDesc InitializeConnectionWithTCPserver(ConfigStruct* _configStruct);
int GoToSelectFunc(ClientManager_t* _clientManager);

ChatRes SendMessage(int _fd, void* _payload, size_t _payloadSize);
ChatRes ReceiveMessage(int _fd, void* _payload, size_t _payloadSize);

#endif /*__CLIENTMANAGER_H__*/
