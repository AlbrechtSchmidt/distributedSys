all: client server rmServer

client: clientUDP.c UDP.h
	gcc clientUDP.c -o client -D_REENTRANT -lsocket -lnsl -lthread

server: serverUDP.c UDP.h
	gcc serverUDP.c -o server -D_REENTRANT -lsocket -lthread

rmServer: rmServerUDP.c UDP.h
	gcc rmServerUDP.c -o rmServer -lsocket -lnsl


clean:
	rm -f core
	rm -f server 
	rm -f client 
	rm -f rmServer
	rm -f *.bak
