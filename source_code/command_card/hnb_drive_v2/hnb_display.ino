//****************************************************************************
// OLED screen
//****************************************************************************
void blink_once_display(){
}


void afficheEtat(){
  if (MODE==1){
  Heltec.display->drawString(0, 0,"Mode:");
  switch(u8Etat){
    case 0: Heltec.display->drawString(60, 0, "Arret");break;
    case 1: Heltec.display->drawString(60, 0, "Demmar.");break;
    case 2: Heltec.display->drawString(60, 0, "augm.e PWM");break;
    case 3: Heltec.display->drawString(60, 0, "dim. PWM");break;
    case 4: Heltec.display->drawString(60, 0, "regul.");break;
    case 5: Heltec.display->drawString(60, 0, "Erreur.C");break;
    }
  }
}

void afficheCommande(){
  if (MODE==1){
  Heltec.display->drawString(0, 10,"Commande:");
  Heltec.display->drawStringMaxWidth(60, 10, 6,String(u16PWM));
  }
}


void afficheBatterie(){
  if (MODE==1){
     Heltec.display->drawString(0, 20,"Batterie:");
     Heltec.display->drawStringMaxWidth(60, 20, 6,String(u16Batterie));
     Heltec.display->drawString(100, 20,"mV");
  }

}

void afficheCourant(){
  if (MODE==1){
  Heltec.display->drawString(0, 30,"Courant:");
     if ((u16Courant<0)||(u16Courant>30000)) Heltec.display->drawString(60, 30, "Erreur");
     else {Heltec.display->drawStringMaxWidth(60, 30,6, String(u16Courant));
     Heltec.display->drawString(100, 30,"mA");}
  }
}
void afficheVitesseVelo(){
  if (MODE==1){
  Heltec.display->drawString(0, 40,"Vitesse:"); 
  if ((u16VitesseVelo>40000)||(u16VitesseVelo<0))Heltec.display->drawString(60, 40,"Erreur ");
  else 
    {
    Heltec.display->drawStringMaxWidth(60, 40, 6,String(u16VitesseVelo));
    Heltec.display->drawString(100, 40, "m/h");
    }
  }
}

void affichePedalier(){
  if (MODE==1){
  Heltec.display->drawString(0, 50,"Pedalier:");
  Heltec.display->drawStringMaxWidth(60, 50, 6,String(u16Pedalier));
  if (bPedalage) Heltec.display->drawString(100, 50, "*");   
  }
}

void afficheConsigne(){
  if (MODE==1){
  Heltec.display->drawString(0, 20,"Consigne:");
  Heltec.display->drawStringMaxWidth(60, 20,6, String(u16Consigne));
  Heltec.display->drawString(100, 20, "m/h");
  //if (bPedalage) Heltec.display->drawString(100, 50, "Pédalage");   
  }
}



void setup_display() {
  //
  Heltec.display->init();
  Heltec.display->flipScreenVertically();
  Heltec.display->setFont(ArialMT_Plain_10);
  Heltec.display->setTextAlignment(TEXT_ALIGN_CENTER);
  Heltec.display->clear();
  Heltec.display->drawString(60, 20, "Hope");
  Heltec.display->drawString(60, 30, "&");
  Heltec.display->drawString(60, 40, "Bike");
  
  Heltec.display->display();
  Heltec.display->setTextAlignment(TEXT_ALIGN_LEFT);
 delay(1000);
  pinMode(PIN_LED, OUTPUT);
}

void test_display() {
  
}
