all: philosopherClient philosopherServer

philosopherClient: philosopherClient.c philosopher.h
	$(CC)	-o philosopherClient philosopherClient.c

philosopherServer: philosopherServer.c philosopher.h
	$(CC)	-o philosopherServer philosopherServer.c

clean:
	rm -f philosopherClient
	rm -f philosopherServer