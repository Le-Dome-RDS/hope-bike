/*
 * File:   Hdw_Uart.c
 * Author: francois
 *
 * Created on 31 octobre 2014, 18:17
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

}