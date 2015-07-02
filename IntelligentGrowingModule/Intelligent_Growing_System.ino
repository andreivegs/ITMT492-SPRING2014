#include <LiquidCrystal.h>
#include <SimpleTimer.h>

SimpleTimer timer;                  // Timer Object
LiquidCrystal lcd(12,11,5,4,3,2);   // LCD Object

const int numRows = 2;              // Num of Rows in LCD
const int numCols = 16;             // Num of Cols in LCD
const int lightSensorPin = A0;       // Pin from Light Sensor
const int lightRelayPin = 13;        // Pin to Light Relay
int lightReading;                   // Global Light Value
int lightState = 0;                  // Global Light State
long startTime;                     // Time Articial Light Turned On or Off
long elapsedTime;                   // Time passed since light turned on or off
String lightStatus;                 // String conversion of lightState for output to LCD
String conCat1;
String conCat2;
String conCat3;
String conCat6;
unsigned long time1 = millis();
unsigned long time2;

void setup() {
   Serial.begin(9600);
   timer.setInterval(10000, printLCD);                      // Light Logic every 10 Mins
//   timer.setInterval(2 * 60 * 1000, light);               // LCD Logic every 2 Mins
   timer.setInterval(5000, light); 
   pinMode(lightRelayPin, OUTPUT);                          // Relay Pin
   lcd.begin(numCols, numRows);                             // Declares size of LCD
} 

void light(){                                                // Called every 10 minutes
 
      lightState = digitalRead(lightRelayPin);                   // Read current state of light
      lightReading = analogRead(lightSensorPin);                 // Read in Light Sensor
    
      if ((lightReading < 500) && (lightState == LOW)){          // Light Reading is weak and Light is Off
          digitalWrite(lightRelayPin, HIGH);                     // Turn on Light
          lightState = 1;
          startTime = millis();       // Begin Timer
      } else if ((lightReading > 500) && (lightState == HIGH)){  // Light Reading is high and light is On
         digitalWrite(lightRelayPin, LOW);                       // Turn off Light
         startTime = millis();                                   // Begin Timer
         lightState = 0;
      }
} 



void printLCD(){   // Called every 2 minutes
     lcd.clear();
     lcd.autoscroll();
     if (lightState == 1){                                        // Converts binary lightState to On/Off for LCD Output
        lightStatus = "ON";
      }else{
        lightStatus = "OFF";
      }
      elapsedTime=millis() - startTime;
      lcd.setCursor(16,0);
      conCat1 = "Light Value: ";
      conCat2 = conCat1 + lightReading;
    
      for (int i = 0; i < conCat2.length(); i++){
        lcd.print(conCat2.charAt(i));
        delay(200);
      }
      delay(2000);
      lcd.clear();
    
      lcd.setCursor(16,0);
    
      conCat3 = " time:                ";
      conCat6 = lightStatus + conCat3;
      for (int c = 0; c < conCat6.length(); c++){
        lcd.print(conCat6.charAt(c));
        delay(200);
       }
      delay(500);
    
       print_time(elapsedTime); 
} 



void print_time(unsigned long t_milli)
{
    char buffer[20];
    int days, hours, mins, secs;
    int fractime;
    unsigned long inttime;

    inttime  = t_milli / 1000;
    fractime = t_milli % 1000;
    // inttime is the total number of number of seconds
    // fractimeis the number of thousandths of a second

    // number of days is total number of seconds divided by 24 divided by 3600
    days     = inttime / (24*3600);
    inttime  = inttime % (24*3600);

    // Now, inttime is the remainder after subtracting the number of seconds
    // in the number of days
    hours    = inttime / 3600;
    inttime  = inttime % 3600;

    // Now, inttime is the remainder after subtracting the number of seconds
    // in the number of days and hours
    mins     = inttime / 60;
    inttime  = inttime % 60;

    // Now inttime is the number of seconds left after subtracting the number
    // in the number of days, hours and minutes. In other words, it is the
    // number of seconds.
    secs = inttime;

    // Don't bother to print days
    sprintf(buffer, "%02d:%02d:%02d.%03d", hours, mins, secs, fractime);
    lcd.print(buffer);
    delay(2000);
}  


void loop() {
  // While time2 - time1 (Elapsed Time) < 17 Hours, run the code
  // Else If Elapsed Time <= 24 hours, reset time1 to current millis(), thus running light code again (day cycle)
  // Else If Elapsed time between 17-24 hours, shut off the light
  time2 = millis();
  if (time2 - time1 < 61200000){
    timer.run();
  } else if (time2 - time1 >= 86400000) {
   time1 = millis(); 
  } else if ((time2 - time1 > 61200000) && (time2 - time1 < 86399999)){
    digitalWrite(lightRelayPin, LOW);
    startTime = millis();                                  
    lightState = 0;
  }
  
  
} 

