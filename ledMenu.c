#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include <signal.h> // Ctrl+c handler
#include <time.h>

#include <wiringPi.h>
#include "ledMenu.h"
#include "pcd8544.h"

#define MAX_BTN 4
#define PIN_UP 15
#define PIN_DOWN 16
#define PIN_LEFT 4
#define PIN_RIGHT 5

// TODO: Use PWM pin (PA06)
#define LCD_BACKLIGHT 0
// LCD_PINS: CLK, DIN, DC, CS, RST, Contrast (Max: 127)
#define LCD_PINS 2, 3, 12, 13, 14
#define LCD_CONTRAST 60

void DoNothing()
{
	// Do nothig (Dummy for buttons)
}

static MenuItem MainMenu[] = {
	{"Show time", ShowTime},
	{"Backlight", PrintBackLightMenu},
	{"Network", PrintNetworkSettings},
};

static MenuItem BackLightMenu[] = {
	{"Turn On", TurnBacklightOn},
	{"Turn Off", TurnBacklightOff},
};

static MenuItem NetworkSettingsMenu[] = {
	{"Show info", DoNothing},
	{"Set IP/DHCP", DoNothing},
	{"Set DNS", DoNothing},
};

// Buttons default state
Button Buttons[MAX_BTN] = {
	{PIN_UP, DoNothing},
	{PIN_DOWN, DoNothing},
	{PIN_LEFT, DoNothing},
	{PIN_RIGHT, DoNothing},
};

int ActiveMenuItem = 1; // SHould start from 1 for future tests
int ActiveMenuItems = 0;
static int keepRunning = 1;
MenuItem (*CurrentMenu)[];

void intHandler(int dummy)
{
	printf("\nGot signal: %d, quit...\n", dummy);
	keepRunning = 0;
}

void DrawMenu()
{
	printf("DrawMenu(); Item: %d of %d\n", ActiveMenuItem, ActiveMenuItems);
	LCDclear();
	for (int i = 0; i < ActiveMenuItems; i++)
	{
		/*
		if (i == (ActiveMenuItem - 1)) {
			LCDfillrect(0, i * 8, LCDWIDTH, 9, BLACK);
			LCDSetFontColor(WHITE);
			LCDdrawstring(0, i * 8 + 1, ">");
			LCDdrawstring(6, i * 8 + 1, (*CurrentMenu)[i].Name);
			LCDSetFontColor(BLACK);
		} else {
			LCDdrawstring(6, i * 8 + 1, (*CurrentMenu)[i].Name);
		}
		*/
		LCDdrawstring(0, i * 8 + 1, (i == (ActiveMenuItem - 1)) ? ">" : " ");
		LCDdrawstring(6, i * 8 + 1, (*CurrentMenu)[i].Name);
	}
	LCDdisplay();
}

void RunSelected()
{
	printf("RunSelected(); Item #%d\n", ActiveMenuItem);
	(*CurrentMenu)[ActiveMenuItem - 1].Run();
}

void MenuUp()
{
	printf("MenuUp()\n");
	if ((ActiveMenuItem - 1) < 1)
		ActiveMenuItem = ActiveMenuItems;
	else
		ActiveMenuItem--;
	DrawMenu();
}

void MenuDown()
{
	printf("MenuDown()\n");
	if ((ActiveMenuItem + 1) > ActiveMenuItems)
		ActiveMenuItem = 1;
	else
		ActiveMenuItem++;
	DrawMenu();
}

void PrintMainMenu()
{
	Buttons[KEY_RIGHT].OnPress = RunSelected;
	Buttons[KEY_LEFT].OnPress = PrintMainMenu; // Return to top
	Buttons[KEY_UP].OnPress = MenuUp;
	Buttons[KEY_DOWN].OnPress = MenuDown;

	CurrentMenu = &MainMenu;
	ActiveMenuItems = sizeof(MainMenu) / sizeof(MenuItem);
	ActiveMenuItem = 1;
	DrawMenu();
}

void PrintNetworkSettings()
{
	Buttons[KEY_RIGHT].OnPress = RunSelected;
	Buttons[KEY_LEFT].OnPress = PrintMainMenu; // Return to top
	Buttons[KEY_UP].OnPress = MenuUp;
	Buttons[KEY_DOWN].OnPress = MenuDown;

	CurrentMenu = &NetworkSettingsMenu;
	ActiveMenuItems = sizeof(NetworkSettingsMenu) / sizeof(MenuItem);
	ActiveMenuItem = 1;
	DrawMenu();
}

void PrintBackLightMenu()
{
	Buttons[KEY_RIGHT].OnPress = RunSelected;
	Buttons[KEY_LEFT].OnPress = PrintMainMenu; // Return to top
	Buttons[KEY_UP].OnPress = MenuUp;
	Buttons[KEY_DOWN].OnPress = MenuDown;

	CurrentMenu = &BackLightMenu;
	ActiveMenuItems = sizeof(BackLightMenu) / sizeof(MenuItem);
	ActiveMenuItem = 1;
	DrawMenu();
}

void ShowTime()
{
	LCDclear();
	LCDdrawstring(0, 0, (*CurrentMenu)[ActiveMenuItem - 1].Name);
	LCDdrawline(0, 8, LCDWIDTH, 8, BLACK);

	time_t rawtime;
	struct tm *timeinfo;
	char buffer[32];

	time(&rawtime);
	timeinfo = localtime(&rawtime);

	strftime(buffer, 32, "%d/%m/%Y", timeinfo);
	LCDdrawstring(10, 18, buffer);
	strftime(buffer, 32, "%H:%M:%S", timeinfo);
	LCDdrawstring(15, 34, buffer);
	LCDdisplay();

	Buttons[KEY_RIGHT].OnPress = DoNothing;
	Buttons[KEY_UP].OnPress = DoNothing;
	Buttons[KEY_DOWN].OnPress = DoNothing;
	Buttons[KEY_LEFT].OnPress = PrintMainMenu; // Return to previous menu and reset button controls
}

void TurnBacklightOn()
{
	digitalWrite(LCD_BACKLIGHT, LOW);
}

void TurnBacklightOff()
{
	digitalWrite(LCD_BACKLIGHT, HIGH);
}

//int main(int argc, char **argv)
int main()
{
	/*
        if (argc != 2) {
                printf("Usage:\n%s \"Some string\"\n", argv[0]);
                return 1;
        }
*/

	printf("Program started...\n");
	signal(SIGINT, intHandler);

	if (wiringPiSetup() == -1)
		return 1;

	//wiringPiSetupSys();

	// Turn on backlight
	pinMode(LCD_BACKLIGHT, OUTPUT);
	TurnBacklightOn();

	// LCD Init: CLK, DIN, DC, CS, RST, Contrast (Max: 127)
	LCDInit(LCD_PINS, LCD_CONTRAST);
	LCDshowLogo(BootLogo);
	// TODO: Wait until system fully started (Boot logo)
	delay(1000);

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

	while (keepRunning)
	{
		for (int i = 0; i < MAX_BTN; i++)
		{
			// LOW = Pressed (0), HIGH = unpressed (1)
			if (digitalRead(Buttons[i].pin) == LOW)
			{
				// Wait some time for debounce
				delay(50);
				// Read again
				while (digitalRead(Buttons[i].pin) == LOW && keepRunning)
				{
					// Still pressed, wait for unhold
				}

				Buttons[i].OnPress();
			}
		}
	}

	LCDclear();
	LCDdisplay();
	TurnBacklightOff();

	return 0;
}
