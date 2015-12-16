/* 
 * File:   UART1lib.h
 * Author: Diego Blasco Quetglas
 *
 */

void UART1Init(unsigned char nbitsParity, unsigned char stopBits, unsigned int baudRate);
void UART1Send();
unsigned int UART1Receive();
char UART1DataReceived();
char UART1TBusy();
char UART1TBufferFull();