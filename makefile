CFLAGS = -g -c -std=gnu99 #-pedantic -Werror -Wall 
CC = gcc

# OBJ = ./obj/
CLT_OBJS = clientManager.o client.o userInterface.o
SRV_OBJS = server.o serverManager.o DBmanager.o userDB.o groupDB.o

.PHONY: clean



client : $(CLT_OBJS)
	$(CC) -o client $(CLT_OBJS) -L./lib -lds -llogger
	
clientManager.o : c_files/clientManager.c inc/clientManager.h inc/internal.h inc/protocol.h inc/userInterface.h
	$(CC) $(CFLAGS) c_files/clientManager.c 

client.o : c_files/client.c inc/client.h inc/internal.h
	$(CC) $(CFLAGS) c_files/client.c 
	
userInterface.o : c_files/userInterface.c inc/userInterface.h inc/internal.h
	$(CC) $(CFLAGS) c_files/userInterface.c 



server :  $(SRV_OBJS)
	$(CC) -o server $(SRV_OBJS) -L./lib -lds -llogger -lm
	
server.o : c_files/server.c inc/server.h inc/logmngr.h inc/internal.h inc/serverManager.h
	$(CC) $(CFLAGS) c_files/server.c -D_POSIX_C_SOURCE=199309L

serverManager.o : c_files/serverManager.c inc/serverManager.h inc/logmngr.h inc/internal.h inc/DBmanager.h
	$(CC) $(CFLAGS) c_files/serverManager.c 

DBmanager.o : c_files/DBmanager.c inc/DBmanager.h inc/logmngr.h inc/internal.h inc/userDB.h inc/groupDB.h
	$(CC) $(CFLAGS) c_files/DBmanager.c 

userDB.o : c_files/userDB.c inc/userDB.h inc/logmngr.h inc/internal.h inc/HashMap.h
	$(CC) $(CFLAGS) c_files/userDB.c
	
groupDB.o : c_files/groupDB.c inc/groupDB.h inc/logmngr.h inc/internal.h inc/HashMap.h inc/list.h inc/list_itr.h inc/list_functions.h
	$(CC) $(CFLAGS) c_files/groupDB.c
	
msgReceiver : msgReceiver.o
	$(CC) -o msgReceiver msgReceiver.o -L./lib -llogger -lds

msgReceiver.o : c_files/msgReceiver.c inc/internal.h inc/logmngr.h
	$(CC) $(CFLAGS) c_files/msgReceiver.c



msgSender : msgSender.o
	$(CC) -o msgSender msgSender.o -L./lib -llogger -lds

msgSender.o : c_files/msgSender.c inc/internal.h inc/logmngr.h
	$(CC) $(CFLAGS) c_files/msgSender.c
	
clean:
	rm -f *.o
	rm -f server
	rm -f client













































