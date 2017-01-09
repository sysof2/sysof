LIB :=		`pkg-config --cflags --libs gdk-pixbuf-2.0`
CFLAGS :=	-std=gnu11 -Wall

all: testit.o mandelbrot.o
	gcc $(CFLAGS) -o testit testit.o mandelbrot.o $(LIB)

testit.o: testit.c mandelbrot.h
	gcc $(CFLAGS) -c testit.c $(LIB)

mandelbrot.o: mandelbrot.c mandelbrot.h
	gcc $(CFLAGS) -c mandelbrot.c $(LIB)

clean:
	rm -f *.o
	rm -f main
	rm -f *.core