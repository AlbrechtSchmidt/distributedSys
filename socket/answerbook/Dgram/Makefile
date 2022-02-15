all: DgramSender DgramReader

DgramSender: DgramSender.C
	g++ DgramSender.C -o DgramSender -lsocket -lnsl

DgramReader: DgramReader.C
	g++ DgramReader.C -o DgramReader -lsocket


clean:
	rm -f core
	rm -f DgramReader 
	rm -f DgramSender 


