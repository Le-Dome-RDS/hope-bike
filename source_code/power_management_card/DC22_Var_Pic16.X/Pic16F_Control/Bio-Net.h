/* 
 		File:   Bio-Net.h
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

