/*
		File:   DC22_GlobalVar.c
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
#include "Type.h"

u8 u8ConsigneMotor=0;
u8 u8ConsigneMotorTimeout=0;
u8 u8CourantBatt=0;
u8 u8CourantBattTimeout=0;
u8 u8Pedalage=0;
u8 u8RpmMotor=0;
u16 u16SeuilBatterie=(u16)(360*0.9); //90% de 36V exprimé en 1/10V
u16 u16TensionBatterie=0;
