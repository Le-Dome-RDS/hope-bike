/*
 * File:   DC22_Pedalage.c
 * Author: francois
 *
 * Created on 19 mai 2015, 08:35
 */
#include <xc.h>
#include "Type.h"
#include <stdio.h>

#include "DC22M_Pedalage.h"

// PA0 is the "padalage" input
#define PEDALE_PORT ((PORTA & 0x01) == 0x01)
#define PEDALE_OFF_ON 25
#define PEDALE_ON_OFF 30
#define PEDALE_ON 1
#define PEDALE_OFF 0


u8 u8CptPedalage=PEDALE_OFF;
boolean bMemoPedalage=FALSE;

//*******************************************************
// Pedalage process should be called every 10ms
// pu8Pedalage will contains the result
// if NULL the function is not managed
// Should avoid any glitch @startup
void DC22_PedalageProcess(u8 *pu8Pedalage)
{
    boolean bPedalage;

    // read "pedalage" in one time
    if (PEDALE_PORT)
    {
        bPedalage=TRUE;
    }
    else
    {
        bPedalage=FALSE;
    }

    // detect rising edge on "Pedalage" only if Management Allowed
    if ( (bPedalage && !bMemoPedalage) && (pu8Pedalage != NULL) )
    {
        if (*pu8Pedalage == PEDALE_OFF)
        {
            // the guy was not"pedaling"
            // we check if he "pedals" quick enought
            if (u8CptPedalage < PEDALE_OFF_ON)
            {
                *pu8Pedalage=PEDALE_ON;
            }

        }
        // reset Timer "pedalage"
        // we reset here to ensure that the first edge detected
        // doesn't be concider as a "pedalage" ON

        u8CptPedalage=0;
    }
    // NO rising edge detected on "Pedalage" or Management not allowed
    else
    {
        // increase Timer "pedalage"
        u8CptPedalage++;

        if (u8CptPedalage > PEDALE_ON_OFF)
        {
            u8CptPedalage--; // to stay allways @PEDALE_ON_OFF

            if (pu8Pedalage != NULL)
            {
                *pu8Pedalage=PEDALE_OFF;
            }
        }
    }
    
    // store actual state for edge detection
    bMemoPedalage=bPedalage;
}


