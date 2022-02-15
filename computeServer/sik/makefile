all: ComServer ComClient

ComServer: ComServer.c Compute.h
	gcc -o ComServer -D_REENTRANT ComServer.c -lthread -lsocket -lnsl

ComClient: ComClient.c Compute.h
	gcc -o ComClient ComClient.c -lsocket -lnsl

clean:
	rm -f core
	rm -f ComServer 
	rm -f ComClient 
	rm -f *.bak


