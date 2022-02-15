#CFLAGS=-DDEBUG

all: nameserv mytelnet

mytelnet: nameservlib mytelnet.o
	cc -o mytelnet mytelnet.o -L. -lsns
	
nameservlib: nameclient.o
	ar r libsns.a nameclient.o
	ranlib libsns.a

clean:
	rm *.o libsns.a nameserv mytelnet
