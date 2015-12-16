#include <p30f4011.h>

/******************************************************************************/
/* Pre-processor directives                                                   */
/******************************************************************************/

/******************************************************************************/
/* Prototypes of additional functions										 */
/******************************************************************************/

/******************************************************************************/
/* Functions																  */
/******************************************************************************/

void CANInit(unsigned char operationalMode){
    
    C1CTRLbits.REQOP = 0b100; // Set configuration mode
    while(C1CTRLbits.OPMODE != 0b100); // Wait until configuration mode
    
    C1CTRLbits.CANCKS = 1; // FCAN = FCY
    
    /* BTR */
    // BTR1
    C1CFG1bits.BRP = 0; // 1Mbps
    C1CFG1bits.SJW = 0; // 1 TQ
    
    // BTR2
    C1CFG2bits.PRSEG = 0; // 1 TQs
    C1CFG2bits.SEG1PH = 3; // 4 TQs
    C1CFG2bits.SEG2PH = 1; // 2 TQs
    
    /* Tx buffer 0 */
    C1TX0CONbits.TXREQ = 0; // Clear tx request
    
    /* Rx buffer 0 */
    C1RX0CONbits.RXFUL = 0; // Clear rx status
    
    // Disable double buffer
    C1RX0CONbits.DBEN = 0;
    
    // Configure acceptance masks
    C1RXM0SIDbits.SID = 0; // No bits to compare
    C1RXM0SIDbits.MIDE = 1; // Determine as EXIDE
    
    // Configure acceptance filters
    C1RXF0SIDbits.SID = 0; // Doesn't matter the value as mask is ?0?
    C1RXF0SIDbits.EXIDE = 0; // Enable filter for standard identifier
    
    /* Interrupts */
    IEC1bits.C1IE = 1; // Enable global CAN interrupt
    IFS1bits.C1IF = 0; // Clear global CAN interrupt flag
    
    // Enable Configure interrupts
    C1INTE = 0;
    C1INTEbits.RX0IE = 1; // Enable interrupt associated to rx buffer 0
    C1INTFbits.RX0IF = 0; // Clear interrupt flag associated to rx buffer 0
    
    C1CTRLbits.REQOP = operationalMode; // Set normal/loopback mode
    while(C1CTRLbits.OPMODE != operationalMode); // Wait until normal/loopback mode
}


/**
 *  TRANSMISSION FUNCTIONS
 */
void CANSendMessage(unsigned char dataLength, unsigned char * data){
    
    unsigned char i;
    
    /* Tx buffer 0 */
    C1TX0DLCbits.TXRTR = 0; // Set data frame
    C1TX0SIDbits.TXIDE = 0; // Set standard identifier
    
    // Set identifier 1
    C1TX0SIDbits.SID5_0 = 1; // Set the lowest 6 bits   //posible 0
    C1TX0SIDbits.SID10_6 = 0; // Set the highest 5 bits  //posible 1
    C1TX0DLCbits.DLC = dataLength; // DLC 1-8 bytes
    
    //Fill Tx Buffer 0 with data
    for(i = 0; i < dataLength; i++){
        *((unsigned char *)&C1TX0B1 + i)= data[i];
    }
    //C1TX0B1 = byte_to_send; // Set data
    C1TX0CONbits.TXREQ = 1; // Transmit
    while(C1TX0CONbits.TXREQ != 0); // Wait for transmission
}

/**
 * RECEPTION FUNCTIONS
 */

unsigned int CANReadRxMessageSID(){
    return C1RX0SIDbits.SID;
}

unsigned int CANReadRxMessageDLC(){
    return C1RX0DLCbits.DLC;
}

void CANReadRxMessageData(unsigned char * data){
    
    unsigned char i;
    unsigned char dlc;
    
    dlc = C1RX0DLCbits.DLC;
    
    for(i=0; i < dlc; i++){
        switch(i){
            case 0: data[0] = C1RX0B1 & 0x00FF; break;
            case 1: data[1] = (C1RX0B1 & 0xFF00) >> 8; break;
            case 2: data[2] =  C1RX0B2 & 0x00FF; break;
			case 3: data[3] = (C1RX0B2 & 0xFF00) >> 8; break;
			case 4: data[4] =  C1RX0B3 & 0x00FF; break;
			case 5: data[5] = (C1RX0B3 & 0xFF00) >> 8; break;
			case 6: data[6] =  C1RX0B4 & 0x00FF; break;
			case 7: data[7] = (C1RX0B4 & 0xFF00) >> 8; break;
        }
    }
}

//Clear reception buffer
void CANClearRxBuffer(){
    C1RX0CONbits.RXFUL = 0;
}


/**
 * INTERRUPT MANAGEMENT FUNCTIONS
 */
//Get true if reception interruption flag is activated
unsigned char CANRxInt(){
    return (C1INTFbits.RX0IF == 1);	
}
//Clear reception interruption flag
void CANClearRxInt(){
    C1INTFbits.RX0IF = 0;
}
//Clear global interruption
void CANClearInt(){
    IFS1bits.C1IF = 0;
}

