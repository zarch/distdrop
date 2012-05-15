CFLAGS = -O -Wall -W -pedantic -ansi -std=c99 -ggdb -g2

distdrop: main.o queue.o drop.o ascii.o
	$(CC) -o distdrop main.o queue.o drop.o ascii.o

clean:
	rm *.o distdrop

main.o: main.c queue.h
queue.o: queue.c queue.h
drop.o: drop.c drop.h
ascii.o: ascii.c ascii.h
