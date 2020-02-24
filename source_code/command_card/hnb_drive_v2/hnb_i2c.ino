//****************************************************************************
// I2C library
//****************************************************************************
Adafruit_ADS1015 adc12bits;  /* Use this for the 12-bit version */
Adafruit_MCP4725 dac12bits;   //adresse i2C $60
MCP7940_Class     MCP7940;       

void setup_i2c() {

  // --------------------------------------------------Initialisation du DAC 
  
  Heltec.begin(true /*DisplayEnable Enable*/, false /*LoRa Disable*/, true /*Serial Enable*/);
  
  // --------------------------------------------------Initialisation du DAC 
  dac12bits.begin(0x60);    //12 bits 4096 1,22mV/bit
  // --------------------------------------------------Initialisation de l'horloge interne 
  MCP7940.begin();
  MCP7940.adjust(); 
  enum SquareWaveTypes { Hz1, kHz4, kHz8, kHz32, Hz64 }  ;
  MCP7940.setSQWSpeed(Hz1);
  MCP7940.adjust(DateTime(2020,02,07,00,00,00)); 

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

  // a hall sensor ACS 713 is used to measure de supply current. Its sensisbility is 133mV/A
  //  ACS713-30A  , 

  adc12bits.begin();
  // Lecture du courant de batterie au démarrage
  for (int i=0;i<10;i++)
     u16CourantMoteurInitial=u16CourantMoteurInitial+adc12bits.readADC_SingleEnded(1);
  u16CourantMoteurInitial=u16CourantMoteurInitial/10;
}

int16_t litVitesseRoue() { // vaut 0 ou 4095... C'est le calcul effectue dans le loop qui permet de déduire la vitesse du vélo
  return adc12bits.readADC_SingleEnded(3);
}

int16_t litBatterie() {
  return (adc12bits.readADC_SingleEnded(0))/22; 
  //gainADC*1000/92=
}

int16_t litCourant() { // Valeur du courant en mA 
  return (adc12bits.readADC_SingleEnded(1)-u16CourantMoteurInitial)*15;
  //15=gainADC*1000/gain capteur 
}

int16_t litPedalier() {
  return adc12bits.readADC_SingleEnded(2);
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
