/*
 *	Description: Permite seleccionar una serie de comandos asociados a la matriz de teclado,
 *  los cuales se codifican y envían mediante mensajes a través del bus CAN.
 *  Los comandos seleccionados se mostrarán mediante el LCD hasta que estos sean
 *  enviados. 
 *
 *	Author: Diego Blasco Quetglas
 */


#include <p30f4011.h>
#include <stdio.h>
#include <string.h>

#include "KEYlib.h"
#include "LCDlib.h"
#include "CANlib.h"
#include "delay.h"

#include"codes.h"

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
#define SPACE 0x20  //SPACE ascii code hex representation

int count; //count number of commands that have been selected
unsigned char commands[8]; //commands codificated
unsigned char characters[8]; //characters that represent the commands
char line1[20]; //buffer of DLC first line
char line2[20]; //buffer of DLC second line
    
/******************************************************************************/
/* Interrupts                                                                 */
/******************************************************************************/

/******************************************************************************/
/* Procedures                                                                 */
/******************************************************************************/

void saveCommand(unsigned char command, unsigned char character);
void printCommands();
void sendCommands();
void createCharacters();

int main(void){
    
    int i;
    count = 0;
    
    KeyInit();
    CANInit(NORMAL_MODE);
    LCDInit();
    
    createCharacters(); //create two news characters
    //Fill with spaces both buffers (for correctly show on the display in the second case)
    memset(commands, SPACE, sizeof commands); 
    memset(characters, SPACE, sizeof characters);
    //print actual commands on the DLC
    printCommands();
    
    while(1){
 
        switch(getKey()){
            case 0: saveCommand(NUMBER1_CODE, '1'); break;
            case 1: saveCommand(NUMBER2_CODE, '2'); break;
            case 2: saveCommand(NUMBER3_CODE, '3'); break;
            case 3: saveCommand(NUMBER4_CODE, '4'); break;
            case 4: saveCommand(NUMBER5_CODE, '5'); break;
            case 5: sendCommands(); break;
            case 6: saveCommand(DELETE_LAST_CHAR_CODE, 'R'); break;
            case 7: saveCommand(MOVE_FIRST_LINE_CODE, CHAR1_CODE); break;
            case 8: saveCommand(CLEAR_CODE, 0x43); break;
            case 9: saveCommand(MOVE_LEFT_CODE, 0x7F); break;
            case 10: saveCommand(MOVE_SECOND_LINE_CODE, CHAR2_CODE); break;
            case 11: saveCommand(MOVE_RIGHT_CODE, 0x7E); break;
        }
        
        for(i=0; i<60; i++) Delay5ms();
        
    }
    return 0;
}
//Save the code command and his associated character
void saveCommand(unsigned char command, unsigned char character){
    
    if(count < 8){
        commands[count] = command;
        characters[count] = character;
                
        printCommands();
        count++;
    }
}
//print commands selected until the moment with the following format
void printCommands(){
    
    sprintf(line1, "1:%c 2:%c 3:%c 4:%c", characters[0], characters[1], characters[2], characters[3]);
    LCDMoveFirstLine();
    LCDPrint(line1);
    
    sprintf(line2, "5:%c 6:%c 7:%c 8:%c ", characters[4], characters[5], characters[6], characters[7]);
    LCDMoveSecondLine();
    LCDPrint(line2);  
   
}
//Send commands, clear buffer and reinit message of the DLC
void sendCommands(){
    
    CANSendMessage(count, commands);
    
    memset(commands, SPACE, sizeof commands);
    memset(characters, SPACE, sizeof characters);
    printCommands();
    
    count = 0;
}
//Create up and down arrows characters
void createCharacters(){
    
    struct MyChar ch;
    //up arrow
    ch.addr = CHAR1_ADDR;
    ch.pattern[0] = 0x00;
    ch.pattern[1] = 0x04; 
    ch.pattern[2] = 0x0E; 
    ch.pattern[3] = 0x15; 
    ch.pattern[4] = 0x04; 
    ch.pattern[5] = 0x04; 
    ch.pattern[6] = 0x00; 
    ch.pattern[7] = 0x00; 
    
    LCDStoreCharPattern(ch);
    //sown arrow
    ch.addr = CHAR2_ADDR;
    ch.pattern[0] = 0x00;
    ch.pattern[1] = 0x04; 
    ch.pattern[2] = 0x04; 
    ch.pattern[3] = 0x15; 
    ch.pattern[4] = 0x0E; 
    ch.pattern[5] = 0x04; 
    ch.pattern[6] = 0x00; 
    ch.pattern[7] = 0x00; 
    
    LCDStoreCharPattern(ch);
}