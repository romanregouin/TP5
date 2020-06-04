all: string.o pendu.o exe


clean:
	-rm pendue.o string.o exe

pendu.o: pendu.c string.h
	gcc -g -c pendu.c -o pendue.o

string.o: string.c string.h
	gcc -g -c string.c -o string.o

exe: pendu.o string.o
	gcc -g pendue.o string.o -o exe
