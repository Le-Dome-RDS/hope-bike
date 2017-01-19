/* 
 * File:   Bio-NetApp.h
 * Author: francois
 *
 * Created on 1 novembre 2014, 21:23
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
#define VAR_NUMBER  4

#define MyVarRefCourant 2 //Courant Moteur
#define MyVarCycleCourant 255
#define MyVarSizeCourant  2+VAR_READ

#define MyVarRefPedalage 7 //pedalage
#define MyVarCyclePedalage 255
#define MyVarSizePedalage  1+VAR_READ

#define MyVarRefRPM 8       // Vitesse moteur
#define MyVarCycleRPM 5
#define MyVarSizeRPM  1+VAR_READ

#define MyVarRefInfoBatt 15 // information batterie
#define MyVarCycleInfoBatt 255
#define MyVarSizeInfoBatt  2+VAR_READ

// End of Variables configuration
//*****************************************************************

#define VAR_COMMAND   0x80 // B7=1 + 0x00

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

