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
#include <stdbool.h>

#include <LiquidCrystal.h>

//headers from power to read data into display
//#include <PowerSensor.h>

//following should be included in header (internal)
const int POWER_SENSOR_PIN0 = P1_5; //solar panel
const int POWER_SENSOR_PIN1 = P1_6; //battery
const int POWER_SENSOR_PIN2 = P1_7; //usage
const int CONNECTION_PIN = P1_4;

//total numbers in system
global int total_batteries = 0;
global int total_solar = 0;
global boolean connection = 1;

//outside power, input from powersensor.h
global float powerBus;

// initialize the library with the numbers of the interface pins
LiquidCrystal lcd(P2_0, P2_1, P2_2, P2_3, P2_4, P2_5);
  int size = 0; // how many houses are in the network
  float price=0.10;

  //changed line below from "credit" as that is a section name below
  float credit_owed=0.10;
  float credit_have = 100;
  int wattage = 0;
  int usage = 0;
  int stat = 1;
  int start=65;
  float sensorValue; 
  float sensorHome;
  float sensorOverall;
  float current;  
  float voltageRef=3.7;

  // array counter
  int i = 0;

void setup() {
  Serial.begin(9600);
  pinMode(CONNECTION_PIN,INPUT); // NeighborGrid switch input
  lcd.begin(16, 2);  // how big the display will be
  lcd.setCursor(0, 0);  // where cursor will start

}

void loop() {
  
  connection();
  credit();
  powerPrice();
  

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

    //turn display off / lower when not in use? 
}

int powerPrice(){
    //get value from internal  
    //read other values to get sum

    //make array for each homes power, then add in every other one except for itself to get overall power?

    /*for reference, delete after
		const int POWER_SENSOR_PIN0 = P1_5; //solar panel
		const int POWER_SENSOR_PIN1 = P1_6; //battery
		const int POWER_SENSOR_PIN2 = P1_7; //usage
		const int CONNECTION_PIN = P1_4; */


	//solar and battery connected, boolean vars t/f
	boolean solar_connect = digitalRead(POWER_SENSOR_PIN0);
    if (solar_connect == true ){
    	//total_solar = total_solar + 1;
    	int valueSolar = analogRead(POWER_SENSOR_PIN0);
    	//lcd.print("Solar Panel Connected")

    }

    boolean battery_connect = digitalRead(POWER_SENSOR_PIN1);
    if (battery_connect == true ){
    	//total_batteries = total_batteries + 1;
    	int valueBattery = analogRead(POWER_SENSOR_PIN1);
    	//lcd.print("Battery Connected")
    }

    //what is flowing for the "usage"
    powerBus = POWER_SENSOR_PIN2;

    sensorHome = valueBattery + valueSolar; //sum of internal home power
    sensorOverall = sensorHome + powerBus; //adding in the power from the bus to give total availability 

    lcd.setCursor(0,0);
    lcd.clear();
    lcd.setCursor(0, 0);
    
    //calibrate current value in home
    sensorHome = (sensorHome * voltageRef) / (1023);
    current = 1000 * sensorHome / (10 * 9.99);

    /* 
    Write algorithm to determine the price of power in the network
    depending on how much power is available at the current time
    
    Variables:
    1) systemPower()
    2) past prices
    3) past systemPower 
        this is where the database comes in
    
    //display curent(test to make sure measurement is accurate)
    lcd.print(current);
    
    //print price after algorithm has been calculated
    lcd.print(price);
    
    // return networkPowerPrice
    */
    delay(1000);
}

int credit(String userType){
  if(userType == "producer"){
    // write code to calculate how much power the producer has given
    // to other neighbors in the network.

//error code checking
    //not sure about this if, if "on" add to credit owed...
    if (totalUsagePower[i] >= totalUsagePower[i-1]){
    	//increase credit amount, 10 cents is just a number. Need to mult by a factor for price / watt
    	credit_owed = credit_owed + totalUsagePower* 0.10;

    //reset Usage after "paid"
    if (credit_have == credit_owed){
    	totalUsagePower = 0;
    	credit = 0;
    	}

    //if less credit then what they need to pay, cut off
    else if (credit_have <= credit_owed){ 
    	connection = 0;
    	}

    i++;
    if (i == 24){ //for hours?
    	i = 0;
    }
}

    /*
    lcd.setCursor(8, 0);
    lcd.print(String(userCredit));
    lcd.setCursor(8, 0);
    lcd.print(" $");    
    */
  }
  if(userType == "consumer"){
    /* write code that takes an input on how much credit a consumer is
    willing to pay for power. Depending on how much credit they input and the price
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

  //wrote in powerPrice section - what about an array? 
  
    lcd.setCursor(1, 1);
    lcd.print(String(sysPow));
    lcd.setCursor(4, 1);
    lcd.print("kW/h");  
    
    return networkSystemPower
  */	
    	

}

//write the code for the power sensor so it can monitor power used from the battery and power used in the local outlets

void homePower(String userType) {
  if(userType == "producer"){
    /* code to calculate power that is stored in producer's battery
    
    sensorValue = analogRead(POWER_SENSOR_PIN0);
    sensorHome = (sensorHome * voltageRef) / (1023);
    current = 1000 * sensorHome / (10 * 9.99);

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
  connection = digitalRead(CONNECTION_PIN);
  if(connection == true){
    lcd.setCursor(14,1);
    lcd.print("On");
  }
  else{ 
    lcd.setCursor(14,1);
    lcd.print("Off");
  } 
}

