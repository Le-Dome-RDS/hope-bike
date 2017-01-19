/* 
		File:   PrjDefine.h
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

#ifndef PRJDEFINE_H
#define	PRJDEFINE_H

#ifdef	__cplusplus
extern "C" {
#endif

// Processor selection
#define noPIC12F1822
#define noPIC16F1823
#define noPIC16F1823_INTERNAL
#define PIC16F1509

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

#ifdef PIC16F1509
#define LED_STATUS_ON
#define LED_STATUS_OFF
#define LED_OUTPUT_ON
#define LED_OUTPUT_OFF
#endif

#ifdef	__cplusplus
}
#endif

#endif	/* PRJDEFINE_H */

