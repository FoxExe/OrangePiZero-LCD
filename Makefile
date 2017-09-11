# Makefile

OBJS    = ledMenu.o
CC      = gcc
DEBUG   = -g
CFLAGS  = -Wall -c $(DEBUG)
LFLAGS  = -Wall $(DEBUG)
LIBS    = -I/usr/local/include -L/usr/local/lib -lwiringPi

all: $(OBJS)
	$(CC) $(LIBS) $(LFLAGS) $(OBJS) -o a.out

ledMenu.o: ledMenu.c
	$(CC) $(CFLAGS) pcd8544.c ledMenu.c

clean:
	rm *.o a.out
