/* ################################################################ */
/* ########### Display OLED 0.96" (128x32 pixels) ################# */
/* ################################################################ */
/* # On the back of the OLED display, there are 3 modes:          # */
/* #   IIC: R1, R4, R6, R7, R8                                    # */
/* #   4SPI: R3, R4                                               # */
/* #   3SPI: R2, R3                                               # */
/* # The selected mode is depended on which resistors are used.   # */
/* # In this sketch we use 4SPI because R1(IIC) and R2(3SPI) are  # */
/* # missing on the back of the OLED display. R3 and R4 are used  # */
/* ################################################################ */
/* # The display OLED (in 4SPI mode) has 7 pins:                  # */
/* # 1 2 3 4 5 6 7             Name            Connect to Arduino # */
/* # | | | | | | |________ CS - Chip Select          Any pin      # */
/* # | | | | | |__________ DC - Data/Command         Any pin      # */
/* # | | | | |____________ RES - RST/RESET/REST      Any pin      # */
/* # | | | |______________ D1 - SDA/MOSI             SDA pin      # */
/* # | | |________________ D0 - SCL/CLK/SCK          SCL pin      # */
/* # | |__________________ VCC - Power supply        3.3V         # */
/* # |____________________ GND - Ground              Ground       # */
/* ################################################################ */

/* Library for the OLED display in SPI mode */
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

/* Declaration for the OLED display using software SPI */
#define OLED_WIDTH 128 // pixels
#define OLED_HEIGHT 32 // in pixels
#define OLED_MOSI 20
#define OLED_CLK 21
#define OLED_DC 12
#define OLED_CS 11
#define OLED_RESET 13
Adafruit_SSD1306 display(OLED_WIDTH, OLED_HEIGHT,
						 OLED_MOSI, OLED_CLK, OLED_DC, OLED_RESET, OLED_CS);

/* Global variables of OLED display */
char oledStr[85]; // OLED size 21x4 (in characters) + '/0' = 85

/* Setup section */
void setup()
{
	Serial.begin(9600);

	// SSD1306_SWITCHCAPVCC = generate display voltage from 3.3V internally
	if (!display.begin(SSD1306_SWITCHCAPVCC))
	{
		Serial.println(F("SSD1306 allocation failed"));
		for (;;)
			; // Don't proceed, loop forever
	}

	// Clear the buffer
	display.clearDisplay();
	wasteBinDisplayInitialize();
}

/* Definitions of functions */

void wasteBinDisplayInitialize(void)
{
	int j = 0;
	char title[] = "- WASTE BIN MONITOR -";
	char OLED_LINE2[] = "Level:";
	char OLED_LINE3[] = "Humidity:";
	char OLED_LINE4[] = "Temperature:";
	for (int i = 0; i < 21; i++, j++)
		oledStr[j] = title[i];
	j = 21;
	for (int i = 0; i < 6; i++, j++)
		oledStr[j] = OLED_LINE2[i];
	oledStr[33] = '|';
	j = 42;
	for (int i = 0; i < 9; i++, j++)
		oledStr[j] = OLED_LINE3[i];
	oledStr[54] = '%';
	oledStr[55] = '|';
	j = 63;
	for (int i = 0; i < 12; i++, j++)
		oledStr[j] = OLED_LINE4[i];
	oledStr[78] = 248;
	oledStr[79] = 'C';
}

void displayData(void)
{
	display.clearDisplay();
	display.setTextSize(1);		 // Normal 1:1 pixel scale
	display.setTextColor(WHITE); // Draw white text
	display.setCursor(0, 0);	 // Start at top-left corner
	display.cp437(true);		 // Use full 256 char 'Code Page 437' font
	for (int i = 0; i < 85; i++)
		display.write(oledStr[i]);
	display.display();
	delay(2000);
}

void constructStr(char *lvl, char *flame, char *humi, char *state, char *temp)
{
	int j = 0;
	for (int i = 27; i < 33; i++)
		oledStr[i] = ' ';
	for (int i = 34; i < 42; i++)
		oledStr[i] = ' ';
	for (int i = 51; i < 54; i++)
		oledStr[i] = ' ';
	for (int i = 56; i < 61; i++)
		oledStr[i] = ' ';
	for (int i = 75; i < 78; i++)
		oledStr[i] = ' ';
	if (strlen(lvl) <= 6 && strlen(flame) <= 8 && strlen(humi) <= 3 && strlen(state) <= 5 && strlen(temp) <= 3)
	{
		for (int i = 27, j = 0; i < 33; i++, j++)
			oledStr[i] = *(lvl + j);
		for (int i = 34, j = 0; i < 42; i++, j++)
			oledStr[i] = *(flame + j);
		for (int i = 51, j = 0; i < 54; i++, j++)
			oledStr[i] = *(humi + j);
		for (int i = 56, j = 0; i < 61; i++, j++)
			oledStr[i] = *(state + j);
		for (int i = 75, j = 0; i < 78; i++, j++)
			oledStr[i] = *(temp + j);
	}
}

/* Loop section */
void loop()
{
	// Clear the buffer
	display.clearDisplay();
	constructStr( (char*)"Medium", (char*)"No Flame", (char*)"090", (char*)"Close", (char*)"999");
	displayData();
	delay(500);
}