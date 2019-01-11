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

/* ################################################################ */
/* ############### LoRa connection on Feather M0 ################## */
/* ################################################################ */
/* # Connect io1, and io2 to 2 different digital ports, and       # */
/* # define PIN_CONNECT_TO_IO1 and PIN_CONNECT_TO_IO2             # */
/* ################################################################ */

/* ################################################################ */
/* ####################### Libraries ############################## */
/* ################################################################ */
/* Libraries for the OLED display in SPI mode */
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

/* Library for DHT11 */
#include <DHT.h>

/*Libraries for LoRa*/
#include <lmic.h>
#include <hal/hal.h>
// #include <SPI.h>
// #include <Wire.h>

/* ################################################################ */
/* ########################## Define ############################## */
/* ################################################################ */
/* for LORA connection */
#define PIN_CONNECT_TO_IO1 6
#define PIN_CONNECT_TO_IO2 5

/* Declaration for the OLED display using software SPI */
#define OLED_WIDTH 128 // pixels
#define OLED_HEIGHT 32 // in pixels
#define OLED_MOSI 20
#define OLED_CLK 21
#define OLED_DC 10
#define OLED_CS 11
#define OLED_RESET 9
Adafruit_SSD1306 display(OLED_WIDTH, OLED_HEIGHT,
						 OLED_MOSI, OLED_CLK, OLED_DC, OLED_RESET, OLED_CS);

/* for KY-026 */
#define FLAME_SIG A0

/* for DHT11 */
#define DHT_DATA 15	// label A1
#define DHTTYPE DHT11

/* for HC-SR04 - left */
#define ULTRA_TRIG_L 16	// label A2
#define ULTRA_ECHO_L 17	// label A3


/* BIN level settings */
#define LVL_FULL 5	 // FULL limit
#define LVL_HIGH 15	// HIGH limit
#define LVL_MEDIDUM 30 // MEDIUM limit
#define BIN_HIGH 40	// Height of the bin

/* for mini microswitch */
#define SWITCH_PIN 15
/* ################################################################ */
/* ####################### Global variables ####################### */
/* ################################################################ */

/* LoRa Settings */
// Network key
static const PROGMEM u1_t NWKSKEY[16] = {0xE3, 0x80, 0xB4, 0x9A, 0x74, 0xB8, 0x07, 0xBE, 0x9F, 0x4D, 0xD2, 0xC5, 0x58, 0x21, 0x84, 0x1E};
// Application key
static const u1_t PROGMEM APPSKEY[16] = {0x59, 0xF7, 0xA3, 0xCA, 0x83, 0x42, 0x5C, 0x01, 0x5D, 0x48, 0xFA, 0xB7, 0x71, 0x9C, 0xE7, 0x5D};
// Devices Address
static const u4_t DEVADDR = 0x26011020; // <-- Change this address for every node!
const unsigned TX_INTERVAL = 15;
static osjob_t sendjob;
unsigned char mydata[29];
void os_getArtEui(u1_t *buf) {}
void os_getDevEui(u1_t *buf) {}
void os_getDevKey(u1_t *buf) {}

// Pin mapping
// pin 4, 3, 8 belong to LoRa Module control (see in pinout image of feather M0 LORA)
const lmic_pinmap lmic_pins = {
	.nss = 8,
	.rxtx = LMIC_UNUSED_PIN,
	.rst = 4,
	.dio = {3, PIN_CONNECT_TO_IO1, PIN_CONNECT_TO_IO2}};

/* Global variables of OLED display */
char OLED_String[85]; // OLED size 21x4 = 84 + '\0'(in characters)
char flame_Str[9] = "        ";
char ultra_Str[7] = "      ";
char humi_Str[4] = "   ";
char state_Str[6] = "     ";
char temp_Str[4] = "   ";
unsigned long interval = 500; // in milliseconds
unsigned long previousMillis = 0;

/* Global variables of Flame sensor KY-026 */
int flame_Value = 0; // variable to store the value coming from the photocell val
char flame_temp_Str_1[] = "No Flame";
char flame_temp_Str_2[] = "On Fire!";

/*Global variables of DHT11 */
DHT dht(DHT_DATA, DHTTYPE);
float DHT_temp = 0, DHT_humi = 0;

/* Global variables for Ultrasonic sensors */
long ultra_duration_L = 0;
int ultra_distance_L = 0;
long ultra_duration_R = 0;
int ultra_distance_R = 0;
char ultra_temp_Str_1[] = "LOW   ";
char ultra_temp_Str_2[] = "MEDIUM";
char ultra_temp_Str_3[] = "HIGH  ";
char ultra_temp_Str_4[] = "FULL  ";

/* Global variable for mini switch */
int switchState = 0;
char state_temp_Str1[] = "CLOSE";
char state_temp_Str2[] = "OPEN ";

/* ################################################################ */
/* ##################### Function definitions ##################### */
/* ################################################################ */

/* ################### OLED display's functions ################### */
void wasteBinDisplayInitialize(void)
{
	int j = 0;
	char OLED_temp_String[] = "- WASTE BIN MONITOR -Level:      |        Humidity:   %|       Temperature:    C   ";
	for (int i = 0; i < sizeof(OLED_String); i++)
		OLED_String[i] = OLED_temp_String[i];
	OLED_String[78] = 248; // charater '°'
}

void displayData(void)
{
	display.clearDisplay();
	display.setTextSize(1);		 // Normal 1:1 pixel scale
	display.setTextColor(WHITE); // Draw white text
	display.setCursor(0, 0);	 // Start at top-left corner
	display.cp437(true);		 // Use full 256 char 'Code Page 437' font
	for (int i = 0; i < 84; i++)
		display.write(OLED_String[i]);
	display.display();
	delay(2000);
}

void constructStr(char *lvl, char *flame, char *humi, char *state, char *temp)
{
	int j = 0;
	if (strlen(lvl) <= 7 && strlen(flame) <= 9 && strlen(humi) <= 4 && strlen(state) <= 6 && strlen(temp) <= 4)
	{
		for (int i = 27, j = 0; i < 33; i++, j++)
			OLED_String[i] = *(lvl + j);
		for (int i = 34, j = 0; i < 42; i++, j++)
			OLED_String[i] = *(flame + j);
		for (int i = 51, j = 0; i < 54; i++, j++)
			OLED_String[i] = *(humi + j);
		for (int i = 56, j = 0; i < 61; i++, j++)
			OLED_String[i] = *(state + j);
		for (int i = 75, j = 0; i < 78; i++, j++)
			OLED_String[i] = *(temp + j);
	}
}

/* #################### Mini switch's function #################### */
void miniswitch(void)
{
	switchState = digitalRead(SWITCH_PIN);
	if (switchState == HIGH)
	{
		for (int i = 0; i < sizeof(state_Str); i++)
			state_Str[i] = state_temp_Str1[i];
	}
	else
	{
		for (int i = 0; i < sizeof(state_Str); i++)
			state_Str[i] = state_temp_Str2[i];
	}
	Serial.println(state_Str);
}

/* ##################### Flame sensor's KY-026 #################### */
void flameKY026(void)
{
	flame_Value = analogRead(FLAME_SIG);
	flame_Value = constrain(flame_Value, 0, 1023);
	Serial.print("Flame sensor = ");
	Serial.println(flame_Value); //print our value
	if (flame_Value < 750)
	{
		for (int i = 0; i < sizeof(flame_Str); i++)
		{
			flame_Str[i] = flame_temp_Str_1[i];
		}
	}
	else
	{
		for (int i = 0; i < sizeof(flame_Str); i++)
		{
			flame_Str[i] = flame_temp_Str_2[i];
		}
	}
	Serial.print("Flame detector: ");
	Serial.println(flame_Str);
}

/* ################ DHT_humidity and Temperature DHT11 ################ */
void dht11(void)
{
	/* Read DHT_humi_dataPin */
	DHT_temp = dht.readTemperature();
	DHT_temp = constrain(DHT_temp, 0, 50);
	DHT_humi = dht.readHumidity();
	DHT_humi = constrain(DHT_humi, 20, 90);

	/* Update temp_Str and humi_Str */
	temp_Str[0] = ((int)DHT_temp) / 100 + 48;
	temp_Str[1] = ((int)DHT_temp) / 10 + 48;
	temp_Str[2] = ((int)DHT_temp) % 10 + 48;
	humi_Str[0] = ((int)DHT_humi) / 100 + 48;
	humi_Str[1] = ((int)DHT_humi) / 10 + 48;
	humi_Str[2] = ((int)DHT_humi) % 10 + 48;

	/* Display DHT_humidity and temperature on Serial Monitor */
	Serial.print("DHT_humidity: ");
	Serial.print(DHT_humi);
	Serial.println("%");
	Serial.print("Temperature: ");
	Serial.print(DHT_temp);
	Serial.println("°C");
}
/* ###################### Ultrasonic HC-SR04 ###################### */
void ultrasonic(void)
{
	// Reset ULTRA_TRIG_L pin
	digitalWrite(ULTRA_TRIG_L, LOW);
	delayMicroseconds(5);

	// Set ULTRA_TRIG_L pin HIGH 10 us
	digitalWrite(ULTRA_TRIG_L, HIGH);
	delayMicroseconds(10);
	digitalWrite(ULTRA_TRIG_L, LOW);

	/*
   * Read ultro_echo pin (pulseIn() returns the length of the pulse in micro sec)
   * the length of the pulse = time the ultrasonic travel from the sensor 
   * to a object and come back to the sensor
   */
	ultra_duration_L = pulseIn(ULTRA_ECHO_L, HIGH);
	/* constrain the input, HC-SR04 range = 2cm-400cm = 120us - 23530us*/
	ultra_duration_L = constrain(ultra_duration_L, 120, 23530);

	/* 
   *  Convert the length of the pulse to distance (cm)
   *  Speed of sound = 340m/s = 0.00034m/us = 0.034cm/us
   *  ultra_distance_L = (ultra_duration_L/2) * 0.034
  */
	ultra_distance_L = ultra_duration_L * 0.034 / 2;
	ultra_distance_L = constrain(ultra_distance_L, 0, BIN_HIGH);

	/* Update ultra_Str */
	if (ultra_distance_L <= LVL_FULL)
	{
		for (int i = 0; i < 6; i++)
			ultra_Str[i] = ultra_temp_Str_4[i];
	}
	else if (ultra_distance_L <= LVL_HIGH)
	{
		for (int i = 0; i < 6; i++)
			ultra_Str[i] = ultra_temp_Str_3[i];
	}
	else if (ultra_distance_L <= LVL_MEDIDUM)
	{
		for (int i = 0; i < 6; i++)
			ultra_Str[i] = ultra_temp_Str_2[i];
	}
	else if (ultra_distance_L <= BIN_HIGH)
	{
		for (int i = 0; i < 6; i++)
			ultra_Str[i] = ultra_temp_Str_1[i];
	}

	// Display distance on Serial Monitor
	Serial.print("Distance: ");
	Serial.print(ultra_distance_L);
	Serial.println(" cm");
	Serial.print("Level: ");
	Serial.println(ultra_Str);
}

/* ###################### LORA's functions ######################## */
void onEvent(ev_t ev)
{
	Serial.print(os_getTime());
	Serial.print(": ");
	switch (ev)
	{
	case EV_SCAN_TIMEOUT:
		Serial.println(F("EV_SCAN_TIMEOUT"));
		break;
	case EV_BEACON_FOUND:
		Serial.println(F("EV_BEACON_FOUND"));
		break;
	case EV_BEACON_MISSED:
		Serial.println(F("EV_BEACON_MISSED"));
		break;
	case EV_BEACON_TRACKED:
		Serial.println(F("EV_BEACON_TRACKED"));
		break;
	case EV_JOINING:
		Serial.println(F("EV_JOINING"));
		break;
	case EV_JOINED:
		Serial.println(F("EV_JOINED"));
		break;
	case EV_RFU1:
		Serial.println(F("EV_RFU1"));
		break;
	case EV_JOIN_FAILED:
		Serial.println(F("EV_JOIN_FAILED"));
		break;
	case EV_REJOIN_FAILED:
		Serial.println(F("EV_REJOIN_FAILED"));
		break;
	case EV_TXCOMPLETE:
		Serial.println(F("EV_TXCOMPLETE (includes waiting for RX windows)"));
		if (LMIC.txrxFlags & TXRX_ACK)
			Serial.println(F("Received ack"));
		if (LMIC.dataLen)
		{
			Serial.println(F("Received "));
			Serial.println(LMIC.dataLen);
			Serial.println(F(" bytes of payload"));
		}
		// Schedule next transmission
		os_setTimedCallback(&sendjob, os_getTime() + sec2osticks(TX_INTERVAL), do_send);
		break;
	case EV_LOST_TSYNC:
		Serial.println(F("EV_LOST_TSYNC"));
		break;
	case EV_RESET:
		Serial.println(F("EV_RESET"));
		break;
	case EV_RXCOMPLETE:
		// data received in ping slot
		Serial.println(F("EV_RXCOMPLETE"));
		break;
	case EV_LINK_DEAD:
		Serial.println(F("EV_LINK_DEAD"));
		break;
	case EV_LINK_ALIVE:
		Serial.println(F("EV_LINK_ALIVE"));
		break;
	default:
		Serial.println(F("Unknown event"));
		break;
	}
}

void do_send(osjob_t *j)
{
	// Check if there is not a current TX/RX job running
	if (LMIC.opmode & OP_TXRXPEND)
	{
		Serial.println(F("OP_TXRXPEND, not sending"));
	}
	else
	{

		String temporary = "";
		temporary = String(ultra_Str) + String('|') + String(flame_Str) + String('|') + String(humi_Str) + String('|') + String(state_Str) + String('|') + String(temp_Str) + String(' ');
		temporary.toCharArray((char *)mydata, temporary.length());
		// Prepare upstream data transmission at the next possible time.
		LMIC_setTxData2(1, mydata, sizeof(mydata), 0);
		Serial.println(F("Packet queued"));
	}
	// Next TX is scheduled after TX_COMPLETE event.
}

/* ################################################################ */
/* ######################## Set up section ######################## */
/* ################################################################ */
void setup()
{
	Serial.begin(9600);
	Serial.println(F("Starting"));
	/* ###################### Set up for OLED Display ################# */
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
	/* ###################### Set up for mini switch  ################# */
	pinMode(SWITCH_PIN, INPUT);

	/* ###################### Set up for DHT11 sensor ################# */
	pinMode(DHT_DATA, INPUT); // set DHT_humi_dataPin pin as input

	/* ##################### Set up for HC-SR04 sensor ################ */
	pinMode(ULTRA_TRIG_L, OUTPUT); // set ULTRA_TRIG_L pin as output
	pinMode(ULTRA_ECHO_L, INPUT);  // set ULTRA_ECHO_L pin as input
	pinMode(ULTRA_TRIG_R, OUTPUT); // set ULTRA_TRIG_R pin as output
	pinMode(ULTRA_ECHO_R, INPUT);  // set ULTRA_ECHO_R pin as input
								   /* ################### Set up for LoRa connection ################# */

#ifdef VCC_ENABLE
	pinMode(VCC_ENABLE, OUTPUT);
	digitalWrite(VCC_ENABLE, HIGH);
	delay(1000);
#endif

	// LMIC init
	os_init();
	// Reset the MAC state. Session and pending data transfers will be discarded.
	LMIC_reset();
	LMIC_setClockError(MAX_CLOCK_ERROR * 1 / 100);
/*
	Set static session parameters. Instead of dynamically establishing a session
	by joining the network, precomputed session parameters are be provided.
*/
#ifdef PROGMEM
	/*
	On AVR, these values are stored in flash and only copied to RAM
	once. Copy them to a temporary buffer here, LMIC_setSession will
	copy them into a buffer of its own again.
*/
	uint8_t appskey[sizeof(APPSKEY)];
	uint8_t nwkskey[sizeof(NWKSKEY)];
	memcpy_P(appskey, APPSKEY, sizeof(APPSKEY));
	memcpy_P(nwkskey, NWKSKEY, sizeof(NWKSKEY));
	LMIC_setSession(0x1, DEVADDR, nwkskey, appskey);
#else
	// If not running an AVR with PROGMEM, just use the arrays directly
	LMIC_setSession(0x1, DEVADDR, NWKSKEY, APPSKEY);
#endif

#if defined(CFG_eu868)
	/*
	Set up the channels used by the Things Network, which corresponds
	to the defaults of most gateways. Without this, only three base
	channels from the LoRaWAN specification are used, which certainly
	works, so it is good for debugging, but can overload those
	frequencies, so be sure to configure the full frequency range of
	your network here (unless your network autoconfigures them).
	Setting up channels should happen after LMIC_setSession, as that
	configures the minimal channel set.
	NA-US channels 0-71 are configured automatically
*/
	LMIC_setupChannel(0, 868100000, DR_RANGE_MAP(DR_SF12, DR_SF7), BAND_CENTI);  // g-band
	LMIC_setupChannel(1, 868300000, DR_RANGE_MAP(DR_SF12, DR_SF7B), BAND_CENTI); // g-band
	LMIC_setupChannel(2, 868500000, DR_RANGE_MAP(DR_SF12, DR_SF7), BAND_CENTI);  // g-band
	LMIC_setupChannel(3, 867100000, DR_RANGE_MAP(DR_SF12, DR_SF7), BAND_CENTI);  // g-band
	LMIC_setupChannel(4, 867300000, DR_RANGE_MAP(DR_SF12, DR_SF7), BAND_CENTI);  // g-band
	LMIC_setupChannel(5, 867500000, DR_RANGE_MAP(DR_SF12, DR_SF7), BAND_CENTI);  // g-band
	LMIC_setupChannel(6, 867700000, DR_RANGE_MAP(DR_SF12, DR_SF7), BAND_CENTI);  // g-band
	LMIC_setupChannel(7, 867900000, DR_RANGE_MAP(DR_SF12, DR_SF7), BAND_CENTI);  // g-band
	LMIC_setupChannel(8, 868800000, DR_RANGE_MAP(DR_FSK, DR_FSK), BAND_MILLI);   // g2-band
/*
	TTN defines an additional channel at 869.525Mhz using SF9 for class B
	devices' ping slots. LMIC does not have an easy way to define set this
	frequency and support for class B is spotty and untested, so this
	frequency is not configured here.
*/
#elif defined(CFG_us915)
	/*
	NA-US channels 0-71 are configured automatically
	but only one group of 8 should (a subband) should be active
	TTN recommends the second sub band, 1 in a zero based count.
	https://github.com/TheThingsNetwork/gateway-conf/blob/master/US-global_conf.json
*/
	LMIC_selectSubBand(1);
#endif

	// Disable link check validation
	LMIC_setLinkCheckMode(0);

	// TTN uses SF9 for its RX2 window.
	LMIC.dn2Dr = DR_SF9;

	// Set data rate and transmit power for uplink (note: txpow seems to be ignored by the library)
	LMIC_setDrTxpow(DR_SF7, 14);

	// Start job
	do_send(&sendjob);
}

/* ################################################################ */
/* ######################### Loop section ######################### */
/* ################################################################ */
void loop()
{
	os_runloop_once();
	if ((unsigned long)(millis() - previousMillis) >= interval)
	{
		display.clearDisplay();
		flameKY026();
		Serial.println("---------");
		dht11();
		Serial.println("---------");
		ultrasonic();
		Serial.println("---------");
		miniswitch();
		Serial.println("#########");
		constructStr(ultra_Str, flame_Str, humi_Str, state_Str, temp_Str);
		displayData();
	}
}

