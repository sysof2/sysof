LIB :=		`pkg-config --cflags --libs gdk-pixbuf-2.0`
CFLAGS :=	-std=gnu11 -Wall -lm

all: main.o mandelbrot.o output.o
	gcc $(CFLAGS) -o main main.o mandelbrot.o output.o $(LIB)

main.o: main.c mandelbrot.h output.h
	gcc $(CFLAGS) -c main.c $(LIB)

mandelbrot.o: mandelbrot.c mandelbrot.h
	gcc $(CFLAGS) -c mandelbrot.c $(LIB)

output.o: output.c output.h
	gcc $(CFLAGS) -c output.c $(LIB)

clean:
	rm -f *.o
	rm -f main
	rm -f *.core
