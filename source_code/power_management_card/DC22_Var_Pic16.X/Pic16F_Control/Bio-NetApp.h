/* 
		File:   Bio-NetApp.h
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

#ifndef BIO_NETAPP_H
#define	BIO_NETAPP_H

#ifdef	__cplusplus
extern "C" {
#endif

#define VAR_READ    0x10    // B4&B5 of type/size byte in Read conf frame
#define VAR_WRITE   0x20    // B4&B5 of type/size byte in Read conf frame

//*****************************************************************
// Variables configuration
#define VAR_NUMBER  1

#define MyVarRefConsigneMotor 1 // Consigne moteur
#define MyVarCycleConsigneMotor 50
#define MyVarSizeConsigneMotor  1+VAR_WRITE

#define MyVarRefCourantBatt 2 //Courant Batt
#define MyVarCycleCourantBatt 5
#define MyVarSizeCourantBatt  1+VAR_READ

#define MyVarRefPedalage 7 //pedalage
#define MyVarCyclePedalage 0
#define MyVarSizePedalage  1+VAR_READ

#define MyVarRefRpmMotor 8 //Rpm Motor
#define MyVarCycleRpmMotor 0
#define MyVarSizeRpmMotor  1+VAR_READ

#define MyVarRefSeuilBatt 15 // Seuil Batt
#define MyVarCycleSeuilBatt 0
#define MyVarSizeSeuilBatt  2+VAR_WRITE

#define MyVarRefInfoBatt 16 // information batterie
#define MyVarCycleInfoBatt 0
#define MyVarSizeInfoBatt  2+VAR_READ

    
// End of Variables configuration
//*****************************************************************

#define VAR_COMMAND   0x80 // B7=1 + 0x00
#define RefConsigneMotor   0x81 // B7=1 + 0x01

    typedef struct VarConfDesc{
        u8 u8VarRef;    // Var reference in use
        u8 u8VarCycle;  // Var access cycle time min
        u8 u8VarTypeSize;   // Var Size in byte
    };

    typedef struct BioNetConf{
        struct VarConfDesc sVarConfDesc[VAR_NUMBER];
        u8 u8VarNumber;
    };

    enum BioNetStatus {
        INIT,
        RUN,
        STOP,
        ERROR
    };

extern const struct BioNetConf sBioNetConf;
extern enum BioNetStatus eBioNetStatus;
extern u8 tu8BioNetBuff[];

boolean IsOk_CallBackRead(u8 u8VarNumber, u8* pu8Buffer);
boolean IsOk_CallBackWrite(u8 u8VarNumber, u8* pu8Buffer);


#ifdef	__cplusplus
}
#endif

#endif	/* BIO_NETAPP_H */

