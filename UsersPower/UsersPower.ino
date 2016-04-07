/* This module detects how many producers we have(how many
   batteries are in the network), how many consumers there are
   in the network, the amount of power that is
   available for the system, and the pricing algorythm */

#include <stdlib.h>

void setup()
{
  Serial.begin(9600);
  
}

void loop()
{
  int batteryID[BATTERIES]
  availPower();
}

typedef struct {
  int UserID;
  int BatteryID;
  boolean UserType;
  boolean Active;
}Users;

/* detects amount of users in system, and their characteristics
   (ID, type,and battery qualities) */
int createUser(int batterDectect,){

}
int batteryDetect(Users *user) {
  /* Detects how many batteries are in the system */
}

int availPower(int batteries[]) {
  int totalPower = 0;
  for(int i = 0; i < sizeof(batteries);i++){
    totalPower = totalPower + batteries[i];
  }
  return totalPower;   
}
/*
// algorithm to determine the pricing of power in the network
int powerPrice(){
    //get value from sensors from the two houses that are producing  
    sensorValue = analogRead(POWER_SENSOR_PIN);
    
    //calibrate current value
      sensorValue = (sensorValue * voltageRef) / (1023);
      current = 1000*sensorValue / (10 * 9.99);
    /* 
    Write algorithm to determine to price of power in the network
    depending on how much power is available at the current time
    
    Variables:
    1) systemPower()
    2) past prices
    3) past systemPower 
        this is where the database comes in
    
    
    // display curent(test to make sure measurement is accurate)
    //lcd.print(current);
    
    //print price after algorithm has been calculated
    //lcd.print(price)
    
    // return networkPowerPrice
    delay(1000);
}
 */
