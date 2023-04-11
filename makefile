all: tokenRing tokenServer

tokenRing: tokenRing.c philosophers.h
	$(CC)	-o tokenRing tokenRing.c -pthread -std=c99

tokenServer: tokenServer.c philosophers.h
	$(CC)	-o tokenServer tokenServer.c -pthread -std=c99

clean:
	rm -f tokenRing
	rm -f tokenServer
