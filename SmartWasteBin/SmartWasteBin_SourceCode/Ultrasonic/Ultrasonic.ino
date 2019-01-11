/* Ultrasonic Sensor HC-SR04 */

/* Set pins for Ultrasonic sensor */
#define ultra_trig 12
#define ultra_echo 13

/* Global variables for Ultrasonic sensor */
long ultra_duration;
int ultra_distance;

/* Set up */
void setup() {
  /* Ultrasonic*/
  pinMode(ultra_trig, OUTPUT);  // set ultra_trig pin as output
  pinMode(ultra_echo, INPUT);   // set ulstra_echo pin as input
  
  Serial.begin(9600); // Start the serial communication
}

/* Ultrasonic's function - use in loop()*/
void ultrasonic(void)
{
  // Reset ultra_trig pin
  digitalWrite(ultra_trig, LOW);
  delayMicroseconds(5);
  
  // Set ultra_trig pin HIGH 10 us
  digitalWrite(ultra_trig, HIGH);
  delayMicroseconds(10);
  digitalWrite(ultra_trig, LOW);
  
  /*
   * Read ultro_echo pin (pulseIn() returns the length of the pulse in micro sec)
   * the length of the pulse = time the ultrasonic travel from the sensor 
   * to a object and come back to the sensor
   */
  ultra_duration = pulseIn(ultra_echo, HIGH);
  /* constrain the input, HC-SR04 range = 2cm-400cm = ~120 - 23530*/
  ultra_duration = constrain(ultra_duration,120,23530);
  
  /* 
   *  Convert the length of the pulse to distance (cm)
   *  Speed of sound = 340m/s = 0.00034m/us = 0.034cm/us
   *  ultra_distance = (ultra_duration/2) * 0.034
  */
  ultra_distance= ultra_duration*0.034/2;
  
  // Display distance on Serial Monitor
  Serial.print("Distance: ");
  Serial.print(ultra_distance);
  Serial.println(" cm");
  }

/* Loop */
void loop() 
{
  delay(500); // check and print the distance every 0.5 second
  ultrasonic();
}