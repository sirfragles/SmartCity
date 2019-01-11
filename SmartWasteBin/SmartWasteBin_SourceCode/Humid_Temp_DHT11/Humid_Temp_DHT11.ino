/* Humidity Sensor DHT11 */

/* dht.h library is available on https://github.com/RobTillaart/Arduino/tree/master/libraries/DHTlib */
#include <DHT.h> 


/* Set pins for DHT11 */
#define DHTDATAPIN 6
#define DHTTYPE DHT11

/* Global varaibles for DHT11 */
DHT dht(DHTDATAPIN, DHTTYPE);
float tmp = 0, humi = 0;

void setup() 
{
  /* DHT11 */
  pinMode(DHTDATAPIN, INPUT);   // set humi_dataPin pin as input

  Serial.begin(9600);             // Start the serial communication  
}

void dht11(void)
{
  /* Read humi_dataPin */
  tmp = dht.readTemperature();
  tmp = constrain(tmp, 0, 50);
  humi = dht.readHumidity();
  humi = constrain(humi, 20, 90);

  /* Display humidity and temperature on Serial Monitor */
  Serial.print("Humidity: ");
  Serial.print(humi);
  Serial.println("%");
  Serial.print("Temperature: ");
  Serial.print(tmp);
  Serial.println("°C");

}
void loop() 
{
  delay(500);  // check and display data every 0.5 seconds
  dht11();
}