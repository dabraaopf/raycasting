CC=gcc
FLAGS=-std=c89 \
			-Wall \
			-Wextra \
			-ansi \
			-pedantic 
LIBS= -lSDL3 \
			-lm

run: game
	./game

game: main.o
	$(CC) -o game main.o init.o inputs.o vector.o $(LIBS)

main.o: clear init.o inputs.o vector.o
	$(CC) -o main.o -c main.c $(FLAGS) 

init.o:
	$(CC) -o init.o -c init.c $(FLAGS) 

inputs.o:
	$(CC) -o inputs.o -c inputs.c $(FLAGS) 

vector.o:
	$(CC) -o vector.o -c vector.c $(FLAGS) 

clear:
	rm -rf *.o *~
