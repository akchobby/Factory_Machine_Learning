CC = gcc
CFLAGS = -std=c99 -Wall -c
LFLAGS1 = -std=c99 -Wall -L/usr/local/lib
LFLAGS2 = -lgsl -lgslcblas -lm -o

.PHONY: clean
.ALL: link

%.o: %.c
	$(CC) $(CFLAGS) $^

links: %.o
	$(CC) $(LFLAGS1) $^ $(LFLAGS2) $@

clean:
	rm -f *.o *.out
