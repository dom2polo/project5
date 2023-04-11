all: clean coordinator philosopher

coordinator: coordinator.c 
	$(CC)	-o coordinator coordinator.c

philosopher: philosopher.c
	$(CC)	-o philosopher philosopher.c

clean:
	rm -f philosopher
	rm -f coordinator