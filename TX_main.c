/* --COPYRIGHT--,BSD_EX
 * Copyright (c) 2012, Texas Instruments Incorporated
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * *  Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *
 * *  Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * *  Neither the name of Texas Instruments Incorporated nor the names of
 *    its contributors may be used to endorse or promote products derived
 *    from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
 * THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
 * OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
 * OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
 * EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 *******************************************************************************
 *
 *                       MSP430 CODE EXAMPLE DISCLAIMER
 *
 * MSP430 code examples are self-contained low-level programs that typically
 * demonstrate a single peripheral function or device feature in a highly
 * concise manner. For this the code may rely on the device's power-on default
 * register values and settings such as the clock configuration and care must
 * be taken when combining code from several examples to avoid potential side
 * effects. Also see www.ti.com/grace for a GUI- and www.ti.com/msp430ware
 * for an API functional library-approach to peripheral configuration.
 *
 * --/COPYRIGHT--*/
//******************************************************************************
//  MSP430G2xx3 Demo - Timer_A, PWM TA1, Up/Down Mode, DCO SMCLK
//
//  Description: This program generates one PWM output on P1.2 using
//  Timer_A configured for up/down mode. The value in CCR0, 128, defines the PWM
//  period/2 and the value in CCR1 the PWM duty cycles.
//  A 75% duty cycle is on P1.2.
//  SMCLK = MCLK = TACLK = default DCO
//
//               MSP430G2xx3
//            -----------------
//        /|\|              XIN|-
//         | |                 |
//         --|RST          XOUT|-
//           |                 |
//           |         P1.2/TA1|--> CCR1 - 75% PWM
//
//  D. Dang
//  Texas Instruments, Inc
//  December 2010
//   Built with CCS Version 4.2.0 and IAR Embedded Workbench Version: 5.10
//******************************************************************************

// some definitions for AFE031 registers and commands
#define ENABLE1 0x01
#define GAINSELECT 0x02
#define ENABLE2 0x03
#define CONTROL1 0x04
#define CONTROL2 0x05
#define EN1_TX_PA 0x03
#define EN1_TX_noPA 0x02
#define EN1_RX 0x04
#define GS_TX 0x30
#define GS_RX_max 0x0F
#define GS_RX_min 0x00
#define GS_RX_1 0x02
#define GX_RX_32 0x0B
#define EN2_PAon 0x08
#define EN2_PAoff 0x00
#define C1_SETTING 0x00
#define C2_SETTING 0x00


#include <msp430.h>

volatile int count;
//volatile int ready;
volatile int RxOn;

void AFE_TxMode(void) {
	//IE2 |= UCA0TXIE;                          // Enable USCI0 TX interrupt
	P2OUT &= ~BIT0;						// Drive ~CS low to select AFE031 on SPI
	//ready = 0;
	UCA0TXBUF = ENABLE1;
	while (!(IFG2 & UCA0TXIFG));						// wait for SPI TX buffer to be ready for next char
	//ready = 0;
	UCA0TXBUF = EN1_TX_PA;
	while (!(IFG2 & UCA0TXIFG));					// wait for SPI TX buffer to be ready for next char
	P2OUT |= BIT0;
	__delay_cycles(10);					// give AFE031 time to process and store the previous command

	P2OUT &= ~BIT0;						// Drive ~CS low to select AFE031 on SPI
	//ready = 0;
	UCA0TXBUF = ENABLE2;
	while (!(IFG2 & UCA0TXIFG));						// wait for SPI TX buffer to be ready for next char
	//ready = 0;
	UCA0TXBUF = EN2_PAon;
	while (!(IFG2 & UCA0TXIFG));					// wait for SPI TX buffer to be ready for next char
	P2OUT |= BIT0;
	__delay_cycles(10);					// give AFE031 time to process and store the previous command

	P2OUT &= ~BIT0;						// Drive ~CS low to select AFE031 on SPI
	//ready = 0;
	UCA0TXBUF = GAINSELECT;
	while (!(IFG2 & UCA0TXIFG));						// wait for SPI TX buffer to be ready for next char
	//ready = 0;
	UCA0TXBUF = GS_TX;
	while (!(IFG2 & UCA0TXIFG));						// wait for SPI TX buffer to be ready for next char
	P2OUT |= BIT0;
	__delay_cycles(10);					// give AFE031 time to process and store the previous command

	//IE2 &= ~UCA0TXIE;                          // Disable USCI0 TX interrupt
}

void AFE_RxMode(void) {
	//IE2 |= UCA0TXIE;                          // Enable USCI0 TX interrupt
	P2OUT &= ~BIT0;						// Drive ~CS low to select AFE031 on SPI
	//ready = 0;
	UCA0TXBUF = CONTROL1;
	while (!(IFG2 & UCA0TXIFG));						// wait for SPI TX buffer to be ready for next char
	//ready = 0;
	UCA0TXBUF = C1_SETTING;
	while (!(IFG2 & UCA0TXIFG));						// wait for SPI TX buffer to be ready for next char
	P2OUT |= BIT0;
	__delay_cycles(10);					// give AFE031 time to process and store the previous command

	P2OUT &= ~BIT0;						// Drive ~CS low to select AFE031 on SPI
	//ready = 0;
	UCA0TXBUF = CONTROL2;
	while (!(IFG2 & UCA0TXIFG));						// wait for SPI TX buffer to be ready for next char
	//ready = 0;
	UCA0TXBUF = C2_SETTING;
	while (!(IFG2 & UCA0TXIFG));						// wait for SPI TX buffer to be ready for next char
	P2OUT |= BIT0;
	__delay_cycles(10);					// give AFE031 time to process and store the previous command

	P2OUT &= ~BIT0;						// Drive ~CS low to select AFE031 on SPI
	//ready = 0;
	UCA0TXBUF = ENABLE1;
	while (!(IFG2 & UCA0TXIFG));						// wait for SPI TX buffer to be ready for next char
	//ready = 0;
	UCA0TXBUF = EN1_RX;
	while (!(IFG2 & UCA0TXIFG));						// wait for SPI TX buffer to be ready for next char
	P2OUT |= BIT0;
	__delay_cycles(10);					// give AFE031 time to process and store the previous command

	P2OUT &= ~BIT0;						// Drive ~CS low to select AFE031 on SPI
	//ready = 0;
	UCA0TXBUF = ENABLE2;
	while (!(IFG2 & UCA0TXIFG));						// wait for SPI TX buffer to be ready for next char
	//ready = 0;
	UCA0TXBUF = EN2_PAoff;
	while (!(IFG2 & UCA0TXIFG));						// wait for SPI TX buffer to be ready for next char
	P2OUT |= BIT0;
	__delay_cycles(10);					// give AFE031 time to process and store the previous command

	P2OUT &= ~BIT0;						// Drive ~CS low to select AFE031 on SPI
	//ready = 0;
	UCA0TXBUF = GAINSELECT;
	while (!(IFG2 & UCA0TXIFG));						// wait for SPI TX buffer to be ready for next char
	//ready = 0;
	UCA0TXBUF = GS_TX;
	while (!(IFG2 & UCA0TXIFG));						// wait for SPI TX buffer to be ready for next char
	P2OUT |= BIT0;
	__delay_cycles(10);					// give AFE031 time to process and store the previous command

	//IE2 &= ~UCA0TXIE;                          // Disable USCI0 TX interrupt
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
			count = 0;
			while(count < 11){
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
			count = 0;
			while(count < 14){
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

void transmit(char theMessage[]){
	// check to see if we are actively receiving - if yes, wait until no
	while (RxOn);     // wait until we are done with Rx mode
	// turn off CCR interrupt used for Rx mode

	// Change AFE031 from Rx mode to Tx mode
	AFE_TxMode();

	// count length of theMessage
	int size = len(theMessage);
	int i = 0;
	for (i=0; i < size; i++) {
		sendByte(theMessage[i]);
	}
	// send termination
	sendByte(0xFF);

	// Change AFE031 from Tx mode to Rx mode
	AFE_RxMode();

	// turn on CCR interrupt used for Rx mode
}


int main(void)
{

	WDTCTL = WDTPW + WDTHOLD;                 // Stop watchdog timer
	//P1OUT = 0x00;                             // P1 setup for LED & reset output
	P2DIR = BIT0 + BIT1 + BIT4;					// select GPIO controls for AFE031
	P2OUT = BIT0;								// set P2.0 high (connect to ~CS pin 6 of AFE031)
	P1SEL = BIT1 + BIT2 + BIT4;					// select SPI on USCIA
	P1SEL2 = BIT1 + BIT2 + BIT4;
	P1DIR &= ~BIT6;                           // P1.6/TA0.1 Input Capture
	P1SEL |= BIT6;                            // TA0.1 option select
	UCA0CTL0 |= UCCKPL + UCMSB + UCMST + UCSYNC;  // SPI mode 00, MSB first, 3-pin, 8-bit SPI master
	UCA0CTL1 |= UCSSEL_2;                     // SPI use SMCLK
	UCA0BR0 |= 0x02;                          // clock/2
	UCA0BR1 = 0;                              //
	UCA0MCTL = 0;                             // No modulation
	UCA0CTL1 &= ~UCSWRST;                     // **Initialize USCI state machine**

	_BIS_SR(GIE);  // Enable global interrupts

	// Initialize AFE031 to default RX mode
	AFE_RxMode();


	char msgToSend[] = "Hello.";
	while(1){
		transmit(msgToSend);
		__delay_cycles(5000);
  }


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
  count++;                            // increment the counter
  TA1IV = 0x00;
}

/* Initialize non-used ISR vectors with a trap function */


#pragma vector=PORT2_VECTOR
#pragma vector=PORT1_VECTOR
//#pragma vector=TIMER1_A1_VECTOR
//#pragma vector=TIMER1_A0_VECTOR
#pragma vector=TIMER0_A1_VECTOR
#pragma vector=TIMER0_A0_VECTOR
#pragma vector=ADC10_VECTOR
#pragma vector=USCIAB0TX_VECTOR
#pragma vector=WDT_VECTOR
#pragma vector=USCIAB0RX_VECTOR
#pragma vector=NMI_VECTOR
#pragma vector=COMPARATORA_VECTOR

__interrupt void ISR_trap(void)
{

}
