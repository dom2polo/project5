all: clean programManager philosopher centralized

programManager: programManager.c header.h
	$(CC)	-o programManager header.h programManager.c

philosopher: philosopher.c header.h
	$(CC)	-o philosopher header.h philosopher.c

centralized: centralized.c header.h
	$(CC)	-o centralized header.h centralized.c -pthread

clean:
	rm -f programManager
	rm -f philosopher
	rm -f centralized