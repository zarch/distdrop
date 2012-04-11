distdrop: main.o list.o test.o drop.o
	$(CC) -o distdrop main.o list.o test.o drop.o

main.o: main.c list.h
list.o: list.c list.h
test.o: test.c test.h
drop.o: drop.c drop.h
