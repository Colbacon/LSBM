#include <p30f4011.h>
#include "UART1lib.h"

void UART1Init(unsigned char nbitsParity, unsigned char stopBits, unsigned int baudRate){
    
    U1MODE = 0x0000;
    U1MODEbits.UARTEN = 1;              //UART enabled
    U1MODEbits.ALTIO = 1;               //use alternate I/O
    U1MODEbits.PDSEL = nbitsParity;     //set paraty and data bits
    U1MODEbits.STSEL = stopBits;        //set stop bits
    
    U1STA = 0x0000;
    U1STAbits.UTXEN = 1;                //enable transmit
    
    U1BRG = baudRate;                   //set baud rate generation
}

void UART1end(unsigned int data){
    if(U1MODEbits.PDSEL = 0b11){ //9-bits data
        U1TXREG = data;
    }else{
        U1TXREG = data & 0x00FF;
    }
}

unsigned int UART1Receive(){
    if(U1MODEbits.PDSEL = 0b11){ //9-bits data
        return U1TXREG;
    }else{
        return U1TXREG & 0x00FF;
    }
}

char UART1DataReceived(){
    return U1STAbits.URXDA;
}

char UART1TBusy(){
    return !U1STAbits.TRMT;
}

char UART1TBufferFull(){
    return U1STAbits.UTXBF;
}