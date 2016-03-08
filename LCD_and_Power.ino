/*
  LiquidCrystal Library - Hello World
 
 Demonstrates the use a 16x2 LCD display.  The LiquidCrystal
 library works with all LCD displays that are compatible with the 
 Hitachi HD44780 driver. There are many of them out there, and you
 can usually tell them by the 16-pin interface.
 
 This sketch prints "Hello World!" to the LCD
 and shows the time.

 The circuit:
 =================================
 LCD pin              Connect to
 ---------------------------------
 01 - GND             GND, pot
 02 - VCC             +5V, pot
 03 - Contrast        Pot wiper
 04 - RS              Pin8 (P2.0)
 05 - R/W             GND
 06 - EN              Pin9 (P2.1)
 07 - DB0             GND
 08 - DB1             GND
 09 - DB2             GND
 10 - DB3             GND
 11 - DB4             Pin10 (P2.2)
 12 - DB5             Pin11 (P2.3)
 13 - DB6             Pin12 (P2.4)
 14 - DB7             Pin13 (P2.5)
 15 - BL+             +5V
 16 - BL-             GND
 =================================

 Library originally added 18 Apr 2008
 by David A. Mellis
 library modified 5 Jul 2009
 by Limor Fried (http://www.ladyada.net)
 example added 9 Jul 2009
 by Tom Igoe
 modified 22 Nov 2010
 by Tom Igoe
 
 This example code is in the public domain.

 http://www.arduino.cc/en/Tutorial/LiquidCrystal
 */
//test
// include the library code:
const int SENSOR_PIN = P1_5;
#include <LiquidCrystal.h>


//Constants for multiple power sensor inputs
const int SENSE_PIN0 = P1_5; 
const int SENSE_PIN1 = P1_6;
const int SENSE_PIN2 = P1_7;

float sensorVal0;   // Variable to store value from analog read
float sensorVal1;
float sensorVal2;

const int RL = 10;       // output resistor value (in kohms)
const int RS = 10;       // Shunt resistor value (in ohms)

//Values for each sensor
float current0;       // Calculated current value
float power0;

float current1;       
float power1;

float current2;       
float power2;

//array for storing last values, increment up to 24 (for a days of values)
//one hours values for three sensors should be averaged into one array to save space
int i = 0;
int powerArray[24] = { };

// initialize the library with the numbers of the interface pins
 
LiquidCrystal lcd(P2_0, P2_1, P2_2, P2_3, P2_4, P2_5);
  float pi = 3.14;
  int counter = 0;
  char avail_power[] = "avail";
  float price=0.10;
  float credit=0.10;
  int wattage = 0;
  int usage = 0;
  int stat = 1;
  int start=65;
  int incomingByte = 0; 
  float voltageRef=4.665;

void setup() {
  // set up the LCD's number of columns and rows: 
  Serial.begin(9600);
  lcd.begin(16, 2);
  // Print a message to the LCD.
  lcd.setCursor(0, 0);

}

void loop() {
  
    //char buf[20];
    //snprintf(buf,sizeof(buf),"%f",price);
    
    //sensor values over three inputs
    sensorVal0 = analogRead(SENSE_PIN0);
    sensorVal1 = analogRead(SENSE_PIN1);
    sensorVal2 = analogRead(SENSE_PIN2);
    
    lcd.setCursor(0,0);
    lcd.clear();
    lcd.setCursor(0, 0);
    
    sensorVal0 = (sensorVal0 * voltageRef) / (1023);
    sensorVal1 = (sensorVal1 * voltageRef) / (1023);
    sensorVal2 = (sensorVal2 * voltageRef) / (1023);

    
    //current and power for sensor 0  (might not need to be multiplied by 1000)
    current0 = 1000 * sensorVal0 / (RL * RS);
    power0 = current0 * sensorVal0;
    Serial.print("Current0: ");
    Serial.print(current0, 3);
    Serial.print("A ");
    
    //current and power for sensor 1
    current1 = 1000 * sensorVal1 / (RL * RS);
    power1 = current1 * sensorVal1;
  
    //current and power for sensor 2
    current2 = 1000* sensorVal2 / (RL * RS);
    power2 = current2 * sensorVal2;

    //set present power value to the array
    //average of power from each sensor 
    powerArray[i] = .333 * (power0 + power1 + power2);
    Serial.print("Power0: ");
    Serial.print(powerArray[i], 3);
    Serial.println("W");
   
     lcd.print(current0);
     lcd.setCursor(5,0);
     lcd.print("mA");
     delay(1000);
    
    // delay(hour)... put into sleep?
    // starts to reset array after a day of values. 
    if (i == 24)
     {
      i = 0;
     }
     
    /*
    String x=String(price);
    String c=String(credit);
    String w=String(wattage);
    String u=String(usage);
    String s=String(start);
    
    
    lcd.setCursor(-1, 0);
    lcd.print(x);
    
    lcd.setCursor(0,0);
    lcd.print(" $");
    lcd.setCursor(7,0);
    lcd.print(" ");
    lcd.setCursor(8, 0);
    lcd.print(c);
    lcd.setCursor(8, 0);
    lcd.print(" $");
    
    lcd.setCursor(1, 1);
    lcd.print(w);
    lcd.setCursor(4, 1);
    lcd.print("W");
    
   
    lcd.setCursor(9,1);
    lcd.print(u);
    lcd.setCursor(11,1);
    lcd.print("W");
    
    lcd.setCursor(14,1);
    lcd.print(start);
   
         
     //lcd.setCursor(, 1);
    // lcd.print(conn_avail);
    
    delay(1000);
    counter++;
    
    if(counter%2==0)
    {
        price=(random(0,100))/((float)50);
        credit=(random(0,1500)/((float)50));
        wattage=(random(0,250));
        usage=(random(0,99));
        stat=(random(0,1)); 
      
    }
    
    if(stat == 1){
      
       start = 65;
    }
     else if(stat == 0) {
      start = 78; 
    }
    */

}
