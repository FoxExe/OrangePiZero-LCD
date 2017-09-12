# Makefile

PROG	= ledMenu
CC	= gcc
CFLAGS	= -Wall -Wextra -O2
LIBS	= -lwiringPi -lpthread

all:
	$(CC) $(CFLAGS) pcd8544.c ledMenu.c $(LIBS) -o $(PROG)

clean:
	-@rm -rf *.o 2>/dev/null || true
	-@rm -rf $(PROG) 2>/dev/null || true
