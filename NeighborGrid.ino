/* NEIGHBORGRID!!! */
/* includes Display, Network, Power Transfer, and Pricing Modules */

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
 
//Include Sensor, User Connection, and Relay Pins */
const int BATTERY_POWER_SENSOR = P1_5; //battery power sensor
const int LOCAL_POWER_SENSOR = P1_7; //local usage power sensor
const int CONNECTION_PIN = P1_4;  // connected to user's switch
const int RelayPin1 = P2_6;//If high then device is disconnected thru
const int RelayPin2 = P2_7;//If high then device is disconnected thru

//Declare Floats
float voltageRef=3.7;

//Clean up this values below
/*
 //changed line below from "credit" as that is a section name below
   int size = 0; // how many houses are in the network
  float price=0.10;
  float credit_owed=0.10;
  float credit_have = 100;
  int wattage = 0;
  int usage = 0;
  int stat = 1;
  int start=65;
  float sensorValue; 
  float sensorHome;
  float sensorOverall;
  float currentUsage;
  float currentHome;  
  float powerHome;
  float powerUsage;
  */
 
#include <LiquidCrystal.h>
#include <stdlib.h>
#include <pins_energia.h>

void setup()
{
  Serial.begin(9600);
  
  //display setup
  lcd.begin(16, 2);  // Dimensions of LCD Display
  lcd.setCursor(0, 0);  // where cursor will start 
 
  //Sensor Pin Setup
  pinMode(BATTERY_POWER_SENSOR,INPUT);
  pinMode(LOCAL_POWER_SENSOR,INPUT);
 
  //Connection Pin Setup
  pinMode(CONNECTION_PIN,INPUT);
  
  //Relay Pin Setup
  pinMode(RelayPin1,OUTPUT);
  pinMode(RelayPin1,OUTPUT);
  
}

void loop()
{
  Display();
  Serial.println("hello);
}

/*DISPLAY MODULE*/
//Displaying all five values(Price,Connection,Network Power, User Power,Charges)
void Display() {
  
  //display price
  lcd.setCursor(0,0);
  lcd.clear();
  lcd.setCursor(0, 0);
  int price = PriceAlg();
  lcd.print(price);
  
  //Display Credit
  lcd.setCursor(8, 0);
  int userCredit = Credit(); 
  lcd.print(String(userCredit));
  lcd.setCursor(8, 0);
  lcd.print(" $");
  
  //Display Total Network Power
  lcd.setCursor(1, 1);
  int totalPower = NetworkPower();
  lcd.print(String(totalPower));
  lcd.setCursor(4, 1);
  lcd.print("kW/h");

  //Display User's Power
  lcd.setCursor(9,1);
  int userPower = HomePower()
  lcd.print(String(userPower));
  lcd.setCursor(11,1);
  lcd.print("kW/h");
  
  //Display Connection Status
  lcd.setCursor(14,1);
  String userConnection = ConnStatus();
  lcd.print(userConnection);
}

/*NETWORK MODULE*/
//This module detects how many producers we have(how many batteries are in 
//the network), calculates the amount of power in the system, and stores
//the credits the users have to offer.

//Calculate Power in entire network
int NetworkPower(){
  
  //compute power battery from battery power sensor then send to communication
  //module so it can sum all values from each battery power sensor
  
  //read battery sensor and calibrate value
  int batterySensor = analogRead(BATTERY_POWER_SENSOR);
  int sensorCalibrate = (batterySensor*volatageRef)/1023;
  int currentCalibrate = 1000*sensorCalibrate/(9.99);
  
  //Power Value
  int Power = sensorCalibrate*currentCalibrate;
  
  //Write code to work with the communication module
  //Wrtie code to add all the battery sensor values together
  
  //return netPower
}

//Compute power that is available for user
int UserPower() {
  //return availPower;
}

//Return Credit that user inputs and sends it to producer through CommModule
int Credit(int inputCredit){
    /*
    if (localPowerUsage >= .1){
    creditOwed = localPowerUsage + .39;  //divided by time scale, 
    return inputCredit;
    */
}

//Is the users switch on or off
String ConnStatus() {
  String userStatus;
  boolean connection = digitalRead(CONNECTION_PIN);
  
  if(connection == true)
    userStatus = "Conn";
  else userStatus = "N/A";
  
  return "status"
  //return userStatus
}

/*TRANSFER MODULE*/
//This module detects who wants to transfer and where to transfer power to
void PowerTransfer() {
}

/*PRICING MODULE*/
//This module computes the pricing algorithm

int PriceAlg(){
  //return price
}







  

  
  
  
  
  
