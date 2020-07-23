//****************************************************************************
// OLED screen
//****************************************************************************


void afficheEtat(){
  if (MODE==1){
  Heltec.display->drawString(0, 0,"Mode:");
  switch(u8Etat){
    case 0: Heltec.display->drawString(60, 0, "Arret");break;
    case 1: Heltec.display->drawString(60, 0, "Demmar.");break;
    case 2: Heltec.display->drawString(60, 0, "augm.e PWM");break;
    case 3: Heltec.display->drawString(60, 0, "dim. PWM");break;
    case 4: Heltec.display->drawString(60, 0, "regul.");break;
    case 5: Heltec.display->drawString(60, 0, "Err.");
       Heltec.display->drawString(100, 0, String(u8Erreur));
       break;
    
    }
  }
}

void affichePWM(){
  if ((MODE==1)||(MODE==3)){
  Heltec.display->drawString(0, 10,"PWM:");
  Heltec.display->drawString(50, 10, String(i16PWM));
  }
}

void afficheConsigne(){
  switch(MODE) {
    case 0:
     Heltec.display->drawProgressBar(5,3,118,6,speedLevel*33);
    break;
    default:
     Heltec.display->drawString(60, 10,"Consigne:");
     Heltec.display->drawString(110, 10, String(speedLevel));
    break;
  }
}


void afficheBatterie(){
  if ((MODE==1)||(MODE==3)){
     Heltec.display->drawString(0, 20,"Batterie:");
     Heltec.display->drawString(60, 20, String(u16Batterie));
     Heltec.display->drawString(100, 20,"mV");
  } else
  if (MODE==0) {
    Heltec.display->drawLine(118,59,103,59);
    Heltec.display->drawLine(103,59,103,29);
    Heltec.display->drawLine(103,29,108,29);
    Heltec.display->drawLine(108,29,108,24);
    Heltec.display->drawLine(108,24,113,24);
    Heltec.display->drawLine(113,24,113,29);
    Heltec.display->drawLine(113,29,118,29);
    Heltec.display->drawLine(118,29,118,59);
    for (int i=0;i<10;i++) {
     if (30000+i*1000<=u16Batterie)    Heltec.display->drawLine(116,57-i*3,105,57-i*3);
    }
  }

}

void afficheCourant(){
  if ((MODE==1)||(MODE==3)){
  Heltec.display->drawString(0, 30,"Courant:");
     //if ((u16Courant<0)||(u16Courant>30000)) Heltec.display->drawString(60, 30, "Erreur");
     //else {
      Heltec.display->drawString(60, 30, String(u16Courant));
     Heltec.display->drawString(100, 30,"mA");
  }
  else 
  if (MODE==0) {
   Heltec.display->drawProgressBar(5,10,118,10,u16Courant/100);   
   Heltec.display->setFont(ArialMT_Plain_24);
   Heltec.display->drawString(20, 24, String(u16Courant/100));
   Heltec.display->setFont(ArialMT_Plain_10);
   Heltec.display->drawString(20, 48, "x100 mA");
  }
}
void afficheVitesseVelo(){
  if (MODE==1){
  Heltec.display->drawString(0, 40,"Vitesse:"); 
    Heltec.display->drawString(60, 40,String(u16Vitesse));
    Heltec.display->drawString(100, 40, "m/h");
    }
  else
  if (MODE==0){
    Heltec.display->setFont(ArialMT_Plain_24);
    Heltec.display->drawString(70, 24, String(u16Vitesse/1000));
    Heltec.display->setFont(ArialMT_Plain_10);
    Heltec.display->drawString(70, 48, "km/h");
  }
  else 
  if (MODE==3){
    Heltec.display->setFont(ArialMT_Plain_24);
    Heltec.display->drawString(70, 24, String(u16Vitesse/1000));
    Heltec.display->setFont(ArialMT_Plain_10);
    Heltec.display->drawString(70, 48, "km/h");
  }
}

void affichePedalier(){
  if ((MODE==1)){
  Heltec.display->drawString(0, 50,"Pedalier:");
  Heltec.display->drawString(60, 50,String(u16Pedalier));
  if (bPedalage) Heltec.display->drawString(100, 50, "*");   
  }
  else 
  if (MODE==0) {
   if (bPedalage) Heltec.display->drawString(0, 50, "*");   
  
  }
}



void setup_display() {
  //
  // Heltec.display->init();
  
  
  //Heltec.display->mirrorScreen();
  
  Heltec.display->setFont(ArialMT_Plain_16);
  Heltec.display->setTextAlignment(TEXT_ALIGN_CENTER);
  Heltec.display->clear();
  Heltec.display->resetOrientation();
  Heltec.display->displayOn();
  Heltec.display->setBrightness(255);
  
  Heltec.display->flipScreenVertically();
  
  Heltec.display->drawString(64, 4, "Hope");
  Heltec.display->drawString(64, 20, "&");
  Heltec.display->drawString(64, 36, "Bike");
  Heltec.display->display();
  Heltec.display->setTextAlignment(TEXT_ALIGN_LEFT);
 delay(1000);
 Heltec.display->setFont(ArialMT_Plain_10);
  pinMode(PIN_LED, OUTPUT);
}

void test_display() {
  
}
