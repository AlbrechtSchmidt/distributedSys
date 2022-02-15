all: ProjectServer ProjectClient

ProjectServer: ProjectServer.c
	gcc -o ProjectServer ProjectServer.c -lsocket -lnsl

ProjectClient: ProjectClient.c
	gcc -o ProjectClient ProjectClient.c -lsocket -lnsl

clean:
	rm -f core
	rm -f ProjectServer 
	rm -f ProjectClient 
	rm -f *.bak


