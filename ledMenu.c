#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include <signal.h> // Ctrl+c handler
#include <time.h>
#include <sys/types.h>
#include <string.h>
#include <ifaddrs.h> // For network tools
#include <netinet/in.h>
#include <arpa/inet.h>

#include <wiringPi.h>
#include "ledMenu.h"
#include "pcd8544.h"

#define MAX_BTN 4
#define PIN_UP 9
#define PIN_DOWN 11
#define PIN_LEFT 10
#define PIN_RIGHT 7

// TODO: Use PWM pin (PA06)
#define LCD_BACKLIGHT 8
// LCD_PINS: CLK, DIN, DC, CS, RST, Contrast (Max: 127)
#define LCD_PINS 6, 5, 4, 16, 15
#define LCD_CONTRAST 60

void DoNothing()
{
	// Do nothig (Dummy for buttons)
}

static MenuItem MainMenu[] = {
	{"Show time", ShowTime},
	{"Backlight", PrintBackLightMenu},
	{"Network", PrintNetworkSettings},
	{"Power control", PrintPowerSettings},
	{"Show logo", ShowLogo},
	{"[Exit]", DoExitProgram},
};

static MenuItem BackLightMenu[] = {
	{"Turn On", TurnBacklightOn},
	{"Turn Off", TurnBacklightOff},
	{"< Back", PrintMainMenu},
//	{"Increase", BacklightIncrease},
//	{"Decrease", BacklightDecrease},
};

static MenuItem NetworkSettingsMenu[] = {
	{"Show/Set IP", ShowIP},
	{"Show/Set DNS", DoNothing},
	{"Show route", DoNothing}, // print like menu with scroll
	{"< Back", PrintMainMenu},
};

static MenuItem PowerSettingsMenu[] = {
	{"Shutdown", DoShutdown},
	{"Reboot", DoReboot},
	{"< Back", PrintMainMenu},
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
	// TODO: Scroll
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

void DoShutdown()
{
	LCDclear();
	LCDdrawstring(0, 0, "System\nshutdown...");
	LCDdisplay();
	system("shutdown -P now");
	exit(0);
}

void DoReboot()
{
	LCDclear();
	LCDdrawstring(0, 0, "System\nRebooting...");
	LCDdisplay();
	system("reboot");
	exit(0);
}

void DoExitProgram() {
	LCDclear();
	LCDdrawstring(20, 16, "Goodbye!");
	LCDdisplay();
	delay(1000);
	LCDclear();
	LCDdisplay();
	TurnBacklightOff();
	exit(0);
}

void PrintPowerSettings()
{
	Buttons[KEY_RIGHT].OnPress = RunSelected;
	Buttons[KEY_LEFT].OnPress = PrintMainMenu; // Return to top
	Buttons[KEY_UP].OnPress = MenuUp;
	Buttons[KEY_DOWN].OnPress = MenuDown;

	CurrentMenu = &PowerSettingsMenu;
	ActiveMenuItems = sizeof(PowerSettingsMenu) / sizeof(MenuItem);
	ActiveMenuItem = 1;
	DrawMenu();
}

void ShowTime()
{
	// TODO: Update screen every second until [Back] key pressed.
	LCDclear();
	LCDdrawstring(0, 0, (*CurrentMenu)[ActiveMenuItem - 1].Name);
	LCDdrawline(0, 8, LCDWIDTH, 8, BLACK);

	time_t rawtime;
	struct tm *timeinfo;
	char buffer[32];

	time(&rawtime);
	timeinfo = localtime(&rawtime);

	strftime(buffer, 32, "%d/%m/%Y", timeinfo);
	LCDdrawstring(10, 10, buffer); // Print date DD/MM/YYYY
	strftime(buffer, 32, "%H:%M:%S", timeinfo);
	LCDdrawstring(15, 26, buffer); // print time HH:MM:SS
	LCDdisplay();

	Buttons[KEY_RIGHT].OnPress = ShowTime; // TODO: Temporary: Press any key for update screen
	Buttons[KEY_UP].OnPress = TurnBacklightOn;
	Buttons[KEY_DOWN].OnPress = TurnBacklightOff;
	Buttons[KEY_LEFT].OnPress = PrintMainMenu; // Return to previous menu and reset button controls
}

void ShowIP()
{
	Buttons[KEY_RIGHT].OnPress = ShowIP;
	Buttons[KEY_LEFT].OnPress = PrintNetworkSettings;
	Buttons[KEY_UP].OnPress = DoNothing;
	Buttons[KEY_DOWN].OnPress = DoNothing;

	/*
	// TODO: Show interfaces as menu list
	CurrentMenu = &AddrList;
	ActiveMenuItems = eth_count;
	ActiveMenuItem = 1;
	*/

	struct ifaddrs *ifAddrStruct = NULL;
	struct ifaddrs *ifa = NULL;
	void *tmpAddrPtr = NULL;
	uint8_t line = 0;

	getifaddrs(&ifAddrStruct);

	LCDclear();

	for (ifa = ifAddrStruct; ifa != NULL; ifa = ifa->ifa_next)
	{
		// Ignore not connected
		if (ifa->ifa_addr == NULL)
			continue;

		// Ignore Localhost
		if (strcmp(ifa->ifa_name, "lo") == 0)
			continue;

		// Show only IPv4
		if (ifa->ifa_addr->sa_family == AF_INET)
		{
			tmpAddrPtr = &((struct sockaddr_in *)ifa->ifa_addr)->sin_addr;
			char addressBuffer[INET_ADDRSTRLEN];
			inet_ntop(AF_INET, tmpAddrPtr, addressBuffer, INET_ADDRSTRLEN);
			//printf("'%s': %s\n", ifa->ifa_name, addressBuffer);
			LCDdrawstring(5, line * 16, ifa->ifa_name);
			LCDdrawstring(0, line * 16 + 8, addressBuffer);
			line++;
		}
		/*
		// Show IPv6
		else if (ifa->ifa_addr->sa_family == AF_INET6)
		{
			tmpAddrPtr = &((struct sockaddr_in6 *)ifa->ifa_addr)->sin6_addr;
			char addressBuffer[INET6_ADDRSTRLEN];
			inet_ntop(AF_INET6, tmpAddrPtr, addressBuffer, INET6_ADDRSTRLEN);
			printf("'%s': %s\n", ifa->ifa_name, addressBuffer);
		}
		*/
	}

	LCDdisplay();

	if (ifAddrStruct != NULL)
		freeifaddrs(ifAddrStruct); //remember to free ifAddrStruct
}

void TurnBacklightOn()
{
	//digitalWrite(LCD_BACKLIGHT, !digitalRead(LCD_BACKLIGHT));	// Switch
	digitalWrite(LCD_BACKLIGHT, LOW);
}

void TurnBacklightOff()
{
	digitalWrite(LCD_BACKLIGHT, HIGH);
}

void ShowLogo() {
	Buttons[KEY_RIGHT].OnPress = DoNothing;
	Buttons[KEY_LEFT].OnPress = PrintMainMenu;
	Buttons[KEY_UP].OnPress = TurnBacklightOn;
	Buttons[KEY_DOWN].OnPress = TurnBacklightOff;

	LCDshowLogo(BootLogo);	// No need to clear display and draw image.
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
		pullUpDnControl(Buttons[i].pin, PUD_UP);
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

	DoExitProgram();
}
