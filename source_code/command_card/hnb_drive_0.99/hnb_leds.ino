//****************************************************************************
// GESTION DE L'AFFICHAGE DES LEDS
//****************************************************************************

#define LEFT_EYE  8
#define RIGHT_EYE 9
byte eyeBrows[]   = {2,3,4,5,6,7};
byte mouth[]      = {13,12,11,10};

// setBrows : depending of level, 1 to 6 leds are on or not
void setBrows(int level) {
  for (int i=0;i<6;i++) digitalWrite(eyeBrows[i], (i<level) ? HIGH : LOW);
  //for (int i=0;i<5;i++) digitalWrite(eyeBrows[i], (i<level) ? HIGH : LOW);
}

// setMouth : depending of level, 1 to 4 leds are on or not
void setMouth(int level) {
  for (int i=0;i<4;i++) digitalWrite(mouth[i], (i<level) ? HIGH : LOW);
}

void blinkOnceLeft(byte t) {
  digitalWrite(LEFT_EYE,HIGH);
  delay(t);
  digitalWrite(LEFT_EYE,LOW);
}

void blinkOnceRight(byte t) {
  digitalWrite(RIGHT_EYE,HIGH);
  delay(t);
  digitalWrite(RIGHT_EYE,LOW);
}

void blinkMouth(byte n, byte t) {
  digitalWrite(mouth[n],HIGH);
  delay(t);
  digitalWrite(mouth[n],LOW);
}

void blinkBrows(byte n, byte t) {
  digitalWrite(eyeBrows[n],HIGH);
  delay(t);
  digitalWrite(eyeBrows[n],LOW);
}


//-----------------------------------------------------------------------------
// INTIALISE LES LEDS AVEC UN VISAGE SYMPATHIQUE
//-----------------------------------------------------------------------------

void initDisplay() {
  // configure led pin to output
  for (int i=0;i<6;i++) pinMode(eyeBrows[i], OUTPUT); 
  for (int i=0;i<4;i++) pinMode(mouth[i], OUTPUT); 
  pinMode(LEFT_EYE, OUTPUT); 
  pinMode(RIGHT_EYE, OUTPUT);

  setBrows(0);
  setMouth(0);
  digitalWrite(LEFT_EYE,LOW);
  digitalWrite(RIGHT_EYE,LOW);
}

void testDisplay() {
  setBrows(6);
  setMouth(4);
  digitalWrite(LEFT_EYE,HIGH);
  digitalWrite(RIGHT_EYE,HIGH);

  delay(2000);

  setBrows(0);
  setMouth(0);
  digitalWrite(LEFT_EYE,LOW);
  digitalWrite(RIGHT_EYE,LOW);
}
