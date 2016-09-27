#ifndef __PROTOCOL_H__
#define __PROTOCOL_H__

typedef struct Packet_t Packet_t;
typedef enum Action {SOME_ACTION} Action;

typedef struct Details_t Details_t;/*user name, group name, opCode, result*/

Packet_t* CreatePacket(Details_t _details); 
Details_t* ReadPacket(Packet_t* _packet);

#endif /*__PROTOCOL_H__*/
