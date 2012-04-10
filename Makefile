distdrop: main.o list.o
	$(CC) -o distdrop main.o list.o

main.o: main.c list.h
list.o: list.c list.h
