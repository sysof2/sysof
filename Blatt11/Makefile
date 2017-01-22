LIB :=		-I /opt/diet/include/ -lowfat -L/opt/diet/lib

CFLAGS :=	-std=gnu11 -Wall

all: main.o hashfunction.o
	gcc $(CFLAGS) -o main main.o hashfunction.o $(LIB)

main.o: main.c hashfunction.h
	gcc $(CFLAGS) -c main.c $(LIB)

hashfunction.o: hashfunction.c
	gcc $(CFLAGS) -c hashfunction.c $(LIB)

clean:
	rm -f *.o
	rm -f main
	rm -f *.core

