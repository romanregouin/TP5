all: string.o pendu.o exe client.o serveur.o client serveur


clean:
	-rm *.o exe client serveur

pendu.o: pendu.c string.h
	gcc -g -c pendu.c -o pendue.o

string.o: string.c string.h
	gcc -g -c string.c -o string.o

client.o: client.c fon.h string.h
	gcc -Wall -Werror -c client.c -o client.o

serveur.o: serveur.c fon.h string.h
	gcc -Wall -Werror -c serveur.c -o serveur.o

fon.o: fon.c fon.h
	gcc  -c fon.c -o fon.o

exe: pendu.o string.o
	gcc -g pendue.o string.o -o exe

client: client.o fon.o string.o
	gcc -g fon.o client.o string.o -o client

serveur: serveur.o fon.o string.o
	gcc -g fon.o serveur.o string.o -o serveur
