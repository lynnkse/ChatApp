CFLAGS = -g -c -std=c99 #-pedantic -Werror -Wall 
CC = gcc

# OBJ = ./obj/
CLT_OBJS = clientManager.o client.o userInterface.o
SRV_OBJS = server.o serverManager.o DBmanager.o

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
	
server.o : c_files/server.c inc/server.h inc/logmngr.h inc/internal.h 
	$(CC) $(CFLAGS) c_files/server.c -D_POSIX_C_SOURCE=199309L

serverManager.o : c_files/serverManager.c inc/serverManager.h inc/logmngr.h inc/internal.h 
	$(CC) $(CFLAGS) c_files/serverManager.c 

DBmanager.o : c_files/DBmanager.c inc/DBmanager.h inc/logmngr.h inc/internal.h 
	$(CC) $(CFLAGS) c_files/DBmanager.c 

clean:
	rm -f *.o
	rm -f server
	rm -f client













































