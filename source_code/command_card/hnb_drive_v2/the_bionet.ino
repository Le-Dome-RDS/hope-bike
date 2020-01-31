//****************************************************************************
// I2C library
//****************************************************************************


void gestion_i2c() {
 
  //----------------------------
  // ECRITURES
  //----------------------------

  // write Commande moteur
  // i8PWM is the speed control voltage of the BLDC drive
  // if (IsOkWriteByteVar(1,i8PWM)) blink_display();
 
  //----------------------------
  // LECTURES
  //----------------------------

  // lire le courant moteur
  
  //if (IsOkReadUIntVar(2,&u16CourantMoteur)) blink_display();
  
  // lire la vitesse de pédalage
  // if (IsOkReadByteVar(7,&u8VitessePedale)) blink_display();
  
  // lire la vitesse de rotation de la roue
  //if (IsOkReadByteVar(8,&u8RotationRoue)) blink_display();
  
  // lire le niveau batterie
  //if (IsOkReadUIntVar(15,&u16NiveauBatterie)) blink_display();
  
}

void test_i2c() {
 
}
