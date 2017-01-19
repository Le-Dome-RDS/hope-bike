/*
 * File:   DC24_GlobalVar.c
 * Author: francois
 *
 * Created on 31 octobre 2014, 18:17
 */

#include <stdio.h>
#include <stdlib.h>
#include "Type.h"

u8 Tempo500ms=0;

u8 u8Pedalage=0;
u8 u8PedalageCpt=0;
boolean bPedalageLevelPrev=FALSE;

u8 u8RPM=0;
u8 u8RPMCpt=0;
boolean bRPMLevelPrev=FALSE;

u16 u16TensionBatterie=0;
u16 u16CourantBatt=0;

