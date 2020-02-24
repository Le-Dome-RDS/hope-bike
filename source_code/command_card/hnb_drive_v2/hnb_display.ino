//****************************************************************************
// OLED screen
//****************************************************************************
void blink_once_display(){
}


void afficheMode(int8_t mode){
  if (MODE==1){
  Heltec.display->drawString(0, 0,"Mode:");
  Heltec.display->drawString(60, 0, String(mode));
  }
}

void afficheCommande(int8_t commande){
  if (MODE==1){
  Heltec.display->drawString(0, 10,"Commande:");
  Heltec.display->drawString(60, 10, String(commande));
  }
}


void afficheBatterie(int16_t batterie){
  if (MODE==1){
     Heltec.display->drawString(0, 20,"Batterie:");
     Heltec.display->drawString(60, 20, String(batterie));
     Heltec.display->drawString(80, 20,"V");
  }

}

void afficheCourant(int16_t courant){
  if (MODE==1){
  Heltec.display->drawString(0, 30,"Courant:");
     Heltec.display->drawString(60, 30, String(courant));
     Heltec.display->drawString(80, 30,"mA");
  }
}
void afficheVitesseVelo(int16_t vitesse){
  if (MODE==1){
  Heltec.display->drawString(0, 40,"Vitesse:"); 
  Heltec.display->drawString(60, 40, String(vitesse));
  Heltec.display->drawString(80, 40, "km/h");
  
  }
}

void affichePedalier(int16_t pedalier){
  if (MODE==1){
  Heltec.display->drawString(0, 50,"Pedalier:");
  Heltec.display->drawString(60, 50, String(pedalier));
  if (pedalage) Heltec.display->drawString(80, 50, "Pédalage");   
  }
}



void setup_display() {
  Heltec.display->flipScreenVertically();
  Heltec.display->setFont(ArialMT_Plain_10);
  Heltec.display->setTextAlignment(TEXT_ALIGN_LEFT);
}

void test_display() {
  
}
