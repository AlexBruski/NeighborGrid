/* NeighborGrid communication module
 * Copyright (c) 2016, Flying Wizards
 * All rights reserved.
 *
 * --/COPYRIGHT--*/
//******************************************************************************
//
//   Built with CCS Version 6.1
//******************************************************************************
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
#include <msp430.h>
#include"comms.h"

LiquidCrystal lcd(12, 11, 5, 4, 3, 2);






void setup()
{
  //Serial.begin(9600);

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

  WDTCTL = WDTPW + WDTHOLD;                 // Stop watchdog timer
  	//P1OUT = 0x00;                             // P1 setup for LED & reset output
  	P2DIR = BIT0 + BIT1 + BIT4;					// select GPIO controls for AFE031
  	P2OUT = BIT0;								// set P2.0 high (connect to ~CS pin 6 of AFE031)
  	P1SEL = BIT1 + BIT2 + BIT4;					// select SPI on USCIA
  	P1SEL2 = BIT1 + BIT2 + BIT4;
  	P1DIR &= ~BIT6;                           // P1.6/TA0.1 Input Capture
  	P1SEL |= BIT6;                            // TA0.1 option select
  	UCA0CTL1 |= UCSWRST;						// Start USCI initialization
  	UCA0CTL0 = UCCKPH + UCMSB + UCMST + UCSYNC;  // SPI mode 00, MSB first, 3-pin, 8-bit SPI master
  	UCA0CTL1 |= UCSSEL_2;                     // SPI use SMCLK
  	UCA0BR0 |= 0x02;                          // clock/2
  	UCA0BR1 = 0;                              //
  	UCA0MCTL = 0;                             // No modulation
  	UCA0CTL1 &= ~UCSWRST;                     // **Initialize USCI state machine**

  	_BIS_SR(GIE);  // Enable global interrupts

  	// Initialize AFE031 to default RX mode
  	REdge = 0;
  	AFE_RxMode();

}

void loop()
{
	char msgToSend[] = "Hello.";
		unsigned int incomingByte = 0x00;
		unsigned int byteRX = 0x00;
		//int incomingBit;
		int nextBitNum = 7;
		int wholeBits = 0;
		int remainder = 0;
		while(1){
			if (~RxOn){
				transmit(msgToSend);
			}

			__bis_SR_register(LPM0_bits + GIE);   // Enter LPM0
			__no_operation();                     // For debugger

			// on exiting LPMO
			if(RxOn) {
				if (prevType) { // we received an incoming bit
					switch(prevType) {
					case 17:
						wholeBits = prevCount / 14;
						remainder = prevCount % 14;
						if (remainder > 7) wholeBits ++;
						while (wholeBits > 0) {
							incomingByte &= ~(0x01 << nextBitNum);
							nextBitNum --;
							wholeBits --;
						}
						break;
					case 13:
						wholeBits = prevCount / 11;
						remainder = prevCount % 11;
						if (remainder > 5) wholeBits ++;
						while (wholeBits > 0) {
							incomingByte |= (0x01 << nextBitNum);
							nextBitNum --;
							wholeBits --;
						}
						break;
					default: break;
					}
					if (nextBitNum == 7) {
						if(prevCount) {
							byteRX = incomingByte;
							incomingByte = 0x00;
						}
					}
					prevType = 0; // reset prevType
					prevCount = 0; //reset prevCount
				}
			}
			else{ // we are not receiving right now
				nextBitNum = 7;
				incomingByte = 0x00;
			}


		}
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
  int userCredit = Credit(5);
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
  int userPower = UserPower();
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
  int sensorCalibrate = (batterySensor*voltageRef)/1023;
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
String ConnStatus(void) {
  String userStatus;
  boolean connection = digitalRead(CONNECTION_PIN);

  if(connection == true)
    userStatus = "Conn";
  else userStatus = "N/A";

  //return "status";
  return userStatus;
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

// Timer A1 interrupt service routine
#if defined(__TI_COMPILER_VERSION__) || defined(__IAR_SYSTEMS_ICC__)
#pragma vector=TIMER1_A0_VECTOR
#pragma vector=TIMER1_A1_VECTOR
__interrupt void Timer_A (void)
#elif defined(__GNUC__)
void __attribute__ ((interrupt(TIMER1_A0_VECTOR))) Timer_A (void)
#else
#error Compiler not supported!
#endif
{
  TXcount++;                            // increment the counter
  TA1IV = 0x00;
}

// TA0_A1 Interrupt vector
#if defined(__TI_COMPILER_VERSION__) || defined(__IAR_SYSTEMS_ICC__)
#pragma vector = TIMER0_A1_VECTOR
__interrupt void TIMER0_A1_ISR (void)
#elif defined(__GNUC__)
void __attribute__ ((interrupt(TIMER0_A1_VECTOR))) TIMER0_A1_ISR (void)
#else
#error Compiler not supported!
#endif
{
  //switch(__even_in_range(TA0IV,0x0A))
	if(TA0IV_TACCR1)
    {
      //case  TA0IV_NONE: break;              // Vector  0:  No interrupt
      //case  TA0IV_TACCR1:                   // Vector  2:  TACCR1 CCIFG
        if (TA0CCTL1 & CCI)                 // Capture Input Pin Status
        {
            PrevEdge = REdge;
            REdge = TA0CCR1;
            Period = REdge - PrevEdge;

            if(RxOn) { // we are receiving a character
            	if((Period > 53) && (Period < 64)){  // pulse is 17kHz
            		if(RXtype == 17){ // we are in the process of receiving a 17 kHz stream
            			RXcount ++;
            		}
            		else { // this is a character transition
            			prevType = RXtype;
            			RXtype = 17;
            			prevCount = RXcount;
            			RXcount = 0;
            			__bic_SR_register_on_exit(LPM0_bits + GIE);  // Exit LPM0 on return to main
            		}
            	}
            	else if ((Period > 70)&& (Period < 81)) { //pulse is 13kHz
            		if(RXtype == 13) { // we are in the process of receiving a 13 kHz stream
            			RXcount ++;
            		}
            		else { // this is a character transition
            			prevType = RXtype;
            			RXtype = 13;
            			prevCount = RXcount;
            			RXcount = 0;
            			__bic_SR_register_on_exit(LPM0_bits + GIE);  // Exit LPM0 on return to main
            		}
            	}
            	else if ((Period > 100) && (Period < 120)) { //pulse is 9 kHz - this is a termination
            		if (RXtype == 9) { // we are in the process of receiving a 9kHz stream
            			RxOn = 0;
            			RXcount = 0;
            		}
            		else{ // this is a character transition
            			prevType = RXtype;
            			RXtype = 9;
            			prevCount = RXcount;
            			RXcount = 0;
            			__bic_SR_register_on_exit(LPM0_bits + GIE);  // Exit LPM0 on return to main
            		}
            	}
            }

            else { //check to see if we are receiving the wakeup
            	if ((Period > 40) && (Period < 50)){ // pulse is 21 kHz
            		if (RXtype == 21) { // we are in the process of receiving a 21kHz stream
            			RXcount ++;
            			if (RXcount > 15) RxOn = 1;
            		}
            		else { // this is a new wakeup
            			RXtype = 21;
            			RXcount = 0;
            		}
            	}
            }
        }
        //break;
      //case TA0IV_TACCR2: break;             // Vector  4:  TACCR2 CCIFG
      //case TA0IV_6: break;                  // Vector  6:  Reserved CCIFG
      //case TA0IV_8: break;                  // Vector  8:  Reserved CCIFG
      //case TA0IV_TAIFG: break;              // Vector 10:  TAIFG
      //default: 	break;
    }
}

#pragma vector=TIMER0_A0_VECTOR
__interrupt void TIMER0_A0_ISR (void){
	TA0IV = 0x00;
}


/* Initialize non-used ISR vectors with a trap function */


#pragma vector=PORT2_VECTOR
#pragma vector=PORT1_VECTOR
//#pragma vector=TIMER1_A1_VECTOR
//#pragma vector=TIMER1_A0_VECTOR
//#pragma vector=TIMER0_A1_VECTOR
//#pragma vector=TIMER0_A0_VECTOR
#pragma vector=ADC10_VECTOR
#pragma vector=USCIAB0TX_VECTOR
#pragma vector=WDT_VECTOR
#pragma vector=USCIAB0RX_VECTOR
#pragma vector=NMI_VECTOR
#pragma vector=COMPARATORA_VECTOR

__interrupt void ISR_trap(void)
{

}
