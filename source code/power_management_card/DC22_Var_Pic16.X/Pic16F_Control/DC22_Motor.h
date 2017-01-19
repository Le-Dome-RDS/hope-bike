/* 
		File:   DC22_Motor.h
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

#ifndef DC22_MOTOR_H
#define	DC22_MOTOR_H

#ifdef	__cplusplus
extern "C" {
#endif

enum eMotorStatus {
        M_STOP,
        M_STARTING,
        M_CHECKPOS,
        M_STARTED,
        M_ERROR
};

extern u8 CounterHallTurning;
extern enum eMotorStatus MotorState;

void DC22_MotorProcess(u8 PWMValue);
void DC22_MotorInit(void);
void DC22_MotorReadRPM(u8 * pu8RPM);

//Hall A RB6
#define HALL_A ((PORTB & 0x40) == 0x40)
//Hall B RC0
#define HALL_B ((PORTC & 0x01) == 0x01)
//Hall C RA2
#define HALL_C ((PORTA & 0x04) == 0x04)

//HighSide A RC5 PWM1
#define PWM_A   PWM1DCH
//HighSide B RC3 PWM2
#define PWM_B   PWM2DCH
//HighSide B RC1 PWM4
#define PWM_C   PWM4DCH

//Low Side A RC4
#define LOW_A_ON LATC4=0
#define LOW_A_OFF LATC4=1

//Low Side B RC2
#define LOW_B_ON LATC2=0
#define LOW_B_OFF LATC2=1

//Low Side C RB4
#define LOW_C_ON LATB4=0
#define LOW_C_OFF LATB4=1

// 500ms
#define RUNING_WINDOW 50

#define NB_CYCLE_CHARGE 10

#ifdef	__cplusplus
}
#endif

#endif	/* DC22_MOTOR_H */

