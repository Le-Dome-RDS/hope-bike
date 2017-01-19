/* 
 * File:   Hdw_Adc.h
 * Author: francois
 *
 * Created on 30 avril 2015, 19:28
 */

#ifndef HDW_ADC_H
#define	HDW_ADC_H

#ifdef	__cplusplus
extern "C" {
#endif


void Hdw_AdcInit(void);
void Hdw_AdcStartReadChannel(u8 Channel);
boolean Hdw_AdcIsOkReadAna(void);
u16 Hdw_AdcReadAnaValue(void);


#ifdef	__cplusplus
}
#endif

#endif	/* HDW_ADC_H */

