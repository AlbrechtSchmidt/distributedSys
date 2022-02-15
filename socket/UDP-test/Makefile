all: client server rmServer

client: clientUDP.c UDP.h
	cc  clientUDP.c -o client -lsocket -lnsl

server: serverUDP.c UDP.h
	cc  serverUDP.c -o server -lsocket

rmServer: rmServerUDP.c UDP.h
	cc  rmServerUDP.c -o rmServer -lsocket -lnsl


clean:
	rm -f core
	rm -f server 
	rm -f client 
	rm -f rmServer
	rm -f *.bak
