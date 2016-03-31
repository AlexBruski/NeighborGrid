void setup()
{
  // put your setup code here, to run once:
  
}

void loop()
{
  PowerTransfer();
  
}

void PowerTransfer (int power,int user) {
  /* module for the actual tranfer of power. Takes inputs of how much
  power will be transfered and the user it will be transfered to.
  Should then also recalculate how much power is available in the system
  */
}

int PurchasePower(int credit){
  /* user purchases a certain amount of power depending on how much
  credit the buyer inputs. This function will return how much power
  the buyer will have available to him or her.
  */
  
  //Write algorigthm to determine how much power the user will receive
  //based on the power price at the time and how much credits the buyer gives
  
  int scale;  //scale algorithm to scale the price into power amount
  int pAmount; //amount of power the user will receive
  pAmount = powerPrice()*scale;
  return pAmount
  

int houseID() {
  /* module for identifying the how many houses are in the network
  and give each house an ID and a classification on whether they
  are a producer or a consumer
  */
  
}

// algorithm to determine the pricing of power in the network
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


