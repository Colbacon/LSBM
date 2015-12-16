/* 
 * File:   Timers.c
 * Author: Diego Blasco Quetglas
 *
 * Created on 26 de octubre de 2015
 */

#include <p30f4011.h>

#include "TIMERlib.h"

void Timer1Init(unsigned int period, unsigned char pscaler, unsigned char priority){
    //Configurate timer1 control register
    T1CON = 0x0000;     //timer stop, non pscaler and internal clock
    T1CONbits.TCKPS = pscaler;
    
    TMR1 = 0x0000;      //Clean counter register
    PR1 = period;       //value to achieve
    
    //configurate interruptions
    IEC0bits.T1IE = 1;  //activate timer1 interrupt
    IFS0bits.T1IF = 0;  //clean status flag
    IPC0bits.IC1IP = priority;  //priority
}

void Timer1Start(){
    //activate timer
    T1CONbits.TON = 1;
}

void Timer1Stop(){
    //stop timer
    T1CONbits.TON = 0;
}

void Timer1Reset(){
    //stop timer and reset counter register
    T1CONbits.TON = 0;
    TMR1 = 0x0000;
}

void Timer1Reinit(unsigned int period, unsigned char pscaler, unsigned char priority){
    //reset timer, reconfigure, and start
    Timer1Reset();
    Timer1Init(period, pscaler, priority);
    Timer1Start();
}

void Timer1ClearInt(){
    //clean status flag
    IFS0bits.T1IF = 0;
}