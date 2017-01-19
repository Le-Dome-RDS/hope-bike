/*
 * File:   Bio-NetApp.c
 * Author: francois
 *
 * Created on 31 octobre 2014, 18:17
 */

#include <stdio.h>
#include <stdlib.h>
#include "Type.h"
#include "Bio-NetApp.h"
#include "DC22M_GlobalVar.h"

// Bio-Net configuration
// This structure is used to configure the Bio-Net network
// it described the variable managed by this slaved



const struct BioNetConf sBioNetConf = {
    MyVarRefCourant,MyVarCycleCourant,MyVarSizeCourant,
    MyVarRefPedalage,MyVarCyclePedalage,MyVarSizePedalage,
    MyVarRefRPM,MyVarCycleRPM,MyVarSizeRPM,
    MyVarRefInfoBatt,MyVarCycleInfoBatt,MyVarSizeInfoBatt,
    VAR_NUMBER};

enum BioNetStatus eBioNetStatus= INIT;


// this function is called when a variable needs to be read by the network
// u8VarRef  is the variable reference which read is in progress
// pu8Buffer    pointed to the network buffer to be filled with the variable content
// the return value is FALSE in case of any error in the App
//
// WARNING THIS IS HERE THAT IS MANAGE THE VARIABLE TIMER
// AND THE STOP & RUN SPECIFIC BEHAVIOUR
// THE STATUS WIIL BE STE TO ERROR HERE IN CASE OF VARAIBLE TIME OUT
boolean IsOk_CallBackRead(u8 u8VarRef, u8* pu8Buffer)
{
    boolean Result=TRUE;

    if (u8VarRef == MyVarRefCourant)
    {
        // fill response buffer & raz timeout
        *(u16 *)pu8Buffer = u16CourantBatt;
    }
    else if (u8VarRef == MyVarRefPedalage)
    {
        // fill response buffer & raz timeout
        *pu8Buffer = u8Pedalage;
    }
    else if (u8VarRef == MyVarRefRPM)
    {
        // fill response buffer & raz timeout
        *pu8Buffer = u8RPM;
    }
    else if (u8VarRef == MyVarRefInfoBatt)
    {
         // fill response buffer & raz timeout
        *(u16 *)pu8Buffer = u16TensionBatterie;
    }
    else
    {
        // variable not managed
        Result=FALSE;
    }


    return (Result);
};

// this function is called when a variable needs to be written by the network
// u8VarRef  is the variable reference which write is in progress
// pu8Buffer    pointed to the network buffer where the variable is stored
// the return value is FALSE in case of any error in the App
//
// WARNING THIS IS HERE THAT IS MANAGE THE VARIABLE TIMER
// AND THE STOP & RUN SPECIFIC BEHAVIOUR
// THE STATUS WIIL BE STE TO ERROR HERE IN CASE OF VARAIBLE TIME OUT
boolean IsOk_CallBackWrite(u8 u8VarRef, u8* pu8Buffer)
{
    boolean Result=TRUE;

    // variable not managed
    Result=FALSE;

    return (Result);
};