/* 
 * File:   PrjDefine.h
 * Author: francois
 *
 * Created on 29 décembre 2014, 14:22
 */

#ifndef PRJDEFINE_H
#define	PRJDEFINE_H

#ifdef	__cplusplus
extern "C" {
#endif

// Processor selection
#define PIC12F1822
#define noPIC16F1823
#define noPIC16F1823_INTERNAL

#ifdef PIC16F1823
#define LED_STATUS_ON LATC3=0
#define LED_STATUS_OFF LATC3=1
#define LED_OUTPUT_ON LATC1=0
#define LED_OUTPUT_OFF LATC1=1
#endif

#ifdef PIC16F1823_INTERNAL
#define LED_STATUS_ON LATC3=0
#define LED_STATUS_OFF LATC3=1
#define LED_OUTPUT_ON LATC1=0
#define LED_OUTPUT_OFF LATC1=1
#endif

#ifdef PIC12F1822
#define LED_STATUS_ON
#define LED_STATUS_OFF
#define LED_OUTPUT_ON LATA2=0
#define LED_OUTPUT_OFF LATA2=1
#endif

#ifdef	__cplusplus
}
#endif

#endif	/* PRJDEFINE_H */

