distdrop: main.o list.o test.o
	$(CC) -o distdrop main.o list.o test.o

main.o: main.c list.h
list.o: list.c list.h
test.o: test.c test.h
