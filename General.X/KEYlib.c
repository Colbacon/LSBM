/* 
 * File:   KEYlib.c
 * Author: Diego Blasco Quetglas
 *
 * Created on 22 de septiembre de 2015, 18:38
 */
#include <p30F4011.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "KEYlib.h"
#include "delay.h"

//Column register configuration
#define COL0_CONF TRISBbits.TRISB0
#define COL1_CONF TRISBbits.TRISB1
#define COL2_CONF TRISBbits.TRISB2
//Row register configuration
#define ROW0_CONF TRISDbits.TRISD0
#define ROW1_CONF TRISDbits.TRISD1
#define ROW2_CONF TRISDbits.TRISD2
#define ROW3_CONF TRISDbits.TRISD3
//Column write register
#define COL0 LATBbits.LATB0
#define COL1 LATBbits.LATB1
#define COL2 LATBbits.LATB2
//Row read register
#define ROW0 PORTDbits.RD0
#define ROW1 PORTDbits.RD1
#define ROW2 PORTDbits.RD2
#define ROW3 PORTDbits.RD3

void KeyInit(){
    // Set columns as outputs
    COL0_CONF = 0; 
    COL1_CONF = 0;
    COL2_CONF = 0;

    //Set rows as inputs
    ROW0_CONF = 1; 
    ROW1_CONF = 1;
    ROW2_CONF = 1;
    ROW3_CONF = 1;

}

unsigned char getKey(){

    char value = -1;

    while(value == -1){
       value = getKeyNotBloking();
    }
    return value;
}

unsigned char getKeyNotBloking(){
    //Rows as inputs
    ROW0_CONF = 1; 
    ROW1_CONF = 1;
    ROW2_CONF = 1;
    ROW3_CONF = 1;
    
    //Check column 0
    COL0_CONF = 0;
    COL1_CONF = 1;
    COL2_CONF = 1;
    COL0 = 0;
    COL1 = 1;
    COL2 = 1;

    Delay5ms();
    Delay5ms();

    if(ROW0 == 0) return 0;
    if(ROW1 == 0) return 3;
    if(ROW2 == 0) return 6;
    if(ROW3 == 0) return 9;

    //Check column 1
    COL0_CONF = 1;
    COL1_CONF = 0;
    COL2_CONF = 1;
    COL0 = 1;
    COL1 = 0;
    COL2 = 1;

    Delay5ms();
    Delay5ms();

    if(ROW0 == 0) return 1;
    if(ROW1 == 0) return 4;
    if(ROW2 == 0) return 7;
    if(ROW3 == 0) return 10;

    //Check column 2
    COL0_CONF = 1;
    COL1_CONF = 1;
    COL2_CONF = 0;
    COL0 = 1;
    COL1 = 1;
    COL2 = 0;

    Delay5ms();
    Delay5ms();

    if(ROW0 == 0) return 2;
    if(ROW1 == 0) return 5;
    if(ROW2 == 0) return 8;
    if(ROW3 == 0) return 11;
    
    //Columns as inputs
    COL0_CONF = 1; 
    COL1_CONF = 1;
    COL2_CONF = 1;

    return -1;
}

//unsigned char * get_MULTIKEY_NONBLOCK(unsigned char values[]){
//
//    ROW0_CONF = 1; //Se inicializa como 1 configurandolo como input
//    ROW1_CONF = 1;
//    ROW2_CONF = 1;
//    ROW3_CONF = 1;
//
//    values = '\0';
//
//    COL0 = 0;
//    COL1 = 1;
//    COL2 = 1;
//
//    Delay5ms();
//    Delay5ms();
//
//    if(ROW0 == 1) strcat(values, '0');
//    if(ROW1 == 1) strcat(values, '3');
//    if(ROW2 == 1) strcat(values, '6');
//    if(ROW3 == 1) strcat(values, '9');
//
//    COL0 = 1;
//    COL1 = 0;
//    COL2 = 1;
//
//    Delay5ms();
//    Delay5ms();
//
//    if(ROW0 == 1) return 1;
//    if(ROW1 == 1) return 4;
//    if(ROW2 == 1) return 7;
//    if(ROW3 == 1) return 10;
//
//    COL0 = 1;
//    COL1 = 1;
//    COL2 = 0;
//
//    Delay5ms();
//    Delay5ms();
//
//    if(ROW0 == 1) return 2;
//    if(ROW1 == 1) return 5;
//    if(ROW2 == 1) return 8;
//    if(ROW3 == 1) return 11;
//
//    COL0_CONF = 1; //Se inicializa a 1 configurandolo como input
//    COL1_CONF = 1;
//    COL2_CONF = 1;
//
//    return -1;
//}
//
