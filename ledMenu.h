
enum
{
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

void DoNothing();
void DrawMenu();
void RunSelected();
void MenuUp();
void MenuDown();
void PrintMainMenu();
void PrintNetworkSettings();
void PrintBackLightMenu();
void TurnBacklightOn();
void TurnBacklightOff();
void ShowTime();
void DoReboot();
void DoShutdown();
void PrintPowerSettings();

int main();

const unsigned char BootLogo[] = {
	0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0xC0, 0xE0, 0x70, 0x30, 0x18, 0x18,
	0xFC, 0xFC, 0x1E, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xF0, 0xF0, 0xF0, 0x30,
	0x30, 0x30, 0x30, 0x30, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0x80, 0x80, 0x80, 0x80, 0x00,
	0x00, 0x00, 0x00, 0x80, 0x80, 0x80, 0x00, 0x00, 0x00, 0x00, 0x80, 0x80, 0x80, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xC0, 0xF0, 0xF8, 0xFE, 0xFF, 0xFF, 0xF0, 0xE0, 0xC0,
	0x8E, 0x1F, 0x3F, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xF0, 0xFE, 0x0F, 0x03, 0x01, 0x00,
	0x80, 0x00, 0x00, 0x3F, 0xFF, 0xFF, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0xFF, 0xFF, 0xFF, 0x0C, 0x0C, 0x0C, 0x0C, 0x0C, 0x00, 0x00, 0x00, 0x7C, 0xFF, 0xFF, 0xC3, 0x81,
	0x81, 0xC3, 0xFF, 0xFF, 0x3E, 0x00, 0x00, 0x00, 0xC1, 0xE7, 0x7F, 0x1C, 0x7C, 0xFF, 0xC3, 0x81,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFC, 0xFF, 0xFF, 0xFF, 0xFF, 0x3F,
	0x3F, 0x3F, 0x3C, 0x3C, 0x3D, 0x3F, 0x1F, 0x1E, 0x0E, 0x06, 0x02, 0x00, 0x00, 0x00, 0x0F, 0x3F,
	0xF0, 0xC0, 0xFC, 0xFF, 0xFF, 0xFF, 0xFE, 0xFC, 0xFC, 0xFF, 0xFF, 0xF8, 0xE0, 0x80, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x01, 0x01, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x01, 0x01, 0x01, 0x01, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x01, 0x01, 0x00, 0x00,
	0x00, 0x00, 0x01, 0x01, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x1F,
	0xFF, 0xFF, 0xE0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFF, 0xFE, 0xFC, 0xF8,
	0xF8, 0xF0, 0xF0, 0xE0, 0xE0, 0xC1, 0xC3, 0x87, 0x0F, 0x1F, 0x7F, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
	0xFF, 0xFF, 0xFE, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0xC0, 0xE0,
	0x60, 0x60, 0x60, 0xE0, 0xC0, 0xC0, 0x00, 0x00, 0x00, 0x80, 0xC0, 0xE0, 0x60, 0x60, 0x60, 0x60,
	0x60, 0xC0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x07, 0x0F, 0x1C, 0x38, 0x70, 0x60, 0x00, 0x80, 0xE0, 0xFC,
	0xFF, 0x7F, 0x3F, 0x1F, 0x0F, 0x0F, 0x1F, 0x7F, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFE, 0x00, 0x3F,
	0x7F, 0x7F, 0x3F, 0x1F, 0x0F, 0x07, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x7F, 0xFF, 0xFF, 0x80, 0x00, 0x00, 0x00, 0x80, 0xFF, 0xFF, 0x7F, 0x00, 0x00, 0x83, 0x07, 0x0F,
	0x0E, 0x1C, 0x1C, 0x9C, 0xF8, 0xF8, 0xF0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x60,
	0x3C, 0x0F, 0x03, 0x01, 0x08, 0x1C, 0x38, 0x60, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x01, 0x01,
	0x7B, 0x1F, 0x07, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x01, 0x03, 0x03, 0x03, 0x03, 0x03, 0x01, 0x00, 0x00, 0x00,
	0x00, 0x01, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x01, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
};