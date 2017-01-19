/*
 		File:   Bio-NetApp.c
    Copyright (C) 2015-2016  François Trocque
    
    This file is part of HnB_Controller project.

    HnB_Controller is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    HnB_Controller is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with HnB_Controller.  If not, see <http://www.gnu.org/licenses/>. 2
*/

#include <stdio.h>
#include <stdlib.h>
#include "Type.h"
#include "Bio-NetApp.h"
#include "DC22_GlobalVar.h"

// Bio-Net configuration
// This structure is used to configure the Bio-Net network
// it described the variable managed by this slaved



const struct BioNetConf sBioNetConf = {
    MyVarRefConsigneMotor,MyVarCycleConsigneMotor,MyVarSizeConsigneMotor,
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

    if (u8VarRef == MyVarRefCourantBatt)
    {
        // fill response buffer & raz timeout
        *pu8Buffer = u8CourantBatt;
        u8CourantBattTimeout=0;
    }
    else if (u8VarRef == MyVarRefPedalage)
    {
        // fill response buffer -> No timeout
        *pu8Buffer = u8Pedalage;
    }
    else if (u8VarRef == MyVarRefRpmMotor)
    {
        // fill response buffer -> No timeout
        *pu8Buffer = u8RpmMotor;
    }
    else if (u8VarRef == MyVarRefInfoBatt)
    {
         // fill response buffer -> No timeout
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

    if (u8VarRef == MyVarRefConsigneMotor)
    {
        //write variable & reset time_out
        u8ConsigneMotor=*pu8Buffer;
        u8ConsigneMotorTimeout=0;
    }
    else if (u8VarRef == MyVarRefSeuilBatt)
    {
        //write variable -> No time_out
        u16SeuilBatterie=*(u16 *)pu8Buffer;
    }
    else
    {
        // variable not managed
        Result=FALSE;
    }


    return (Result);
};