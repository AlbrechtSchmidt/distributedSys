LDFLAGS=-lrpcsvc -lnsl
all:	text rpc_server rpc_client

text:	text.x	
	rpcgen text.x

rpc_server:	rpc_server.c	text.h
	gcc -g -o rpc_server rpc_server.c text_svc.c $(LDFLAGS)

rpc_client:	rpc_client.c	text.h
	gcc -g -o rpc_client rpc_client.c text_clnt.c $(LDFLAGS)

clean:	
	rm -f core
	rm -f rpc_client
	rm -f rpc_server
	rm -f *.bak
	rm -f text.h
	rm -f text_*
	rm -f *.o
