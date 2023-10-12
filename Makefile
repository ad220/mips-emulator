SRC_DIR := src
INC_DIR := include
BIN_DIR := bin


all: $(BIN_DIR)/emulateur.o $(BIN_DIR)/memoire.o $(BIN_DIR)/registre.o $(BIN_DIR)/conversionHexa.o $(BIN_DIR)/execution.o
	gcc -o emulateur $(BIN_DIR)/emulateur.o $(BIN_DIR)/memoire.o $(BIN_DIR)/registre.o $(BIN_DIR)/conversionHexa.o $(BIN_DIR)/execution.o -lm

$(BIN_DIR)/conversionHexa.o: $(SRC_DIR)/conversionHexa.c $(INC_DIR)/conversionHexa.h
	gcc -c $(SRC_DIR)/conversionHexa.c -Wall -pedantic -o $(BIN_DIR)/conversionHexa.o

$(BIN_DIR)/memoire.o: $(SRC_DIR)/memoire.c $(INC_DIR)/memoire.h
	gcc -c $(SRC_DIR)/memoire.c -Wall -pedantic -o $(BIN_DIR)/memoire.o

$(BIN_DIR)/registre.o: $(SRC_DIR)/registre.c $(INC_DIR)/registre.h
	gcc -c $(SRC_DIR)/registre.c -Wall -pedantic -o $(BIN_DIR)/registre.o

$(BIN_DIR)/execution.o: $(SRC_DIR)/execution.c $(INC_DIR)/execution.h
	gcc -c $(SRC_DIR)/execution.c -Wall -pedantic -o $(BIN_DIR)/execution.o

$(BIN_DIR)/emulateur.o: $(SRC_DIR)/emulateur.c
	gcc -c $(SRC_DIR)/emulateur.c -Wall -pedantic -o $(BIN_DIR)/emulateur.o

clean:
	@$(RM) -rv $(BIN_DIR)/*
