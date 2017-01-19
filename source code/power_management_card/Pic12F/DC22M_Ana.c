/*
 * File:   DC22_Ana.c
 * Author: francois
 *
 * Created on 30 avril 2015, 19:44
 */
#include <xc.h>
#include "Type.h"
#include "DC22M_GlobalVar.h"
#include "Hdw_Adc.h"

#define ADC_BATTERY_CHANNEL 0
#define ADC_CURRENT_CHANNEL 2

enum DC22_AnaStatus {
    A_START_I,
    A_READ_I,
    A_START_BATT,
    A_READ_BATT
};


enum DC22_AnaStatus eDC22_AnaState=A_START_I;

void DC22_AnaProcess(void)
{

    // launch Motor Current read
    if (eDC22_AnaState==A_START_I)
    {
        Hdw_AdcStartReadChannel(ADC_CURRENT_CHANNEL);
        eDC22_AnaState=A_READ_I;
    }

    // Check end & treat Motor Current
    else if (eDC22_AnaState==A_READ_I)
    {
        // check if read
        if (Hdw_AdcIsOkReadAna())
        {
            // here we read now the real current
            // the value decrease from the offset down to 0V for 20A
            u16CourantBatt=Hdw_AdcReadAnaValue();

            eDC22_AnaState=A_START_BATT;
        }
    }

    // Read V Batt
    else if (eDC22_AnaState==A_START_BATT)
    {
        Hdw_AdcStartReadChannel(ADC_BATTERY_CHANNEL);

        eDC22_AnaState=A_READ_BATT;
    }

    else if (eDC22_AnaState==A_READ_BATT)
    {
        // read batt
         // check if read
        if (Hdw_AdcIsOkReadAna())
        {
           u16TensionBatterie=Hdw_AdcReadAnaValue();

           eDC22_AnaState=A_START_I;
        }
    }

}