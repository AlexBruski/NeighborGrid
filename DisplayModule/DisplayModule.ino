/*
  LiquidCrystal

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
 */

#include <LiquidCrystal.h>

const int POWER_SENSOR_PIN = P1_0;
const int POWER_SENSOR_PIN = P1_1;
const int POWER_SENSOR_PIN = P1_2;
const int POWER_SENSOR_PIN = P1_3;
const int CONNECTION_PIN = P1_4;
const int POWER_SENSOR_PIN = P1_5;

// initialize the library with the numbers of the interface pins
LiquidCrystal lcd(P2_0, P2_1, P2_2, P2_3, P2_4, P2_5);
  int size = 0; // how many houses are in the network
  float price=0.10;
  float credit=0.10;
  int wattage = 0;
  int usage = 0;
  int stat = 1;
  int start=65;
  float sensorValue; 
  float current;  
  float voltageRef=3.7;

void setup() {
  Serial.begin(9600);
  pinMode(CONNECTION_PIN,INPUT); // NeighborGrid switch input
  lcd.begin(16, 2);  // how big the display will be
  lcd.setCursor(0, 0);  // where cursor will start

}

void loop() {
  
  powerPrice();
  connection();
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
    lcd.print("kW/h");
   
   
    lcd.setCursor(9,1);
    lcd.print(u);
    lcd.setCursor(11,1);
    lcd.print("kW/h");
    
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

int powerPrice(){
    //get value from sensor  
    sensorValue = analogRead(POWER_SENSOR_PIN);
    lcd.setCursor(0,0);
    lcd.clear();
    lcd.setCursor(0, 0);
    
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
    */
    
    // display curent(test to make sure measurement is accurate)
    //lcd.print(current);
    
    //print price after algorithm has been calculated
    //lcd.print(price)
    
    // return networkPowerPrice
    delay(1000);
}

int credit(String userType){
  if(userType == "producer"){
    /* write code to calculate how much power the producer has given
    to other neighbors in the network.
    
    lcd.setCursor(8, 0);
    lcd.print(String(userCredit));
    lcd.setCursor(8, 0);
    lcd.print(" $");    
    */
  }
  if(userType == "consumer"){
    /* write code that takes an input on how much credit a consumer is
    willing to purshuse for power. Depending on how much credit they input and the price
    of power at a given time, they will receive a certain amount of 
    kilowatt hours to use in their home.
    
    lcd.setCursor(8, 0);
    lcd.print(String(userCredit));
    lcd.setCursor(8, 0);
    lcd.print(" $");
    
    return userCredit
    */
  }
}

int systemPower(){
  /* Write code to gather how many batteries or generators are 
  connected in the network. Then sum all the power measurements 
  from each of these generators to find the total amount of power
  in the NeighborGrid network
  
    lcd.setCursor(1, 1);
    lcd.print(String(sysPow));
    lcd.setCursor(4, 1);
    lcd.print("kW/h");  
    
    return networkSystemPower
  */
}

void homePower(String userType) {
  if(userType == "producer"){
    /* code to calculate power that is stored in producer's battery
    
     sensorValue = analogRead(POWER_SENSOR_PIN);
     sensorValue = (sensorValue * voltageRef) / (1023);
     current = 1000*sensorValue / (10 * 9.99);
     */
  }
  if(userType == "consumer"){
    /* take the input of the consumers credits and depending on the price
    of power at the moment, they will receive an amount of kW/h for their
    home
    */
  }
    /*
    lcd.setCursor(9,1);
    lcd.print(String(homePow));
    lcd.setCursor(11,1);
    lcd.print("kW/h");
    */
}

void connection(){
  boolean connection = digitalRead(CONNECTION_PIN);
  if(connection == true){
    lcd.setCursor(14,1);
    lcd.print("On");
  }
  else{ 
    lcd.setCursor(14,1);
    lcd.print("Off");
  } 
}
