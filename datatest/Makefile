CFLAGS = -O -Wall -W -pedantic -ansi -std=c99 -ggdb -g2

distdrop: main_alg.o queue_alg.o drop_alg.o ascii_alg.o
	$(CC) -o distdrop main_alg.o queue_alg.o drop_alg.o ascii_alg.o

clean:
	rm *.o distdrop

main_alg.o: main_alg.c queue_alg.h
queue.o: queue_alg.c queue_alg.h
drop.o: drop_alg.c drop_alg.h
ascii.o: ascii_alg.c ascii_alg.h
