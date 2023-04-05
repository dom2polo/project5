CC = gcc
CFLAGS = -pthread

all: dining_philosophers

dining_philosophers: main.o centralized.o
	$(CC) $(CFLAGS) -o dining_philosophers main.o centralized.o

main.o: main.c header.h
	$(CC) $(CFLAGS) -c main.c

centralized.o: centralized.c header.h
	$(CC) $(CFLAGS) -c centralized.c

clean:
	rm -f *.o dining_philosophers
