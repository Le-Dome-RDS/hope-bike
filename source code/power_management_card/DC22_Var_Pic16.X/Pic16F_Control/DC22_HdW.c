/*
		File:   DC22_Hdw.c
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
#include "PrjDefine.h"

void DC22HdwInit(void)
{
#ifdef PIC12F1822
    TRISA2=0; // RA2 Output
#endif

#ifdef PIC16F1823
    TRISC3=0; // RC3 Output
    TRISC1=0; // RC1 Output
#endif

#ifdef PIC16F1823_INTERNAL
    TRISC3=0; // RC3 Output
    TRISC1=0; // RC1 Output
#endif

#ifdef PIC16F1509
#endif

}