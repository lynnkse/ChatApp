CFLAGS = -g -c -pedantic -Werror -Wall -std=c99
CC = gcc

# OBJ = ./obj/
OBJS = clientManager.o client.o

.PHONY: clean

client : $(OBJS)
	$(CC) -o client $(OBJS) -L./lib -llogger
	
clientManager.o : c_files/clientManager.c inc/clientManager.h inc/internal.h inc/protocol.h
	$(CC) $(CFLAGS) c_files/clientManager.c 

client.o : c_files/client.c inc/client.h inc/internal.h
	$(CC) $(CFLAGS) c_files/client.c 

clean:
	rm -f obj/*.o
	rm -f server
	rm -f client
