// Etat possible des boutons
#define         PAS_APPUYE  0
#define         APPUYE      1
#define         APPUYE_LONG 2
#define         RELACHE     3
// pin des boutons
#define BUTTON_GAUCHE 12 
#define BUTTON_MILIEU 14
#define BUTTON_DROIT 27

byte             u8BoutonGauche=PAS_APPUYE;          // Est ce que le bouton gauche est enfoncé 
byte             u8BoutonDroit=PAS_APPUYE;           //droit 
byte             u8BoutonMilieu=PAS_APPUYE;          // milieu

unsigned long TEMPS_ANTI_REBOND = 50;    // the debounce time; increase if the output flickers
unsigned long TEMPS_APPUI_LONG=2000;
unsigned long TEMPS_ATTENTE=2000;

unsigned long tempsGauche=0;
unsigned long tempsDroit=0;
unsigned long tempsMilieu=0;

int lectureGaucheAvant=HIGH;
int lectureDroitAvant=HIGH;
int lectureMilieuAvant=LOW;

//****************************************************************************
// GESTION DU CLAVIER DE COMMANDE
//****************************************************************************

void setup_keyboard(){
  pinMode(BUTTON_GAUCHE, INPUT);
  pinMode(BUTTON_DROIT, INPUT);
  pinMode(BUTTON_MILIEU, INPUT);
  
}


void gestionClavierMilieu() {
  //lecture==LOW lorsqu'on appuit : à modifier en fonction de l'interrupteur.
  int lectureMilieu=digitalRead(BUTTON_MILIEU);
  if ((lectureMilieu==LOW)&&(lectureMilieuAvant==HIGH)&&(u8BoutonMilieu==PAS_APPUYE))tempsMilieu=millis();
  if (((millis()-tempsMilieu)>TEMPS_ANTI_REBOND) && (lectureMilieu==LOW) &&(u8BoutonMilieu==PAS_APPUYE))u8BoutonMilieu=APPUYE;
  if (((millis()-tempsMilieu)>TEMPS_APPUI_LONG) && (lectureMilieu==LOW) &&(u8BoutonMilieu==APPUYE)) u8BoutonMilieu=APPUYE_LONG;
  if (((millis()-tempsMilieu)>TEMPS_ANTI_REBOND) && (lectureMilieu==HIGH)&&(u8BoutonMilieu==APPUYE))u8BoutonMilieu=RELACHE;
  
  lectureMilieuAvant=lectureMilieu;
  
}

void gestionClavierDroit() {
  //lecture=LOW lorsqu'on appuit
  int lectureDroit=digitalRead(BUTTON_DROIT);
  if ((lectureDroit==LOW)&&(lectureDroitAvant==HIGH)&&(u8BoutonDroit==PAS_APPUYE))tempsDroit=millis();
  if (((millis()-tempsDroit)>TEMPS_ANTI_REBOND) && (lectureDroit==LOW) &&(u8BoutonDroit==PAS_APPUYE))u8BoutonDroit=APPUYE;
  if (((millis()-tempsDroit)>TEMPS_APPUI_LONG) && (lectureDroit==LOW) &&(u8BoutonDroit==APPUYE)) u8BoutonDroit=APPUYE_LONG;
  if (((millis()-tempsDroit)>TEMPS_ANTI_REBOND) && (lectureDroit==HIGH)&&(u8BoutonDroit==APPUYE))u8BoutonDroit=RELACHE;
  lectureDroitAvant=lectureDroit;
}

void gestionClavierGauche() {
  //lecture==LOW lorsqu'on appuit : à modifier en fonction de l'interrupteur.
  int lectureGauche=digitalRead(BUTTON_GAUCHE);
  if ((lectureGauche==LOW)&&(lectureGaucheAvant==HIGH)&&(u8BoutonGauche==PAS_APPUYE))tempsGauche=millis();
  if (((millis()-tempsGauche)>TEMPS_ANTI_REBOND) && (lectureGauche==LOW) &&(u8BoutonGauche==PAS_APPUYE))u8BoutonGauche=APPUYE;
  if (((millis()-tempsGauche)>TEMPS_APPUI_LONG) && (lectureGauche==LOW) &&(u8BoutonGauche==APPUYE)) u8BoutonGauche=APPUYE_LONG;
  if (((millis()-tempsGauche)>TEMPS_ANTI_REBOND) && (lectureGauche==HIGH)&&(u8BoutonGauche==APPUYE))u8BoutonGauche=RELACHE;
  lectureGaucheAvant=lectureGauche;

}

void gestionClavier(){
  gestionClavierDroit();
  gestionClavierGauche();
  gestionClavierMilieu();
  if ((MODE==0)||(MODE==1)) {
  // gestion de la vitesse
  if ((u8BoutonGauche==RELACHE)&&(speedLevel>=1)) {speedLevel--;u8BoutonGauche=PAS_APPUYE;}
  if ((u8BoutonDroit==RELACHE)&&(speedLevel<=3)) {speedLevel++;u8BoutonDroit =PAS_APPUYE;}
  if (u8BoutonMilieu==RELACHE) {if (DEBUG==1)DEBUG=0; else DEBUG=1;u8BoutonMilieu=PAS_APPUYE;}
  if (u8BoutonMilieu==APPUYE_LONG) {bPedalage=false;u8BoutonMilieu=PAS_APPUYE;}
  
  // Fonction speciale
  if (u8BoutonDroit==APPUYE_LONG) {speedLevel=4; u8BoutonDroit=PAS_APPUYE;}
  if (u8BoutonGauche==APPUYE_LONG) {speedLevel=0; u8BoutonGauche=PAS_APPUYE;}
  
  } else if (MODE==3){ 
  // gestion de la vitesse
      if (u8BoutonGauche==RELACHE) {i16PWM=i16PWM-50;u8BoutonGauche=PAS_APPUYE;}
      if (u8BoutonDroit==RELACHE)  {i16PWM=i16PWM+50;u8BoutonDroit =PAS_APPUYE;}
      if (u8BoutonDroit==APPUYE_LONG) {i16PWM=0;u8BoutonDroit=PAS_APPUYE;}
  }
  
  
  
}
