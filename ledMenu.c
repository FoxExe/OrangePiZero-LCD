#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>

#include <wiringPi.h>
#include "pcd8544.h"

int main(int argc, char **argv)
{
	wiringPiSetup();

	// LCD Init: CLK, DIN, DC, CS, RST, Contrast (Max: 127)
	LCDInit(2, 3, 12, 13, 14, 64);
	LCDdrawstring_P(0, 10, "Hello world!");

	return 0;
}
