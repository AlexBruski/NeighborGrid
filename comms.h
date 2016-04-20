/*
 * comms.h
 *
 *  Created on: Apr 20, 2016
 *      Author: Mandy
 */

#ifndef COMMS_H_
#define COMMS_H_

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
#define READ 0x80

// global variables
extern volatile int TXcount;
extern volatile int RXcount;
extern volatile int RXtype;
extern volatile int RxOn;
extern volatile int PrevEdge;
extern volatile int REdge;
extern volatile int Period;
extern volatile int prevCount;
extern volatile int prevType;


/* Prototypes for the functions */

void SetSPI(char addr, char setting); // SPI command for writing to AFE031

int CheckSPI(char addr, char setting);    // SPI command for checking the value on AFE031

void AFE_TxMode(void); // sets up Tx Mode for the MCU and the AFE031

void AFE_RxMode(void); // sets up RX mode for the MCU and the AFE031

int len(char* s); // find the length of a null terminated string

void sendByte(char dataByte);

void sendWake(void); // send the wakeup signal to the modem on the other end

void sendTerm(void) ; // send the termination signal to the modem on the other end

void transmit(char theMessage[]); // transmit a string to the modem on the other end

#endif /* COMMS_H_ */
