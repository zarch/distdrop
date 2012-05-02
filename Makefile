CFLAGS = -Wall -pedantic -std=c99 -ggdb -g2

distdrop: main.o list.o drop.o ascii.o
	$(CC) -o distdrop main.o list.o drop.o ascii.o

clean:
	rm *.o distdrop

main.o: main.c list.h
list.o: list.c list.h
drop.o: drop.c drop.h
ascii.o: ascii.c ascii.h
