//****************************************************************************
// OLED screen
//****************************************************************************
void blink_once_display(){
}


void afficheEtat(){
  if (MODE==1){
  Heltec.display->drawString(0, 0,"Mode:");
  Heltec.display->drawString(60, 0, String(u8Etat));
  }
}

void afficheCommande(){
  if (MODE==1){
  Heltec.display->drawString(0, 10,"Commande:");
  Heltec.display->drawString(60, 10, String(u16PWM));
  }
}


void afficheBatterie(){
  if (MODE==1){
     Heltec.display->drawString(0, 20,"Batterie:");
     Heltec.display->drawString(60, 20, String(u16Batterie));
     Heltec.display->drawString(100, 20,"mV");
  }

}

void afficheCourant(){
  if (MODE==1){
  Heltec.display->drawString(0, 30,"Courant:");
     Heltec.display->drawString(60, 30, String(u16Courant));
     Heltec.display->drawString(100, 30,"mA");
  }
}
void afficheVitesseVelo(){
  if (MODE==1){
  Heltec.display->drawString(0, 40,"Vitesse:"); 
  Heltec.display->drawString(60, 40, String(u16VitesseVelo));
  Heltec.display->drawString(100, 40, "m/h");
  
  }
}

void affichePedalier(){
  if (MODE==1){
  Heltec.display->drawString(0, 50,"Pedalier:");
  Heltec.display->drawString(60, 50, String(u16Pedalier));
  if (bPedalage) Heltec.display->drawString(100, 50, "Pédalage");   
  }
}


void setup_display() {
  Heltec.display->flipScreenVertically();
  Heltec.display->setFont(ArialMT_Plain_10);
  Heltec.display->setTextAlignment(TEXT_ALIGN_LEFT);
}

void test_display() {
  
}
