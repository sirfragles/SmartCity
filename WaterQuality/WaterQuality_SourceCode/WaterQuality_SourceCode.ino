/*Libraries for LoRa*/
#include <lmic.h>
#include <hal/hal.h>
#include <SPI.h>
#include <Wire.h>

/* Libraries for sensors*/
#include <OneWire.h>
#include <DallasTemperature.h>


/* for LORA connection */
#define PIN_CONNECT_TO_IO1 6
#define PIN_CONNECT_TO_IO2 5

/* for sensors */
#define SensorpHPin A0          //pH meter Analog output to Arduino Analog Input 0
#define SensorTubidityPin A1          //turbidity meter Analog output to Arduino Analog Input 1
#define ONE_WIRE_BUS_1 9 //d9

/* LoRa Settings */
// Network key
static const PROGMEM u1_t NWKSKEY[16] = { 0xC7, 0x4D, 0x97, 0x18, 0xD2, 0xA0, 0x43, 0x30, 0xAD, 0xD1, 0xBE, 0xB1, 0xC5, 0x7F, 0xE2, 0xEA };
// Application key
static const u1_t PROGMEM APPSKEY[16] = { 0x5E, 0x0C, 0x14, 0x2A, 0xF5, 0x0D, 0xAD, 0xD5, 0xD4, 0xEA, 0xA1, 0x04, 0x79, 0x13, 0x33, 0xA7 };
// Devices Address
static const u4_t DEVADDR = 0x26011DD8; // <-- Change this address for every node!
const unsigned TX_INTERVAL = 15;
static osjob_t sendjob;
unsigned char mydata[16];
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

/* Sending data */
char ph[6] = "     ";
char ultra_distance[4] = "   ";
char turbidity[6] = "     ";
char temp[4] = "   ";

unsigned long interval = 500; // in milliseconds
unsigned long previousMillis = 0;

/* for Ultrasonic */
const int trigPin = 10;
const int echoPin = 11;
const int buzzer = 12; //buzzer to arduino pin 12
unsigned long int avgValue;  //Store the average value of the sensor feedback
long duration;
int distance;

/* for sensors*/
OneWire oneWire_in(ONE_WIRE_BUS_1);
DallasTemperature sensor_inhouse(&oneWire_in);

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
    temporary = String(ph) + String('|') + String(ultra_distance) + String('|') + String(turbidity) + String('|') + String(temp) + String(' ');
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
  pinMode(buzzer, OUTPUT); // Set buzzer - pin 8 as an output
  pinMode(trigPin, OUTPUT); // Sets the trigPin as an Output
  pinMode(echoPin, INPUT); // Sets the echoPin as an Input
  pinMode(SensorTubidityPin, INPUT); // Set the analog A1 input
  Serial.println("Ready");    //Test the serial monitor
  digitalWrite(buzzer, LOW);  // set buzzer
  sensor_inhouse.begin();
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

void PH(){
  Serial.println("***********************-- PH --**************** ");
  float b;
  int buf[10],temp;
  // set buzzer
  digitalWrite(buzzer, LOW);
  
  for(int i=0;i<10;i++)       //Get 10 sample value from the sensor for smooth the value
  { 
    buf[i]=analogRead(SensorpHPin);
    delay(10);
  }
  for(int i=0;i<9;i++)        //sort the analog from small to large
  {
    for(int j=i+1;j<10;j++)
    {
      if(buf[i]>buf[j])
      {
        temp=buf[i];
        buf[i]=buf[j];
        buf[j]=temp;
      }
    }
  }
  avgValue=0;
  for(int i=2;i<8;i++)                      //take the average value of 6 center sample
    avgValue+=buf[i];
  float phValue=(float)avgValue*5.0/1024/6; //convert the analog into millivolt
  phValue=3.5*phValue;                      //convert the millivolt into pH value

  ph[0] = (int)phValue/10 + 48;
  ph[1] = (int)phValue%10 + 48;
  ph[2] = ',';
  ph[3] = (int)(phValue * 10.0)%10 + 48;
  ph[4] = (int)(phValue * 100.0)%10 + 48;  
  if(phValue >= 7.40){
    digitalWrite(buzzer, HIGH);
    Serial.print("PH VALUE: ");
    Serial.println(phValue);
    Serial.println("Water Alkalinity high");
    delay(500);
   }

  if(phValue >= 5.50 && phValue <= 7.40){
    digitalWrite(buzzer, LOW);
    Serial.print("PH VALUE: ");
    Serial.println(phValue);
    Serial.println("Water Is  neutral (safe)");
    delay(500);
  }

  if(phValue < 5.50){
    digitalWrite(buzzer, HIGH);
    Serial.print("PH VALUE: ");
    Serial.println(phValue);
    Serial.println("Water Acidity High");
    delay(500);
   }
  
  delay(500);
}

void distanceLevel(){
   Serial.println("*********************-- Distance --****************** ");
  digitalWrite(buzzer, LOW);
  // Clears the trigPin
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  // Sets the trigPin on HIGH state for 10 micro seconds
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  // Reads the echoPin, returns the sound wave travel time in microseconds
  duration = pulseIn(echoPin, HIGH);
  // Calculating the distance
  distance= duration*0.034/2;
  distance= constrain(distance, 2, 400);
  // Prints the distance on the Serial Monitor
  Serial.print("Distance: ");
  Serial.print(distance);
  Serial.println(" cm");

  ultra_distance[0] = distance/100 + 48;
  ultra_distance[1] = (distance/10)%10 + 48;
  ultra_distance[2] = distance%10 + 48;
  if(distance < 5){
     digitalWrite(buzzer, HIGH);
    Serial.println("Water Level high");
    delay(1000);
  }

  if(distance >= 5 && distance <= 7){
     digitalWrite(buzzer, LOW);
    Serial.println("Water Level normal");
    delay(1000);
  }
 
  if(distance > 7){
    digitalWrite(buzzer, HIGH);
    Serial.println("Water Level low");
    delay(1000);
  }
  
}

void turbidity() {
  Serial.println("*******************-- turbidity --******************** ");
  int turbidityValue = analogRead(SensorTubidityPin);
  float turbidityV = turbidityValue/100;
  turbidity[0] = (int)turbidityV/10 + 48;
  turbidity[1] = (int)turbidityV%10 + 48;
  turbidity[2] = ',';
  turbidity[3] = (int)(turbidityV * 10.0)%10 + 48;
  turbidity[4] = (int)(turbidityV * 100.0)%10 + 48;   
  if( turbidityV > 9){
    digitalWrite(buzzer, LOW);
    Serial.print("Turbidity Level: ");
    Serial.print(turbidityV); 
    Serial.println("NTU");
    Serial.println("Water Very Clean ");
    delay(1000);
  }

  if( turbidityV >= 5 && turbidityValue/100 <= 9 ){
    digitalWrite(buzzer, LOW);
    Serial.print("Turbidity Level: ");
    Serial.print(turbidityV); 
    Serial.println("NTU");
    Serial.println("Water Clean ");
    delay(1000);
  }

  if( turbidityV < 5){
    digitalWrite(buzzer, HIGH);
    Serial.print("Turbidity Level: ");
    Serial.print(turbidityV); 
    Serial.println("NTU");
    Serial.println("Water Very Dirty ");
    delay(1000);
   }
   
  delay (1000);
}

void temp(){
  Serial.println("*******************-- temperature --******************** ");
  Serial.print("Requesting temperatures...");
  sensor_inhouse.requestTemperatures();
  
  int reading = sensor_inhouse.getTempCByIndex(0);  
  temp[0] = reading/100 + 48;
  temp[1] = (reading/10)%10 + 48;
  temp[2] = reading%10 + 48;
  if(reading > 50){
    digitalWrite(buzzer, HIGH);
    Serial.print("Temperature: ");
    Serial.print(reading); 
    Serial.println(" degree C");
    Serial.println("Temperature high");
    delay(1000);
   }

  if(reading >= 10 && reading <= 50){
    digitalWrite(buzzer, LOW);
    Serial.print("Temperature: ");
    Serial.print(reading); 
    Serial.println(" degree C");
    Serial.println("Surrounding Temperature normal");
    delay(1000);
  }

  if(reading < 10){
    digitalWrite(buzzer, HIGH);
    Serial.print("Temperature: ");
    Serial.print(reading); 
    Serial.println(" degree C");
    Serial.println("Temperature low");
    delay(1000);
   }
  
  delay(1000);
}

/* ################################################################ */
/* ######################### Loop section ######################### */
/* ################################################################ */
void loop()
{
  os_runloop_once();
  if ((unsigned long)(millis() - previousMillis) >= interval)
  {
    PH();
    Serial.println("---------");
    distanceLevel();
    Serial.println("---------");
    turbidity();
    Serial.println("---------");
    temp();
    Serial.println("#########");
    constructStr(ultra_Str, flame_Str, humi_Str, state_Str, temp_Str);
    displayData();
  }
}
