all: client server rmServer

client: clientTCP.c TCP.h
	cc -o client clientTCP.c -lsocket -lnsl

server: serverTCP.c TCP.h
	cc -o server serverTCP.c -lsocket -lnsl

rmServer: rmServerTCP.c TCP.h
	cc -o rmServer rmServerTCP.c -lsocket -lnsl

clean:
	rm -f core
	rm -f server 
	rm -f client 
	rm -f rmServer
	rm -f *.bak

