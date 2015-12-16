
#include "CADlib.h"
#include <p30f4011.h>

void CADInit(){
//    // Set PIN B7 (AN7) as input
//	TRISBbits.TRISB7 = 1;
    
    ADPCFG = 0xFF7F;            //Select port AN7 as analog input
    ADCON1 = 0x0000;            //Manual sample, data output -> integer
    ADCON1bits.SSRC = 0b111;    //Conversion trigger -> autoconversion
    
    ADCON2 = 0x0000;            //Voltage reference as internal (AVdd, AVss)
    
    ADCON3 = 0x0003;            //Tad = internal 2Tcy
    
    ADCHS = 0x0007;             //Connect AN7 as CH0 input
    ADCSSL = 0;                 //Not to alternative scan
    
    ADCON1bits.ADON = 1;        //Turn on CAD module
    
//    ADCON2bits.VCFG = 0b000;    //Select voltage reference as internal (AVdd, AVss)
//    ADCON3bits.ADCS = 0x03;     //Set analog conversion clock to 2Tcy
//    ADCON2bits.CHPS = 0b00;     //Converts channel 0 
//    ADCHS = 0x0007;             //Select AN7
//    ADCON1bits.SSRC = 0b111;    //Conversion trigger -> autoconversion
//    ADCON1bits.ASAM = 0;        //Sample autostart -> disabled
//    ADCON1bits.FORM = 0b00;     //Data output format -> integer
//    ADCSSL = 0;                 //Not to alternative scan
//    ADCON1bits.ADON = 1;        //Turn on CAD module
}

unsigned int CADGetValue(){
    int i; 
    
    ADCON1bits.SAMP = 1;            //Start sampling
    for(i=0; i<20; i++);            //samplng 100ms
    ADCON1bits.SAMP = 0;            //Start converting
    while(!ADCON1bits.DONE);    
    return ADCBUF0;
}