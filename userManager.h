#ifndef __USERMANAGER_H__
#define __USERMANAGER_H__

#include "internal.h"

typedef struct User_t User_t;
typedef struct UserDetails UserDetails;
typedef int UserResult;

User_t* CreateUser(UserDetails _userDetails);
void DestroyUser(User_t* _user);

UserResult RergisterNewUser(Details_t* _details); 
UserResult DeleteUser(Details_t* _details); 
UserResult LoginExistingUser(Details_t* _details); 
UserResult LogoutExistingUser(Details_t* _details); 
UserResult CreateNewGroup(Details_t* _details); 
UserResult DeleteGroup(Details_t* _details); 
UserResult JoinGroup(Details_t* _details); 
UserResult BanUser(Details_t* _details); 
UserResult UnBanUser(Details_t* _details); 
UserResult BanFromGroup(Details_t* _details); 
UserResult UnBanFromGroup(Details_t* _details); 

UserResult SendMessage(Details_t* _details, void* _payload); /*length of payload is specified in _details*/
UserResult RecieveMessage(Details_t* _details, void** _payload);

#endif /*__USER_H__*/
