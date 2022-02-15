CFLAGS=-DDEBUG

all: nameserv mytelnet

nameserv: nameserv.o
	cc -o nameserv nameserv.o -lthread -lnsl -lsocket
mytelnet: nameservlib mytelnet.o
	cc -o mytelnet mytelnet.o -lnsl -lsocket -L. -lsns
	
nameservlib: nameclient.o
	ar r libsns.a nameclient.o
#	ranlib libsns.a

clean:
	rm *.o libsns.a nameserv mytelnet
