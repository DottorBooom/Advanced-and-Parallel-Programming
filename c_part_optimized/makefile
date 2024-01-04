CC = gcc
CFLAGS = -O3 -Wall -std=c17 -pedantic -fopenmp
HEADERS = ${wildcards *.h}
OBJECTS = main.o mandelbrot.o pgm.o util.o

main: all

main: ${HEADERS} ${OBJECTS}
	${CC} ${CFLAGS} ${OBJECTS} -lm -mavx -o main

%.o: %.c ${HEADERS}
	${CC} ${CFLAGS} -c $< 

.PHONY: all clean

clean:
	rm *.o
