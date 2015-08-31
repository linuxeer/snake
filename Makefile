SRC = main.c snake.c
OBJ = main.o snake.o
PROG = snake

$(PROG): $(OBJ)
	gcc -std=c11 -lncurses $(OBJ) -o $(PROG)

$(OBJ): $(SRC)