all: emulateur.o memoire.o registre.o conversionHexa.o execution.o
	gcc -o emulateur emulateur.o memoire.o registre.o conversionHexa.o execution.o

conversionHexa.o: conversionHexa.c conversionHexa.h
	gcc -c conversionHexa.c -Wall -pedantic -o conversionHexa.o

memoire.o: memoire.c memoire.h
	gcc -c memoire.c -Wall -pedantic -o memoire.o

registre.o: registre.c registre.h
	gcc -c registre.c -Wall -pedantic -o registre.o

execution.o: execution.c execution.h
	gcc -c execution.c -Wall -pedantic -o execution.o

emulateur.o: emulateur.c header.h
	gcc -c emulateur.c -Wall -pedantic -o emulateur.o