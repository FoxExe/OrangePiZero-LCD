#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>

#include <wiringPi.h>
#include "pcd8544.h"

typedef struct MenuItem
{
	char *Name;
	void (*Run)();
} MenuItem;

void (*CurrentFunction)();

void PrintHello()
{
	// LCD.Clear();
	// LCD.Print("Hello, world!");
	printf("PrintHello()\n");
}

void PrintWelcome()
{
	printf("PrintWelcome()\n");
}

void PrintSubmenu_1()
{
	//LCD.Clear();
	//Print menu items from submenu #1
	//Print first item like:
	// { "...", PrintMainMenu }
	printf("PrintSubmenu_1()");
}

static MenuItem MainMenu[] = {
	{"Item #1", PrintHello},
	{"Item #2", PrintWelcome},
	{"Submenu #1", PrintSubmenu_1},
};

void PrintMenuItems(MenuItem *item)
{
	printf("PrintMenuItems()\n");
	// for loop (MenuItems)
	// print names only
}

void PrintMainMenu()
{
	PrintMenuItems(MainMenu);
}

int main(int argc, char **argv)
{
	/*
        if (argc != 2) {
                printf("Usage:\n%s \"Some string\"\n", argv[0]);
                return 1;
        }
*/

	PrintMainMenu();

	CurrentFunction = MainMenu[1].Run;
	CurrentFunction();

	// while loop:
	// Read keys
	// If key press - Run "CurrentFunction()"

	return 0;
}
