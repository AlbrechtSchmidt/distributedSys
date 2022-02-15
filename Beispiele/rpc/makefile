LDFLAGS=-lrpcsvc -lnsl
date:	date.x	
	rpcgen date.x

rpc_server:	rpc_server.c	date.h
	cc -o rpc_server rpc_server.c date_svc.c $(LDFLAGS)

rpc_client:	rpc_client.c	date.h
	cc -o rpc_client rpc_client.c date_clnt.c $(LDFLAGS)

clean:   
	rm -f core
	rm -f rpc_client
	rm -f rpc_server
	rm -f *.bak
	rm -f date.h
	rm -f date_*
	rm -f *.o

