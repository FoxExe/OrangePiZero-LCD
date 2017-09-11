#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>

#include <wiringPi.h>
#include "pcd8544.h"

#define MAX_BTN		4
#define PIN_UP		15
#define PIN_DOWN	16
#define PIN_LEFT	1
#define PIN_RIGHT	4

#define LCD_BACKLIGHT	0

enum {
	KEY_UP,
	KEY_DOWN,
	KEY_LEFT,
	KEY_RIGHT
};

typedef struct Button
{
	short int pin;
	void (*OnPress)();
} Button;

typedef struct MenuItem
{
	char *Name;
	void (*Run)();
} MenuItem;

//void (*CurrentFunction)();

void DoNothing()
{
	// Do nothig (Dummy for buttons)
}

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

// Buttons default state
Button Buttons[MAX_BTN] = {
	{PIN_UP, DoNothing},
	{PIN_DOWN, DoNothing},
	{PIN_LEFT, DoNothing},
	{PIN_RIGHT, DoNothing},
};

void PrintMenuItems(MenuItem *item)
{
	printf("PrintMenuItems()\n");
	// for loop (MenuItems)
	// print names only
}

void SelectCurrent() {
	printf("SelectCurrent()\n");
}

void PrintMainMenu()
{
	PrintMenuItems(MainMenu);
	Buttons[KEY_RIGHT].OnPress = SelectCurrent;
	Buttons[KEY_LEFT].OnPress = DoNothing;
}

int main(int argc, char **argv)
{
	/*
        if (argc != 2) {
                printf("Usage:\n%s \"Some string\"\n", argv[0]);
                return 1;
        }
*/

	wiringPiSetup();

	// Turn on backlight
	// TODO: PWM mode for brighteness control
	pinMode(LCD_BACKLIGHT, OUTPUT);
	digitalWrite(LCD_BACKLIGHT, LOW);


	// LCD Init: CLK, DIN, DC, CS, RST, Contrast (Max: 127)
	LCDInit(2, 3, 12, 13, 14, 64);
	LCDclear();
	LCDdisplay();

	LCDdrawstring(2, 2, "Testing");
	LCDdrawrect(0, 0, LCDWIDTH, LCDHEIGHT, BLACK);
	LCDdrawline(1, 10, LCDWIDTH-1, 10, BLACK);
	LCDdisplay();

	// Init buttons
	for (int i = 0; i < MAX_BTN; i++)
	{
		pinMode(Buttons[i].pin, INPUT);
	}

	PrintMainMenu();

	//CurrentFunction = MainMenu[1].Run();
	//CurrentFunction();
	// or
	//MainMenu[1].Run();
/*
	while (1) {
		for (int i = 0; i < MAX_BTN; i++)
		{
			if (DigitalRead(Buttons[i].pin) == LOW) {
				// Wait some time for debounce
				delay(50);
				// Read again
				if (DigitalRead(Buttons[i].pin) == LOW) {
					// Still pressed, run function.
					Buttons[i].OnPress();
					// Do not run twice!
					Buttons[i].OnPress = DoNothing;
				}
			}
		}
	}
*/
	return 0;
}
