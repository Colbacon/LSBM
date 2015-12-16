

#include <stdio.h>
#include <stdlib.h>
#include "LCDlib.h"
#include "CADlib.h"
//#include "libCAD.h"
#include "delay.h"

int main() {
    int i;
    char buffer [50];
    LCDInit();
    CADInit();
//    CADInit(CAD_INTERACTION_BY_INTERRUPT,5);
//    CADStart(5);
    while (1){
        sprintf(buffer, "%u", CADGetValue());
        LCDPrint(buffer);
        for(i=0; i<60; i++) Delay5ms();
        LCDClear();
        LCDMoveHome();
    }
    return 0;
}

