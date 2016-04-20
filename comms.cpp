/*
 * comms.c
 *
 *  Created on: Apr 20, 2016
 *      Author: Mandy
 */

#include <LiquidCrystal.h>
#include <stdlib.h>
#include <pins_energia.h>
#include <msp430.h>
#include "comms.h"

volatile int TXcount;
volatile int RXcount;
volatile int RXtype;
volatile int RxOn;
volatile int PrevEdge;
volatile int REdge;
volatile int Period;
volatile int prevCount;
volatile int prevType;



void SetSPI(char addr, char setting) { // SPI command for writing to AFE031
	P2OUT &= ~BIT0;						// Drive ~CS low to select AFE031 on SPI
	UCA0TXBUF = addr;
	while (!(IFG2 & UCA0TXIFG));						// wait for SPI TX buffer to be ready for next char
	UCA0TXBUF = setting;
	while (!(IFG2 & UCA0TXIFG));					// wait for SPI TX buffer to be ready for next char
	P2OUT |= BIT0;
	__delay_cycles(10);	  // give AFE031 time to process and store the previous command
}

int CheckSPI(char addr, char setting){      // SPI command for checking the value on AFE031
	// returns 1 if SPI setting is matched, otherwise 0
	int theValue;
	P2OUT &= ~BIT0;						// Drive ~CS low to select AFE031 on SPI
	UCA0TXBUF = (addr | READ);
	while (!(IFG2 & UCA0TXIFG));						// wait for SPI TX buffer to be ready for next char
	UCA0TXBUF = setting;
	while (!(IFG2 & UCA0TXIFG));					// wait for SPI TX buffer to be ready for next char
	P2OUT |= BIT0;
	__delay_cycles(10);	  // give AFE031 time to process and store the previous command

	P2OUT &= ~BIT0;						// Drive ~CS low to select AFE031 on SPI
	UCA0TXBUF = (addr | READ);
	while (!(IFG2 & UCA0TXIFG));						// wait for SPI TX buffer to be ready for next char
	UCA0TXBUF = setting;
	while (!(IFG2 & UCA0TXIFG));					// wait for SPI TX buffer to be ready for next char
	P2OUT |= BIT0;
	if (UCA0RXBUF == setting)
		theValue = 0x01;
	else
		theValue = 0x00;
	return theValue;
}

void AFE_TxMode(void) { // sets up Tx Mode for the MCU and the AFE031
	TA0CCTL1 &= ~CCIE; // turn off the interrupt used in RX mode
	int AFEvalid;
	AFEvalid = 0x00;
	while (AFEvalid !=1) {   // set ENABLE1 and verify that the setting is correct
		SetSPI(ENABLE1, EN1_TX_PA);
		AFEvalid = CheckSPI(ENABLE1, EN1_TX_PA);

	}

	SetSPI(ENABLE2, EN2_PAon);
	SetSPI(GAINSELECT, GS_TX);
}

void AFE_RxMode(void) { // sets up RX mode for the MCU and the AFE031
	SetSPI(CONTROL1, C1_SETTING);
	SetSPI(CONTROL2, C2_SETTING);
	SetSPI(ENABLE1, EN1_RX);
	SetSPI(ENABLE2, EN2_PAoff);
	SetSPI(GAINSELECT, GS_RX_max);

	TA0CCTL1 = CAP + CM_1 + CCIE + SCS + CCIS_0; // TA0CCR1 Capture mode; CCI1A; Rising edge; interrupt enable
	TA0CTL |= TASSEL_2 + MC_2 + TACLR;        // SMCLK, Cont Mode; start timer
}

int len(char* s){
    if(!s) return 0;
    char* ofs=s;
    while(*s++);
    return s-ofs-1;
}

void sendByte(char dataByte){
	int j = 0;
	for(j=0; j < 8; j++){
		if (dataByte & 0x80){
			//data bit is a 1 - send 11 pulses of 13kHz PWM on P2.2
			TXcount = 0;
			while(TXcount < 11){
				P2DIR |= 0x04;                            // P2.2 output
				P2SEL |= 0x04;                            // P2.2 TA1/2 options
				TA1CCR0 = 76;                               // PWM Period/2
				TA1CCTL1 = OUTMOD_7;                 		// CCR1 reset/set
				TA1CCR1 = 38;                                // CCR1 PWM duty cycle 50%
				TA1CTL = TASSEL_2 + MC_1 + TAIE;                  // SMCLK, up mode, TAIFG interrupt enable
			}
			TA1CTL = MC_0;     //turn off timer A1

		}
		else{
			// data bit is a 0 - send 14 pulses of 17kHz PWM
			TXcount = 0;
			while(TXcount < 14){
				P2DIR |= 0x04;                            // P2.2 output
				P2SEL |= 0x04;                            // P2.2 TA1/2 options
				TA1CCR0 = 58;                               // PWM Period/2
				TA1CCTL1 = OUTMOD_7;                 	// CCR1 reset/set
				TA1CCR1 = 29;                                // CCR1 PWM duty cycle 50%
				TA1CTL = TASSEL_2 + MC_1 + TAIE;                  // SMCLK, up mode, TAIFG interrupt enable

			}
			TA1CTL = MC_0;        // turn off timer A1

		}
		dataByte = dataByte << 1;
	}

}

void sendWake(void) {
	TXcount = 0;
	while(TXcount < 21){ // wakeup signal is 21 pulses of 21kHz
		P2DIR |= 0x04;                            // P2.2 output
		P2SEL |= 0x04;                            // P2.2 TA1/2 options
		TA1CCR0 = 46;                               // PWM Period/2
		TA1CCTL1 = OUTMOD_7;                 	// CCR1 reset/set
		TA1CCR1 = 23;                                // CCR1 PWM duty cycle 50%
		TA1CTL = TASSEL_2 + MC_1 + TAIE;                  // SMCLK, up mode, TAIFG interrupt enable
	}
	TA1CTL = MC_0;        // turn off timer A1
}

void sendTerm(void) {
	TXcount = 0;
	while(TXcount < 5){ // termination of message is 5 pusles of 9kHz
		P2DIR |= 0x04;                            // P2.2 output
		P2SEL |= 0x04;                            // P2.2 TA1/2 options
		TA1CCR0 = 110;                               // PWM Period/2
		TA1CCTL1 = OUTMOD_7;                 	// CCR1 reset/set
		TA1CCR1 = 55;                                // CCR1 PWM duty cycle 50%
		TA1CTL = TASSEL_2 + MC_1 + TAIE;                  // SMCLK, up mode, TAIFG interrupt enable
	}
	TA1CTL = MC_0;        // turn off timer A1
}

void transmit(char theMessage[]){
	// check to see if we are actively receiving - if yes, wait until no
	while (RxOn);     // wait until we are done with Rx mode
	// turn off CCR interrupt used for Rx mode

	// Change AFE031 from Rx mode to Tx mode
	AFE_TxMode();

	// send wakeup - 21 pulses of 21kHz PWM
	sendWake();

	// count length of theMessage and send it
	int size = len(theMessage);
	int i = 0;
	for (i=0; i < size; i++) {
		sendByte(theMessage[i]);
	}
	// send termination - 5 pulses of 9 kHz PWM
	sendTerm();

	// Change AFE031 from Tx mode to Rx mode
	AFE_RxMode();

	// turn on CCR interrupt used for Rx mode
}
