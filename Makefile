CC = gcc
CFLAGS = -Wall -I/usr/local/include -c
LFLAGS1 = -static -std=c99 -Wall -L/usr/local/lib
LFLAGS2 = -lgsl -lgslcblas -lm -o

.PHONY: clean

compile: test2.c
	$(CC) $(CFLAGS) $^

test1: test2.o
	$(CC) $(LFLAGS1) $^ $(LFLAGS2) $@

gplus: test2.c
	g++ $(CFLAGS) $^	
	g++ $(LFLAGS1) test2.o $(LFLAGS2) $@
clean:
	rm -f *.o *.out
