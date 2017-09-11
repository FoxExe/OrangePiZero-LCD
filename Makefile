# Makefile

PROG	= ledMenu
CC	= gcc
CFLAGS	= -Wall -Wextra -O2
LIBS	= -lwiringPi -lpthread

all:
	$(CC) $(CFLAGS) pcd8544.c ledMenu.c $(LIBS) -o $(PROG)

clean:
	rm *.o 2>&1 >/dev/null || true
	rm $(PROG) 2>&1 >/dev/null || true
