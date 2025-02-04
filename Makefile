all: clean
	gcc main.c console.c util.c colors.c player.c world.c chunk.c -lncurses -o main
clean:
	rm -f main
