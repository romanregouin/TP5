#all: string pendue exe


clean:
	-rm pendue.o string.o exe

string.o: string.c string.h
	gcc -Wall -Werror -c string.c -o string.o

pendue.o: pendue.c string.h
	gcc -Wall -Werror -c pendue.c -o pendue.o

exe: pendue.o string.o
	gcc -Wall -Werror pendue.o string.o -o exe
