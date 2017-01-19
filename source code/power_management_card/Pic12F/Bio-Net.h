/* 
 * File:   Bio-Net.h
 * Author: francois
 *
 * Created on 1 novembre 2014, 19:16
 */

#ifndef BIO_NET_H
#define	BIO_NET_H

#ifdef	__cplusplus
extern "C" {
#endif

#include "Type.h"

extern u8 u8IndexRx;
extern boolean bNetworkCommand;
extern boolean bNetworkCommandOK;
extern boolean bIsWriteFrame;
extern boolean bNetworkReadOK;
extern boolean bNetworkWriteOK;

extern u8 u8RxNoDelay;
extern boolean bRazFlagAllowed;
extern u8 u8Prescaler10ms;

#define VARCYCLE    4*10 //10ms

void BioNetInit(void);
void BioNetProcess(void);
void BioNetRxInt(void);
void BioNetTxInt(void);
void BioNetRazFlag(void);


#ifdef	__cplusplus
}
#endif

#endif	/* BIO_NET_H */

