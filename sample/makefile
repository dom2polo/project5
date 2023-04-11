CC = gcc
CFLAGS = -pthread

all: clean main centralized coo

main: main.c philosopher.h
	$(CC) $(CFLAGS) -o start philosopher.h main.c

centralized: centralized.c philosopher.h 
	$(CC) $(CFLAGS) -o centralized philosopher.h centralized.c 

coordinator: coordinator.c philosopher.h
	$(CC) $(CFLAGS) -o coordinator philosopher.h coordinator.c 

clean:
	rm -f *start
	rm -f centralized
	rm -f coordinator