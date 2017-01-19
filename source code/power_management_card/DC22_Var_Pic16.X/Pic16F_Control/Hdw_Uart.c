/*
		File:   Hdw_Uart.c
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
//#include "pic12f1822.h"
#include "Type.h"
#include "PrjDefine.h"
#include <xc.h>

void HdwUartInit (void)
{
    // physical port setting
#ifdef PIC12F1822
   //TXon RA4&  RXon RA5
   RXDTSEL=1;
   TXCKSEL=1;

   // Baudrate setting
   // seeting following Table 26-5 of the manual
   SYNC=0;
   BRG16=1;
   BRGH=1;

   // normally 416(0x1A0) for 32MHz
   SPBRGH= 0x1;
   SPBRGL=0xA0; //19200bds --> 138 for 55600 To be tested
   // Character format setting
   // 8 bit by default
   // no parity manage by the processor

   //interrupt setting
   //allows Interrupt for RX
   RCIE=1;

   // enable UART & reception
   TXEN=1;
   SPEN=1;
   CREN=1;
#endif

#ifdef PIC16F1823
  // default is RC4 RC5 for 1823

   // Baudrate setting
   // seeting following Table 26-5 of the manual
   SYNC=0;
   BRG16=1;
   BRGH=1;

   // normally 416 for 32MHz
   SPBRGH= 0;
   SPBRGL=208; //19200bds --> 138 for 55600 To be tested
   // Character format setting
   // 8 bit by default
   // no parity manage by the processor
   
   //interrupt setting
   //allows Interrupt for RX
   RCIE=1;

   // enable UART & reception
   TXEN=1;
   SPEN=1;
   CREN=1;
#endif

#ifdef PIC16F1823_INTERNAL
  // default is RC4 RC5 for 1823

   // Baudrate setting
   // seeting following Table 26-5 of the manual
   SYNC=0;
   BRG16=1;
   BRGH=1;

   // normally 416(0x1A0) for 32MHz
   SPBRGH= 0x1;
   SPBRGL=0xA0; //19200bds --> 138 for 55600 To be tested
   // Character format setting
   // 8 bit by default
   // no parity manage by the processor

   //interrupt setting
   //allows Interrupt for RX
   RCIE=1;

   // enable UART & reception
   TXEN=1;
   SPEN=1;
   CREN=1;
#endif

#ifdef PIC16F1509

   ANSB5=0;
   // Baudrate setting
   // seeting following Table 26-5 of the manual
   SYNC=0;
   BRG16=1;
   BRGH=1;

   // normally 208 for 16MHz
   SPBRGH= 0;
   SPBRGL=208; //19200bds --> 138 for 55600 To be tested
   // Character format setting
   // 8 bit by default
   // no parity manage by the processor

   //interrupt setting
   //allows Interrupt for RX
   RCIE=1;

   // enable UART & reception
   TXEN=1;
   SPEN=1;
   CREN=1;
#endif

}