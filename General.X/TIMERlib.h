/* 
 * File:   Timers.h
 * Author: Diego Blasco Quetglas
 *
 * Created on 26 de octubre de 2015
 */

//Values of pscaler
#define TIMER_PSCALER_1_1   0b00
#define TIMER_PSCALER_1_8   0b01
#define TIMER_PSCALER_1_64  0b10
#define TIMER_PSCALER_1_256 0b11

//Pscaler and number of bits to count 1ms
#define TIMER_PSCALER_FOR_1ms   TIMER_PSCALER_1_1
#define TIMER_PERIOD_FOR_1ms    10000

void Timer1Init(unsigned int period, unsigned char pscaler, unsigned char priority);
void Timer1Start();
void Timer1Stop();
void Timer1Reset();
void Timer1Reinit(unsigned int period, unsigned char pscaler, unsigned char priority);
void Timer1ClearInt();