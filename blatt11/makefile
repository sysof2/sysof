LIB :=		-I /opt/diet/include/ -lowfat -L/opt/diet/lib

CFLAGS :=	-std=gnu11 -Wall

all: main.o hashfunction.o
	gcc $(CFLAGS) -o main main.o hashfunction.o $(LIB) -lm

main.o: main.c hashfunction.h
	gcc $(CFLAGS) -c main.c $(LIB) -lm

hashfunction.o: hashfunction.c hashfunction.h
	gcc $(CFLAGS) -c hashfunction.c

clean:
	rm -f *.o
	rm -f main
	rm -f *.core

