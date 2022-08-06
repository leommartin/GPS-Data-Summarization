CC = gcc
flags = -Wall -std=c99 -D_DEFAULT_SOURCE
prog = gps

all: $(prog)

$(prog): *.o
	$(CC) $(flags) *.o -o gps

*.o : *.c
	$(CC) $(flags) -c *.c

clean: 
	rm -f *.o

purge: clean
	rm -f gps


	

