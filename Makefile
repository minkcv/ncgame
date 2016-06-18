all: 
	gcc main.c colors.c player.c world.c chunk.c windowstyle.c -lncurses -o main
