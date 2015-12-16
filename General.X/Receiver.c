/*
 *	Description: Se encarga de recibir una serie de comandos mediante mensajes a través
 *  del bus CAN, decodificarlos y ejecutarlos sobre el LCD.
 *
 *	Author: Diego Blasco Quetglas
 */


#include <p30f4011.h>
#include <stdio.h>

#include "KEYlib.h"
#include "LCDlib.h"
#include "CANlib.h"
#include "codes.h"

/******************************************************************************/
/* Configuration words                                                        */
/******************************************************************************/
_FOSC(CSW_FSCM_OFF & EC_PLL16);
_FWDT(WDT_OFF);
_FBORPOR(MCLR_EN & PBOR_OFF & PWRT_OFF);
_FGS(CODE_PROT_OFF);

/******************************************************************************/
/* Hardware                                                                   */
/******************************************************************************/

/******************************************************************************/
/* Global Variable declaration                                                */
/******************************************************************************/

//Define several modes used for LCD control bounds
#define PRINT_CHAR_MODE     0
#define DELETE_CHAR_MODE    1
#define MOVE_LEFT_MODE      2
#define MOVE_RIGHT_MODE     3

unsigned char lock = 0; //0-wait 1-operate the data that has been received (CS: critical section)
int counter; //count the cursor position on the display area
//Message received variables
unsigned int DLC;  //number of data bytes
unsigned char data [8]; //data received

/******************************************************************************/
/* Interrupts                                                                 */
/******************************************************************************/

void _ISR _C1Interrupt(void){
    
    //Clear CAN global interrupt
    CANClearInt();
    
    if(CANRxInt()){
        //Clear Rx interrupt
        CANClearRxInt();
        //Read DLC and DATA
        DLC = CANReadRxMessageDLC();
        CANReadRxMessageData(data);
        //Clear Rx Buffer
        CANClearRxBuffer();
        
        lock = 1;
    }
}

/******************************************************************************/
/* Procedures                                                                 */
/******************************************************************************/

void LCDRightControl(unsigned char mode, char * ch);
void LCDLeftControl(unsigned char mode);

int main(void){
    
    int i;
    counter = 0;
    
    LCDInit();
    KeyInit();
    CANInit(NORMAL_MODE);
    
    while(1){
        
        while(lock == 0);
        
        for(i=0; i < DLC; i++){

            switch(data[i]){
                case NUMBER1_CODE : LCDRightControl(PRINT_CHAR_MODE, "1");    break;
                case NUMBER2_CODE : LCDRightControl(PRINT_CHAR_MODE, "2");    break;
                case NUMBER3_CODE : LCDRightControl(PRINT_CHAR_MODE, "3");    break;
                case NUMBER4_CODE : LCDRightControl(PRINT_CHAR_MODE, "4");    break;
                case NUMBER5_CODE : LCDRightControl(PRINT_CHAR_MODE, "5");    break;
                case DELETE_LAST_CHAR_CODE : LCDLeftControl(DELETE_CHAR_MODE); break;
                case MOVE_FIRST_LINE_CODE : LCDMoveFirstLine(); counter = 0; break;
                case CLEAR_CODE : LCDClear(); LCDMoveHome(); counter = 0;  break;
                case MOVE_LEFT_CODE : LCDLeftControl(MOVE_LEFT_MODE); break;
                case MOVE_SECOND_LINE_CODE: LCDMoveSecondLine(); counter = 16; break;
                case MOVE_RIGHT_CODE : LCDRightControl(MOVE_RIGHT_MODE, '\0'); break;
            }
        }
        
        lock = 0;
    }
    
    return 0;
}
//Treat not to go out of the display area from de left and operate 
//delete char or move to the left
void LCDLeftControl(unsigned char mode){
    int i;
    
    if(counter != 0){  //First position of DLC first line
        if(counter == 16){ //PFirst position of DLC second line
            LCDMoveFirstLine();
            for(i=0;i<15;i++){
                LCDMoveRight();
            }
        }else{  //another position of visible space of the DLC
            LCDMoveLeft();
        }
        if(mode == DELETE_CHAR_MODE){
            LCDPrint(" ");
            LCDMoveLeft();
        }
        counter--;
    }                    
}
//Treat not to go out of the display area from de right and operate 
//print char or move to the right
void LCDRightControl(unsigned char mode, char * ch){
    //EIn both cases, treat not to get out of the display area
    if(counter < 31){
        if (mode == PRINT_CHAR_MODE){
            LCDPrint(ch);
        }else{
            LCDMoveRight();
        }
        counter++;
    }
    if(counter == 16) LCDMoveSecondLine();
}
