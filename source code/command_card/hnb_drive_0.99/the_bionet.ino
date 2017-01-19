//****************************************************************************
// BIBLIOTHEQUE BIONET
//****************************************************************************

#define INTERFRAME 5

/****************************** 
 * read a byte type variable  *
 ******************************/

boolean IsOkReadByteVar(unsigned char VarNumber, unsigned char * ByteRead) {
  
  int incomingByte;
  int ReceiveData;
  unsigned char RxCheckSum;
  unsigned char CheckSum;
  boolean ReadError=false;
    
  Serial.write(VarNumber);  
  delay(INTERFRAME);
  
  // receive the byte
  incomingByte=Serial.read();   // the Var Number of question
  if (incomingByte != (int)VarNumber) ReadError=true;
 
  incomingByte=Serial.read();   // the Var Number of answer
  if (incomingByte != (int)VarNumber) ReadError=true;
  
  ReceiveData = Serial.read();  // data
  RxCheckSum  = Serial.read();  // checksum
  
  CheckSum =(unsigned char)(~(VarNumber+ReceiveData));

  if (RxCheckSum != CheckSum) {
    // checksum error
    ReadError=true;
    *ByteRead=0;
    }
  
  else {
    // return the Byte value
    *ByteRead=(unsigned char )ReceiveData;
  }
  
  return (ReadError);
}

/******************************
 * read a uint type variable  *
 ******************************/

boolean IsOkReadUIntVar(unsigned char VarNumber, unsigned int * UIntRead) {
  int incomingByte;
  int ReceiveData;
  int ReceiveDataHigh;
  unsigned char RxCheckSum;
  unsigned char CheckSum;
  boolean ReadError=false;
  
  Serial.write(VarNumber);
  
  delay(INTERFRAME);
  // receive the byte
  incomingByte = Serial.read(); // the VarNumber of question
  if (incomingByte != VarNumber) ReadError=true;
  
  incomingByte = Serial.read(); // the VarNumber of answer
  if (incomingByte != VarNumber) ReadError=true;
  
  ReceiveData = Serial.read();     // data
  ReceiveDataHigh = Serial.read(); // data
  RxCheckSum = Serial.read();      // chechsum
  
  CheckSum =(unsigned char)(~(VarNumber + ReceiveData + ReceiveDataHigh));
  if (RxCheckSum != CheckSum) {
    ReadError=true;
    *UIntRead=0;
  }
  else {
    // renvoie la valeur lue
    *UIntRead= (unsigned int)ReceiveDataHigh*256 + (unsigned int)ReceiveData;  //TODO faire l'op en binaire
  }
  
  return (ReadError);
}


/****************************** 
 * write a type byte variable * 
 ******************************/
 
boolean IsOkWriteByteVar(unsigned char VarNumber, unsigned char Byte) {
  int incomingByte;
  unsigned char CheckSum;
  boolean ReadError=false;
  
  CheckSum=VarNumber+128;
  CheckSum+=Byte;
  CheckSum=~CheckSum;
  
  Serial.write(VarNumber+128);
  Serial.write(Byte);
  Serial.write(CheckSum);
  delay(INTERFRAME);
  
  incomingByte=Serial.read(); // the VarNumber+128 of question
  if (incomingByte != (VarNumber+128)) ReadError=true;
  
  incomingByte=Serial.read(); // the byte of question
  if (incomingByte != Byte) ReadError=true;

  incomingByte=Serial.read(); // the checksum of question
  if (incomingByte != CheckSum) ReadError=true;
  
  incomingByte=Serial.read(); // the Write OK of ansdwer
  if (incomingByte != 0xA5) ReadError=true;
  
  return (ReadError);
}

/****************************** 
 * send Run command           * 
 ******************************/

void SendRun(void) {
  Serial.write(0x80);
  Serial.write(0x01);
  Serial.write(0x00);
  Serial.write(0x7E);
  delay(INTERFRAME);
}

/****************************** 
 * Flush Serial Buffer        * 
 ******************************/
 
void FlushSerialBuff() {
  int incomingByte;  
  do {
    incomingByte=Serial.read();
  }
  while (incomingByte != -1);
}

/****************************** 
 * Echanges BIONET            * 
 ******************************/
 
void gestionBionet() {
  FlushSerialBuff();
  SendRun();
  FlushSerialBuff();

  //----------------------------
  // ECRITURES
  //----------------------------

  // write Commande moteur
  if (IsOkWriteByteVar(1,i8PWM)) blinkOnceRight(1);
  FlushSerialBuff();

  //----------------------------
  // LECTURES
  //----------------------------

  // lire le courant moteur
  if (IsOkReadUIntVar(2,&u16CourantMoteur)) blinkOnceRight(1);
  FlushSerialBuff();

  // lire la vitesse de pédalage
  if (IsOkReadByteVar(7,&u8VitessePedale)) blinkOnceRight(1);
  FlushSerialBuff();

  // lire la vitesse de rotation de la roue
  if (IsOkReadByteVar(8,&u8RotationRoue)) blinkOnceRight(1);
  FlushSerialBuff();

  // lire le niveau batterie
  if (IsOkReadUIntVar(15,&u16NiveauBatterie)) blinkOnceRight(1);
  FlushSerialBuff();
}

void testBionet() {
  FlushSerialBuff();
  SendRun();
  FlushSerialBuff();

  //----------------------------
  // ECRITURES
  //----------------------------

  if (IsOkWriteByteVar(1,i8PWM)) blinkBrows(5,1);
  FlushSerialBuff();

  //----------------------------
  // LECTURES
  //----------------------------

  // Courant Moteur Valeur Brut Capteur 10bit 0 à 1023
  if (IsOkReadUIntVar(2,&u16CourantMoteur)) blinkBrows(0,1);
  FlushSerialBuff();

  // Capteur de pédalage DC22 Brut nombre de fronts en 0,5s valeur saturée à 255
  if (IsOkReadByteVar(7,&u8VitessePedale)) blinkBrows(1,1);
  FlushSerialBuff();

  // Valeur Brut rotation roue en nombre de fronts de la phase A en 0,5s max 255 équivalent de 255Hz (2 fronts par pertiode )
  if (IsOkReadByteVar(8,&u8RotationRoue)) blinkBrows(2,1);
  FlushSerialBuff();

  // Valeur Batterie Brut Capteur 10bit 0 à 2047
  // B15 = 0 Batterie OK moteur activable ou activé
  // B15 = 1 Batterie < Seuil et Moteur Arrêté
  // B0 à B14 valeur tension batterie en 1/10V
  if (IsOkReadUIntVar(15,&u16NiveauBatterie)) blinkBrows(3,1);
  FlushSerialBuff();
}
