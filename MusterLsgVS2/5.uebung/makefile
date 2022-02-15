all: ProjectServer ProjectClient

ProjectServer: ProjectServer.c Project.h
	gcc -o ProjectServer -D_REENTRANT ProjectServer.c -lthread -lsocket -lnsl

ProjectClient: ProjectClient.c Project.h
	gcc -o ProjectClient ProjectClient.c -lsocket -lnsl

clean:
	rm -f core
	rm -f ProjectServer 
	rm -f ProjectClient 
	rm -f *.bak


