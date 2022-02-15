all: client_name server_name rmServer_name

client_name: client_name.c nameServer.h
	cc  client_name.c -o client_name -lsocket -lnsl

server_name: server_name.c nameServer.h
	cc  server_name.c -o server_name -D_REENTRANT -lsocket -lnsl -lthread

rmServer_name: rmServer_name.c nameServer.h
	cc  rmServer_name.c -o rmServer_name -lsocket -lnsl


clean:
	rm -f core
	rm -f server_name 
	rm -f client_name 
	rm -f rmServer_name
	rm -f *.bak
