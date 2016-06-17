all: 
	gcc test.c colors.c player.c world.c chunk.c windowstyle.c -lncurses -o test
