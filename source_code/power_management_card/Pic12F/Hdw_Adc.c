/*
 * File:   Hdw_Adc.c
 * Author: francois
 *
 * Created on 30 April 2015
 */

#include <xc.h>
#include "Type.h"
#include "PrjDefine.h"

void Hdw_AdcInit(void)
{
#ifdef PIC12F1822

    // initialized Batterie analog input
    TRISA0=1;
    ANSA0=1;

    // initialized Motor courant analog input
    TRISA2=1;
    ANSA2=1;

    ADCON1=0xA0;
    //ADFM=1;    B7right justify
    //ADCS0=0;   B6Fosc/32
    //ADCS1=1;   B5
    //ADCS2=0;   B4
    //ADPREF0=0; B1 // Ref = Vcc
    //ADPREF1=0; B0

    // start ADC module
    ADON=1;


#endif
}

//*************************************************
// Start the Channel read
// Input Channel number 0 --> 11
void Hdw_AdcStartReadChannel(u8 Channel)
{
    // set channel and Keep ADON=1
    ADCON0=(Channel<<2)+1;

    // start conversion
    ADGO=1;
}

//*************************************************
// Check if Analog read finished
// Input none
// Output TRUE --> finished
boolean Hdw_AdcIsOkReadAna(void)
{
    // check if Analog channel read is finished
    if (ADGO==0)
    {
        return (TRUE);
    }
    else
    {
        return (FALSE);
    }

}

//*************************************************
// Read the Analog value read
// Input none
// Output TRUE --> finished
u16 Hdw_AdcReadAnaValue(void)
{
    u16 temp;
    temp = (u16)(ADRESH & 0x3);
    temp <<= 8;
    temp += ADRESL;

    return (temp);
}