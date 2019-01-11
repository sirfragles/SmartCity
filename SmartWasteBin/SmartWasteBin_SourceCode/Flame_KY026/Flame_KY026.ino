/* ################################################################ */
/* ################ Flame sensor - KY026 - OSEPP ################## */
/* ################################################################ */
/* # KY-026 has 3 pins: VCC, GROUND and analog output (SIG) pins  # */
/* ################################################################ */



int flame_SIG = A0;  // flame sensor sensor input
int flame_Value = 0; // variable to store the value coming from the photocell val
char flame_Str[9]; 
char flame_temp_Str_1[] = "No Flame";
char flame_temp_Str_2[] = "On Fire!";
unsigned long interval = 1000; // in milliseconds
unsigned long previousMillis = 0;

void setup()
{
	Serial.begin(9600);
}

void loop()
{
	// read the value from the flame sensor after an interval:
	if ((unsigned long)(millis() - previousMillis) >= interval)
	{
		flame_Value = analogRead(flame_SIG);
		flame_Value = constrain(flame_Value, 0, 1023);
		Serial.print("Flame sensor = ");
		Serial.println(flame_Value); //print our value
		if(flame_Value < 750)
		{
			for(int i = 0; i < sizeof(flame_Str); i++)
			{
				flame_Str[i] = flame_temp_Str_1[i];
			}
		}
		else
		{
			for(int i = 0; i < sizeof(flame_Str); i++)
			{
				flame_Str[i] = flame_temp_Str_2[i];
			}			
		}
		Serial.println(flame_Str);
		previousMillis = millis();
	}
}
