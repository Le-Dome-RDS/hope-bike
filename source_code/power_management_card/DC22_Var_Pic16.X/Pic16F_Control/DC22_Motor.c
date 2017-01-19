/*
		File:   DC22_Motor.c
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

#include <xc.h>
#include "Type.h"
#include "DC22_Motor.h"

// this is the product of mechanical gear rate by number of pole
#define RMP_RATE 10
#define RPM_COUNTING_CYCLE 100

u8 ValHallMem1=0;
u8 ValHallMem2=0;
u8 ValHallMem3=0;
u8 ValHallPrevious=0;

u8 u8MotorSequence;
u8 u8HallSequenceNext;
boolean bWriteMotorSequence=FALSE;

u8 PWMValuePrevious=0;

enum eMotorStatus MotorState=M_STOP;
u8 u8NbCycleChargeHight=0;
u8 CounterHallTurning=0;

u8 CycleChargeHight=0;

u16 u16CounterPhase=0;
u8 u8TimerRPM=0;

// Sequence Moteur 1 à 6 vs Hall position
// it can be different for motor manufacturer
// but allway composed of sequence 5 4 6 2 3 1
u8 tu8Motor2Hall[6]=        {2,3,1,5,4,6};
u8 tu8Motor2HallNext[6]=    {3,1,5,4,6,2};

void DC22_MotorInit(void)
{
        // set Timer 2 to be used for PWM at about 10Khz
    // prescaler & postscaler are set to 1/4 defaut value
    T2CKPS0=1;
    PR2=100; //10KHz
    TMR2ON=1;

    // enable OutPut for low side
    TRISC4=0;
    TRISC2=0;
    TRISB4=0;

    LOW_A_OFF;
    LOW_B_OFF;
    LOW_C_OFF;

    // raz PWM values
    PWM1DCL=0;
    PWM2DCL=0;
    PWM4DCL=0;
    PWM1DCH=0;
    PWM2DCH=0;
    PWM4DCH=0;

    // Invert PWM polarity  not needed anymore because PWM are now in High Side
    //PWM1POL=1;
    //PWM2POL=1;
    //PWM4POL=1;

    // enable Output for PWM
    TRISC1=0;
    TRISC3=0;
    TRISC5=0;

    // enable PWM
    PWM1EN=1;
    PWM2EN=1;
    PWM4EN=1;

    // enable PWM output
    PWM1OE=1;
    PWM2OE=1;
    PWM4OE=1;

    // enable digital input for Hall sensors
    ANSC0=0;
    ANSA2=0;

}


void DC22_MotorProcess(u8 PWMValue)
{
    u8 ValHall=0;
    u8 i;

    // disable interrupt
    GIE=0;

    if ( MotorState == M_STARTED)
    {
        if (PWMValue == 0)
        {
            MotorState=M_STOP;
        }
        else
        {
            if (!HALL_A) ValHall+=4; //B2
            if (!HALL_B) ValHall+=2; //B1
            if (!HALL_C) ValHall+=1; //B0

            // store the last 3 Halls values
            //ValHallMem3=ValHallMem2;
            //ValHallMem2=ValHallMem1;
            //ValHallMem1=ValHall;

            //if ( (ValHallMem1 == ValHallMem2) && (ValHallMem2 == ValHallMem3))
            //{
                // we check if we get the Next Hall Phase
            if (ValHall == u8HallSequenceNext)
            {
                // yes next phase
                u8MotorSequence++;
                if (u8MotorSequence > 5) u8MotorSequence=0;
                bWriteMotorSequence=TRUE;

                u8HallSequenceNext=tu8Motor2HallNext[u8MotorSequence];

                // Running
                CounterHallTurning=0;
                // increase the number of phase for RPM management
                u16CounterPhase++;
            }


            //}
            
            if (bWriteMotorSequence==TRUE)
            {
                // do the job one time;
                bWriteMotorSequence=FALSE;
                
                // we apply the selected phase
                if (u8MotorSequence == 0)
                {
                    // SEQ 0
                    LOW_A_OFF;
                    PWM_A=PWMValue;
                    PWM_B=0;
                    LOW_B_ON;
                    PWM_C=0;
                    LOW_C_OFF;
                }
                else if (u8MotorSequence == 1)
                {
                    // SEQ 1
                    LOW_A_OFF;
                    PWM_A=PWMValue;
                    PWM_B=0;
                    LOW_B_OFF;
                    PWM_C=0;
                    LOW_C_ON;
                }
                else if (u8MotorSequence == 2)
                {
                    // SEQ 2
                    LOW_A_OFF;
                    PWM_A=0;
                    PWM_B=PWMValue;
                    LOW_B_OFF;
                    PWM_C=0;
                    LOW_C_ON;
                }
                else if (u8MotorSequence == 3)
                {
                    // SEQ 3
                    PWM_A=0;
                    LOW_A_ON;
                    PWM_B=PWMValue;
                    LOW_B_OFF;
                    PWM_C=0;
                    LOW_C_OFF;
                }
                else if (u8MotorSequence == 4)
                {
                    // SEQ 4
                    PWM_A=0;
                    LOW_A_ON;
                    PWM_B=0;
                    LOW_B_OFF;
                    LOW_C_OFF;
                    PWM_C=PWMValue;
                }
                else if (u8MotorSequence == 5)
                {
                    // SEQ 5
                    PWM_A=0;
                    LOW_A_OFF;
                    PWM_B=0;
                    LOW_B_ON;
                    LOW_C_OFF;
                    PWM_C=PWMValue;
                }
            }
            
            // check if the counter has elapsed
            if (CounterHallTurning==RUNING_WINDOW)
            {
                MotorState=M_ERROR;
            }
        }
    }
    
    else if ( (MotorState == M_STOP) || (MotorState == M_ERROR) )
    {
        // stop driver
        PWM_A=0;
        PWM_B=0;
        PWM_C=0;
        LOW_A_OFF;
        LOW_B_OFF;
        LOW_C_OFF;

        // check if restart motor
        if ( (PWMValuePrevious==0) && (PWMValue!=0) )
        {
            // motor restarted we allow to retry running the motor
            MotorState=M_STARTING;
        }
    }
    // we activate low side to keep bootstrap charged
    // when motor is stopped or unable to run
    else if ( MotorState == M_STARTING)
    {
        // maintien charge high
        PWM_A=0;
        PWM_B=0;
        PWM_C=0;

        if (CycleChargeHight==0)
        {
            // CYCLE 1 LOW SIDE A =ON
            LOW_A_ON;
            LOW_B_OFF;
            LOW_C_OFF;
            CycleChargeHight=1;
        }
        else if (CycleChargeHight==1)
        {
            // CYCLE 2 LOW SIDE B =ON
            LOW_A_OFF;
            LOW_B_ON;
            LOW_C_OFF;
            CycleChargeHight=2;
        }
        else if (CycleChargeHight==2)
        {
            // CYCLE 3 LOW SIDE C =ON
            LOW_A_OFF;
            LOW_B_OFF;
            LOW_C_ON;
            CycleChargeHight=3;
        }
        else if (CycleChargeHight==3)
        {
            // CYCLE 4  All LOW SIDE OFF
            // all off to allow starting
            LOW_A_OFF;
            LOW_B_OFF;
            LOW_C_OFF;
            CycleChargeHight=0;

            // check if the number of cycle is reached
            u8NbCycleChargeHight++;
            if (u8NbCycleChargeHight == NB_CYCLE_CHARGE)
            {
                // reached
                MotorState=M_CHECKPOS;
                u8NbCycleChargeHight=0;
                CounterHallTurning=0;
                ValHallMem1=255;
                ValHallMem2=255;
                ValHallMem3=255;
            }
        }
    }

    else if ( MotorState == M_CHECKPOS)
    {
        // here we analyse the motor position
        // to start at the good one
        if (!HALL_A) ValHall+=4; //B2
        if (!HALL_B) ValHall+=2; //B1
        if (!HALL_C) ValHall+=1; //B0

        // store the last 3 Halls values
        ValHallMem3=ValHallMem2;
        ValHallMem2=ValHallMem1;
        ValHallMem1=ValHall;

        if ( (ValHallMem1 == ValHallMem2) && (ValHallMem2 == ValHallMem3))
        {
            // Hall are stable 3 samples identical
            // look for the Motor sequence number
            u8MotorSequence=255;
            for (i=0;i<6;i++)
            {
                // existing Hall Value ?
                if (ValHallMem1 == tu8Motor2Hall[i])
                {
                    //Yes
                    // we store next Hall Sequence & Motor sequence
                    u8MotorSequence=i;
                    u8HallSequenceNext=tu8Motor2HallNext[i];
                    break;
                }
            }

            if (u8MotorSequence != 255)
            {
                // HAll Ok sequence found we can start
                MotorState = M_STARTED;
                bWriteMotorSequence=TRUE;
            }
            else
            {
                // error HAll (Motor)
                MotorState = M_ERROR;
            }
        }
    }


    // store the actual PWM value for edge detection
    PWMValuePrevious=PWMValue;

    GIE=1; // enable interrupt
}

//*******************************************************
// ReadRPM process should be called every 10ms
// pu8RPM will contains the result
// result is filled every 1s and pu8RPM not change in other cases
void DC22_MotorReadRPM(u8 * pu8RPM)
{
    u16 u16RPM;
    
    // sequence manage with "u8TimerRPM"
    if (u8TimerRPM==0)
    {
        // first sequence we reset RPM
        u16CounterPhase=0;
    }
    else if (u8TimerRPM >= RPM_COUNTING_CYCLE)
    {
        // we calculate RPM
        u16RPM=u16CounterPhase*10;
        u16RPM/=RMP_RATE;
        // store result
        *pu8RPM=(u8)u16RPM;

        // restart sequence
        u8TimerRPM=0;
    }
    else
    {
        u8TimerRPM++;
    }
}

