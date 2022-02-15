LDFLAGS=-lrpcsvc -lnsl
date:	date.x	
	rpcgen date.x

server:	server.c	date.h
	cc -o server server.c date_svc.c $(LDFLAGS)

client:	client.c	date.h
	cc -o client client.c date_clnt.c $(LDFLAGS)

