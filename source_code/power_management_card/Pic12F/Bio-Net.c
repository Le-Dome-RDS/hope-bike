/*
 * File:   Bio-Net.c
 * Author: francois
 *
 * Created on 31 octobre 2014, 18:17
 */

#include <stdio.h>
#include <stdlib.h>
#include "Type.h"
#include <xc.h>
#include "Bio-NetApp.h"
#include "PrjDefine.h"

#define TailleBuff 4 //VarSize Max + 2 && >=4 ( init frame )
#define SizeControlFrame 4
#define SizeConfFrame 4

#define WRITEOK 0xA5
#define WRITENOK 0x33

#define STOPRECEIVING 0xFF
#define RX_DELAY_1MS 5

u8 tu8BioNetBuff[TailleBuff];
u8 u8IndexRx =0;
u8 u8IndexTx =0;
u8 u8TxFrameSize;

u8 u8RxNoDelay=0;
boolean bRazFlagAllowed=FALSE;

u8 u8Prescaler10ms=0;

struct VarConfDesc * pVarConfDes;

boolean bNetworkCommand=FALSE;
boolean bNetworkCommandOK=FALSE;
boolean bIsWriteFrame=FALSE;
boolean bNetworkReadOK=FALSE;
boolean bNetworkWriteOK=FALSE;

boolean IsOKVarIsForUs(u8 VarRef);
boolean IsOKChecksum(u8 u8SizeFrame);
void BioNetRazFlag(void);
struct VarConfDesc * pGetVafConf(u8 VarRef);
void CalculChecksumAndSend(u8 u8SizeFrame);


// this function does all stuf needed to initialize the The Bio-Net Stack
void BioNetInit(void)
{
#ifdef PIC12F1822
    //Clock initialisation for 32MHz
    OSCCON= 0x0E<<3; //IRCF=1110 0x0E

    // set TIMER0 for application management timer
    // it will launch a IT every 1ms
    TMR0CS =0; // use Fclock/4 --> 8Mhz
    PS0=0;
    PS1=1;      // prescaler by 8 --> 1Mz
    PS2=0;      // /256 we get about 0,25ms per IT (+2%)
    PSA=0;      // prescaler used
    TMR0IE=1;   // IT timer 0 enable

    // enable global & peripheral interrupt
    //GIE PEIE
    INTCON|=BIT7 + BIT6;

#endif

#ifdef PIC16F1823
    //Clock initialisation for 16MHz
    OSCCON= 0x0E<<3; //IRCF=1110 0x0E

    // set TIMER0 for application management timer
    // it will launch a IT every 1ms
    TMR0CS =0; // use Fclock/4 --> 8Mhz
    PS0=1;
    PS1=0;      // prescaler by 32 --> 0.25Mz
    PS2=0;      // /256 we get about 1ms per IT (+2%)
    PSA=0;      // prescaler used
    TMR0IE=1;   // IT timer 0 enable

    // enable global & peripheral interrupt
    //GIE PEIE
    INTCON|=BIT7 + BIT6;

#endif

#ifdef PIC16F1823_INTERNAL
    //Clock initialisation for 32MHz
    OSCCON= 0x0E<<3; //IRCF=1110 0x0E

    // set TIMER0 for application management timer
    // it will launch a IT every 1ms
    TMR0CS =0; // use Fclock/4 --> 8Mhz
    PS0=0;
    PS1=1;      // prescaler by 32 --> 0.25Mz
    PS2=0;      // /256 we get about 1ms per IT (+2%)
    PSA=0;      // prescaler used
    TMR0IE=1;   // IT timer 0 enable

    // enable global & peripheral interrupt
    //GIE PEIE
    INTCON|=BIT7 + BIT6;

#endif

}

// This is the main function of BioNet stack
// It should be called as often as possible
void BioNetProcess(void)
{
    enum BioNetStatus eBioNetStatusRx;

    if (bNetworkCommandOK==TRUE)
    {
        // Network Control Frame received

        //change of status
        eBioNetStatusRx=tu8BioNetBuff[1];
        if (eBioNetStatus ==  INIT)
        {
            // we can go in all status when in INIT
            eBioNetStatus=eBioNetStatusRx;
        }
        else if ((eBioNetStatus ==  RUN) && (eBioNetStatusRx != INIT))
        {
            // we can go in all status except INIT
            eBioNetStatus=eBioNetStatusRx;
        }
        else if (eBioNetStatus ==  STOP)
        {
            // we can go in all status
            eBioNetStatus=eBioNetStatusRx;
        }
        else if ((eBioNetStatus ==  ERROR) && (eBioNetStatusRx == INIT))
        {
            // only go to INIT
            eBioNetStatus=INIT;
        }
        BioNetRazFlag();
    }

    if ( (eBioNetStatus == INIT) && bNetworkReadOK)
    {
        // *********** state INIT **************
        // we answer only to read frame
        tu8BioNetBuff[1]=pVarConfDes->u8VarTypeSize;
        tu8BioNetBuff[2]=pVarConfDes->u8VarCycle;

        CalculChecksumAndSend(SizeConfFrame);
        bNetworkReadOK=FALSE;
    }

    if ( eBioNetStatus == RUN || eBioNetStatus ==STOP)
    {
        // *********** state RUN **************
        if (bNetworkReadOK)
        {
            // Read Frame
            if (IsOk_CallBackRead(pVarConfDes->u8VarRef,&tu8BioNetBuff[1]))
            {
                // Fill value OK
                // send response
                CalculChecksumAndSend( (pVarConfDes->u8VarTypeSize & 0x0F) +2);
                bNetworkReadOK=FALSE;
            }
        }
        if (bNetworkWriteOK)
        {
            // Write Frame
            if (IsOk_CallBackWrite(pVarConfDes->u8VarRef,&tu8BioNetBuff[1]))
            {
                // send ACK
                tu8BioNetBuff[0]=WRITEOK;
            }
            else
            {
                 // send ACK
                tu8BioNetBuff[0]=WRITENOK;
            }
            CalculChecksumAndSend(1);
            bNetworkWriteOK=FALSE;
        }

    }

    if ( eBioNetStatus == ERROR )
    {
        // *********** state ERROR **************
        // We do not respond only Raz Flag is needed
        if (bNetworkReadOK || bNetworkWriteOK)
        {
            BioNetRazFlag();
        }
    }

}

//****************************************
// this is the RX interrupt to manage
void BioNetRxInt(void)
{
    u8 u8CharBuf;
    u8 u8TxFrameSize;

    u8RxNoDelay=RX_DELAY_1MS;
    bRazFlagAllowed=TRUE;

    // read the received byte & store it
    u8CharBuf=RCREG;
    // do nothing more if Receiving not allowed
    if (u8IndexRx == STOPRECEIVING)
    {
        return;
    }

    // we store the char in the buffer
    tu8BioNetBuff[u8IndexRx]=u8CharBuf;

     //****** First byte received management *****
    if (u8IndexRx == 0)
    {
        // we check if this is the first char of a Network Control frame
        // code 0x80
        if ( u8CharBuf==VAR_COMMAND )
        {
            // yes this is a Network Control frame
            bNetworkCommand=TRUE;
            //u8IndexRx++;
           //return;
        }
        else if (IsOKVarIsForUs(u8CharBuf))
        {
            // we manage the Variable
            // first get conf
            pVarConfDes=pGetVafConf(u8CharBuf & 0x7F);
            
            //check if read or write
            if ((u8CharBuf & BIT7)==0)
            {
                // this is a Read frame
                // nothing more to receive
                bIsWriteFrame=FALSE;
                bNetworkReadOK=TRUE;
                u8IndexRx=STOPRECEIVING;
            }
            else
            {
                // write frame we continue receiving
                bIsWriteFrame=TRUE;

                if (pVarConfDes==NULL)
                {
                    // error somewhere Var Ref not found
                    // stop receiving
                    bIsWriteFrame=FALSE;
                    u8IndexRx=STOPRECEIVING;
                }
            }
        }
        else
        {
            // we do not manage the Variable
            // so we do not listen anymore untill ennd of frames
            // detected by Timer
            u8IndexRx=STOPRECEIVING;
        }
        // end of fist char management
    }
    else
    {   // we now manage Next byte 2nd and following
        if (bNetworkCommand==TRUE)
        {
            //****** manage Network Control frame *****
            // last frame charactere ?
            if (u8IndexRx== (SizeControlFrame-1))
            {
                // yes we stop receiving bytes
                u8IndexRx=STOPRECEIVING;
                // checksum OK ?
                if (IsOKChecksum(SizeControlFrame))
                {
                    //Yes
                    bNetworkCommandOK=TRUE;
                    
                }
            }
            //****** end manage Network Control frame *****
         }
         else if (bIsWriteFrame)
         {
            // we now manage WRITE frames
             u8TxFrameSize=(pVarConfDes->u8VarTypeSize & 0x0F)+2;
            // last frame charactere ?
            if (u8IndexRx== (u8TxFrameSize-1))
            {
                // yes we stop receiving bytes
                u8IndexRx=STOPRECEIVING;
                // checksum OK ?
                if (IsOKChecksum(u8TxFrameSize))
                {
                    //Yes
                    bNetworkWriteOK=TRUE;
                }
            }

         // end manage WRITE frames
         }
         else
         {
             // default path normaly never used
             BioNetRazFlag();
         }
    }

    // prepare for next byte
    if (u8IndexRx!=STOPRECEIVING) u8IndexRx++;
}

//****************************************
// this is the RX interrupt to manage
void BioNetTxInt(void)
{
    if (u8IndexTx < u8TxFrameSize)
    {
        // there are some char to send
        TXREG=tu8BioNetBuff[u8IndexTx++];
    }
    else
    {
        TXIE=0; // disable Tx Interrupt
    }
}

//****************************************
// Check if the Var Reference is managed by ourself
boolean IsOKVarIsForUs(u8 VarRef)
{
    u8 VarIdx=0;
    boolean IsManagedByOurself = FALSE;

    VarRef&=0x7F; // remove R/W bit (B7)

    for (;VarIdx<VAR_NUMBER;VarIdx++)
    {
        // sarch for Variable Id in the slave configuration
        if (sBioNetConf.sVarConfDesc[VarIdx].u8VarRef == VarRef)
        {
            // Found, we have to respond;
            IsManagedByOurself=TRUE;
            // reponce will be manage in BioNetProcess()
        }
    }
    // Not Found we do Nothing
    return (IsManagedByOurself);
}

//********************************************
// calculate the checksum of the transmit frame
// Send the frame
void CalculChecksumAndSend(u8 u8SizeFrame)
{
    u8 u8Check=0;
    u8 index=0;

    // we calculate the checksum only if Frame has more than 1 char
    if (u8SizeFrame != 1)
    {
        // do not take checksum itself into account
        u8SizeFrame--;

        // do the calculation
        for (; index < u8SizeFrame ; index++)
        {
            u8Check+=tu8BioNetBuff[index];
        }
        // store the complemented checksum
        tu8BioNetBuff[index]= ~u8Check;

        u8SizeFrame++;
    }

    // frame completed we send it
    u8TxFrameSize=u8SizeFrame;
    u8IndexTx=1;
    TXREG=tu8BioNetBuff[0];
    TXIE=1; // enable Tx Interrupt
}

// check the checksum of the received frame
boolean IsOKChecksum(u8 u8SizeFrame)
{
    u8 u8Check=0;
    u8 u8CheckCpl;
    u8 index=0;

    // size error
    if (u8SizeFrame==1) return(FALSE);
    if (u8SizeFrame > TailleBuff) return(FALSE);

    // do not take checksum itself into account
    u8SizeFrame--;

    for (; index < u8SizeFrame ; index++)
    {
        u8Check+=tu8BioNetBuff[index];
    }

    u8CheckCpl=~tu8BioNetBuff[index];

    if (u8Check == u8CheckCpl)
    {
        //Checksum OK
        return(TRUE);
    }
    else
    {
        //Checksum Error
        return (FALSE);
    }
        
}

//***********************************************
// Raz all flag used to manage Network frame
// This will allow receiving new frames
void BioNetRazFlag(void)
{
    u8IndexRx =0;

    bNetworkCommand=FALSE;
    bNetworkCommandOK=FALSE;
    bIsWriteFrame=FALSE;
    bNetworkReadOK=FALSE;
    bNetworkWriteOK=FALSE;

}


//***********************************************
// read in the configuration the variable size
struct VarConfDesc * pGetVafConf(u8 VarRef)
{
    u8 idx=0;

    // check in configfor only the number of variable contained
    for (;idx<VAR_NUMBER;idx++)
    {
        if (sBioNetConf.sVarConfDesc[idx].u8VarRef ==  VarRef)
        {
            // found we return the address
            return ((struct VarConfDesc *)&sBioNetConf.sVarConfDesc[idx]);
        }
    }
    // Variable Ref Not found. it should never happend
    return (NULL);
}