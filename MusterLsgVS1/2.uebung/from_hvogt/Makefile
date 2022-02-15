

all: client server 

client: client.c service.o
	cc -c client.c
	cc -o client client.o service.o

server: server.c service.o
	cc -c server.c
	cc -o server server.o service.o

service.o: service.c service.h
	cc -c service.c


