//****************************************************************************
// I2C library
//****************************************************************************
Adafruit_ADS1015 adc12bits;  /* Use this for the 12-bit version */
Adafruit_MCP4725 dac12bits;   //adresse i2C $60
MCP7940_Class     MCP7940;       

void setup_i2c() {

  // --------------------------------------------------Initialisation du DAC 
  
  Heltec.begin(true /*DisplayEnable Enable*/, false /*LoRa Disable*/, true /*Serial Enable*/);
  Heltec.display->displayOff();
  // --------------------------------------------------Initialisation du DAC 
  dac12bits.begin(0x60);    //12 bits 4096 1,22mV/bit
  dac12bits.setVoltage(0,true);
  delay(100);
  // --------------------------------------------------Initialisation de l'horloge interne 
  MCP7940.begin();
  MCP7940.adjust(); 
  enum SquareWaveTypes { Hz1, kHz4, kHz8, kHz32, Hz64 }  ;
  MCP7940.setSQWSpeed(Hz1);
  MCP7940.adjust(DateTime(2020,02,07,00,00,00)); 
  delay(100);
  // --------------------------------------------------Initialisation de l'ADC 

  // The ADC input range (or gain) can be changed via the following
  // functions, but be careful never to exceed VDD +0.3V max, or to
  // exceed the upper and lower limits if you adjust the input range!
  // Setting these values incorrectly may destroy your ADC!
  //                                                                ADS1015 12b 4096  x16  ADS1115 16b 65535
  //                                                                -------  -------
  // ads.setGain(GAIN_TWOTHIRDS);  // 2/3x gain +/- 6.144V    1 bit = 3mV      0.1875mV (default)
  adc12bits.setGain(GAIN_ONE);        // 1x gain   +/- 4.096V       1 bit = 2mV      0.125mV    /16 x2 = 8
  //  ads.setGain(GAIN_TWO);        // 2x gain   +/- 2.048V   1 bit = 1mV      0.0625mV
  // ads.setGain(GAIN_FOUR);       // 4x gain   +/- 1.024V    1 bit = 0.5mV    0.03125mV
  // ads.setGain(GAIN_EIGHT);      // 8x gain   +/- 0.512V    1 bit = 0.25mV   0.015625mV
  // ads.setGain(GAIN_SIXTEEN);    // 16x gain  +/- 0.256V    1 bit = 0.125mV  0.0078125mV

  // with this setup, we have 2mV per bit for the ADC

  // a hall sensor ACS 713 is used to measure de supply current. Its sensisbility is 133mV/A with 0.5 V for I=0
  //Vout=0.5+0.133*I
  
  //  ACS713-30A  , 
  adc12bits.begin();
  Wire.setClock(50000);
  delay(1000);
  
  u16CourantMoteurInitial=250; //0.5V en sortie du capteur orsque le courant est nul.
}




uint16_t litVitesseRoue() { // vaut 0 ou 4095... C'est le calcul effectue dans le loop qui permet de déduire la vitesse du vélo
  uint16_t tmp;
  if (MODE!=3) {if (VERSION_HARD==1) tmp=adc12bits.readADC_SingleEnded(3);
               else if (VERSION_HARD==2) tmp=adc12bits.readADC_SingleEnded(2);}
  if (MODE==3) {
    tmp=0;
    if (k%1==0) tmp=1000;
    if (k%250==0) tmp=1000;
  }
  return tmp;
}



uint16_t litBatterie() {
  uint16_t calcul;
  calcul=adc12bits.readADC_SingleEnded(0)*1000/46;

  // pour k=0; on doit initialiser la variable : 
  
   
  // Il y a une erreur si la tension mesurée est supérieure à 42V ou bien que la différence entre 2 mesures successives est supérieure à 5V 
  //if (k>0) {
  // if ((calcul>25000)&&(calcul<42000)&&(abs(u16Batterie-calcul)<5000)) {u8CompteurErreurBatterie=0; }
  // else {u8CompteurErreurBatterie++;calcul=u16Batterie;}
  // if (u8CompteurErreurBatterie>10) u8Erreur=ERREUR_BATTERIE;
  // }
  return(calcul);
  }

 uint16_t litCourant() { // Valeur du courant en mA 
   uint16_t calcul;
  //calcul = adc12bits.readADC_SingleEnded(1);
  // courant=(N_mesure-N_mesure0)/(0.133*2) pour un résultat en mA.
   calcul = ((adc12bits.readADC_SingleEnded(1)-250)*14);
   //if (k>0) {
   // il y a une erreur si le courant mesuré est supéreieur à 35 A et que a différence entre 2 mesures de courant est supérieure à 10A
   //if ((calcul <20000) && (abs(calcul-u16Courant)<10000)) {u8CompteurErreurCourant=0;}
   //else {u8CompteurErreurCourant++;calcul=u16Courant;}
   // if (u8CompteurErreurCourant>10) u8Erreur=ERREUR_COURANT;  
   //}
   return(calcul);
  
}

 uint16_t litPedalier() {
  uint16_t calcul;
  if (VERSION_HARD==1) calcul=adc12bits.readADC_SingleEnded(2);
  else if (VERSION_HARD==2) calcul=adc12bits.readADC_SingleEnded(3 );
  return calcul;
  // il y a une erreur si la mesure est supérieur à 2500 et si la différence entre 2 mesures est supérieur à 500
  
  //if ((calcul <2500) && (abs(calcul-u16Pedalier)<2000)) {u8CompteurErreurPedalier=0;return calcul;}
  //else u8CompteurErreurPedalier++;
  //if (u8CompteurErreurPedalier>10) u8Erreur=ERREUR_PEDALIER;  
  
}



void test_i2c() {
 for (byte i=1;i<127;i++){
  Wire.beginTransmission(i);
  if (Wire.endTransmission()==0) {
    // Something has been found
    Heltec.display->drawString(0, 20, "found");
    }
  }
}
