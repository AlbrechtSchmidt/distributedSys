all: client server removeServer

client: client.C KommuDIR.h
	g++ client.C -o client

server: server.C KommuDIR.h
	g++ server.C -o server

removeServer: removeServer.C KommuDIR.h
	g++ removeServer.C -o removeServer

clean:
	rm -f core
	rm -f client
	rm -f server
	rm -f removeServer

