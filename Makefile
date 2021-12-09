all: emulateur.o memoire.o registre.o conversionHexa.o
	gcc -o emulateur emulateur.o memoire.o registre.o conversionHexa.o

memoire.o: memoire.c memoire.h header.h
	gcc -c memoire.c -Wall -pedantic -o memoire.o

conversionHexa.o: conversionHexa.c conversionHexa.h header.h
	gcc -c conversionHexa.c -Wall -pedantic -o conversionHexa.o

registre.o: registre.c registre.h header.h
	gcc -c registre.c -Wall -pedantic -o registre.o

emulateur.o: emulateur.c
	gcc -c emulateur.c -Wall -pedantic -o emulateur.o