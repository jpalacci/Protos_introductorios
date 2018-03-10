CC=gcc
CFLAGS=-Wall

HEADERS = ej2.h

default: ej2

ej2.o : ej2.c $(HEADERS)
	gcc -c ej2.c -o ej2.o

ej2: ej2.o
	gcc ej2.o -o ej2

clean:
	rm -f *.o
	rm -f ej2