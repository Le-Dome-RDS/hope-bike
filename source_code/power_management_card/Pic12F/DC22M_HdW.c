/*
 * File:   DC24_Hdw.c
 * Author: francois
 *
 * Created on 31 octobre 2014, 18:17
 */

#include <xc.h>
#include "Type.h"
#include "PrjDefine.h"

/**************************************************************
 * Hardware description of HW3
 *
 * RA3 --> Capteur de pédalage
 * RA0 --> Vbatt Pont 82K 10K --> 0,1087 ou / 9,2
 * RA1 --> Hall A
 * RA2 --> IMOT Courant Négatif à partir de 2,5V
***************************************************************/

void DC24HdwInit(void)
{
#ifdef PIC12F1822
    ANSA1=0;
#endif

#ifdef PIC16F1823
    TRISC3=0; // RC3 Output
    TRISC1=0; // RC1 Output
#endif

#ifdef PIC16F1823_INTERNAL
    TRISC3=0; // RC3 Output
    TRISC1=0; // RC1 Output
#endif

}