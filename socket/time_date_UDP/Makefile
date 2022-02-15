all: client_time_date server_time_date rmServer_time_date

client_time_date: client_time_date.c time_date.h
	cc  client_time_date.c -o client_time_date -lsocket -lnsl

server_time_date: server_time_date.c time_date.h
	cc  server_time_date.c -o server_time_date -lsocket

rmServer_time_date: rmServer_time_date.c time_date.h
	cc  rmServer_time_date.c -o rmServer_time_date -lsocket -lnsl


clean:
	rm -f core
	rm -f server_time_date 
	rm -f client_time_date 
	rm -f rmServer_time_date
	rm -f *.bak
