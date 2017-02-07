CFLAGS :=	-std=gnu11 -Wall

all: main.o filefunctions.o
	gcc $(CFLAGS) -o main main.o filefunctions.o

main.o: main.c filefunctions.h
	gcc $(CFLAGS) -c main.c

filefunctions.o: filefunctions.c filefunctions.h
	gcc $(CFLAGS) -c filefunctions.c

clean:
	rm -f *.o
	rm -f main
	rm -f *.core 
