//****************************************************************************
// GESTION DU MOTEUR ET DE LA BATTERIE
//****************************************************************************
//

// Definition 

#define NB_LED_MOUTH              4    // Nombre de led disponible pour l'affichage du niveau de batterie

#define COURANT_REPOS             602  // JMR : ses valeurs sont déduites de mesure effectuées sur le vélo 1
// TBD : Il faudrait mesurer le courant lorsque le moteur est à l'arret...
#define COURANT_RESOLUTION        18  //      


#define ETAT_ARRET                0
#define ETAT_DEMARRAGE            1
#define ETAT_REGULATION           2

#define ETAT_BUG                  3


/* Les valeurs limites qui gère la machine d'etat */

#define PEDALE_LIMITE             2   // est ce qu'il y a un pedalage ?
#define COURANT_MOTEUR_MAX        130  // valeur max du courant que peux fournir la batterie
#define COURANT_MOTEUR_LIMITE_1   60  // valeur limite du courant en fonctionnement en vitesse 1
#define COURANT_MOTEUR_LIMITE_2   80  // valeur limite du courant en fonctionnement en vitesse 2
#define COURANT_MOTEUR_LIMITE_3   100  // valeur limite du courant en fonctionnement en vitesse 3

#define COURANT_DEMARRAGE_LIMITE  30  // valeur limite du courant au demmarrage 
#define PWM_DEMARRAGE             7   // valeur du PWM lorsque le velo demarre
#define VITESSE_DEMARRAGE_LIMITE  8  // vitesse de la roue à partir de laquelle le velo accelere j'aurai aimé imposer 20 mais limité à 8 ??
#define VITESSE_REGULATION_SEUIL  5   // ecart de vitesse par rapport à la consigne qui provoque la regulation de la vitesse


//-----------------------------------------------------------------------------
// Variable globale
//-----------------------------------------------------------------------------


byte            u8Etat              = ETAT_ARRET; // Etat du velo

byte            u8VitessePedale    = 0;          // INFO BIONET vitesse de pédalage 

byte            u8RotationRoue     = 0;          // INFO BIONET vitesse de rotation de la roue     

// chronometre pour mettre à zero la consigne si l'utilisateur ne pedale pas pendant plus de 5 secondes
boolean         bChronoArretPedalage         =  false;      // est ce que le chronometre a demarré
unsigned long   u32ChronoArrretDebut   =  millis();    //valeur du temps au demarrage du chronometre
 
unsigned int    u16NiveauBatterie     = 0;        // INFO BIONET niveau de la batterie
 
unsigned int    u16CourantMoteur     = 0;        // Valeur du courant en dizieme d'ampere
                                        
int             s16CourantMotUnitAmp = 0;        // INFO BIONETValeur du courant issu de la CAN code sur 10bit


int8_t          i8PWMTemp              = 0;        // la valeur PWM  temporaire utilisee pour rendre continue PWM
int8_t          i8PWM = 0;                         // CRITURE BIONET la valeur PWM  envoyée au moteur 

float commande;
float commande_old=0;
float commande_sat;
float commande_sat_old;
float parametre_reglable=10;//parametre pour ajuster l assistance, plus c'est elever, plus il y a d assistance
float kp=0.5387;
float Ti=0.0383;
float Ts=0.05;
float CurrentConsigned=0; //dixieme Ampere
float Current=0;
float Current_old=0;
int8_t PWM_PID=0;
 
//-----------------------------------------------------------------------------
// Chronometre pour imposer une consigne nulle si pas de pédalage pendant plus de 10 s
//-----------------------------------------------------------------------------

void  gestionChronometreArretPedalage(){
  if ((bChronoArretPedalage) && (millis()-u32ChronoArrretDebut>10000)){
     
        u8Etat=ETAT_ARRET;
        ConsigneDesiree=0;
        stopSpeed();
        bChronoArretPedalage=false;
        afficheValeurLed(255);
      }
}

//-----------------------------------------------------------------------------
// Mesure du niveau DE BATTERIE
//-----------------------------------------------------------------------------

void mesureBatterie() {
  int level = 0;
  if (u16NiveauBatterie < 32768) {
    level = map(u16NiveauBatterie,LOW_LEVEL,HIGH_LEVEL,1,NB_LED_MOUTH);
    level = constrain(level,1,NB_LED_MOUTH);
  }
  setMouth(level);
}


//-----------------------------------------------------------------------------
// Mesure DU COURANT
//-----------------------------------------------------------------------------

/* ici on calcule la valeur du courant par pas de 0.1 Ampères
 */

void CalculCourant() {
  //la valeur de courant lue diminue plus le courant augmente
  s16CourantMotUnitAmp = (10*(COURANT_REPOS-(int)u16CourantMoteur))/COURANT_RESOLUTION;
  // s16CourantMotUnitAmp est la valeur du courant en dizieme d'ampere
  if ( s16CourantMotUnitAmp < 0 ) s16CourantMotUnitAmp = 0;
}  





//-----------------------------------------------------------------------------
// Gestion de l'état du velo 
//-----------------------------------------------------------------------------

/* ici on gère l'état du vélo en fonction du pédalage, du courant et de la rotation 
 *  de la roue
 */

void gestionEtat(){
int s16courantLimit=0;

switch (speedLevel){
  case 0: s16courantLimit=0;break;
  case 1: s16courantLimit=COURANT_MOTEUR_LIMITE_1;break;
  //case 2: s16courantLimit=COURANT_MOTEUR_LIMITE_2;break;
  //case 3: s16courantLimit=COURANT_MOTEUR_LIMITE_3;break;
  
}

switch(u8Etat){
  
  case ETAT_ARRET:
   if (u8VitessePedale<PEDALE_LIMITE) 
       u8Etat=ETAT_ARRET;
   else if ((u8VitessePedale>=PEDALE_LIMITE)&&(ConsigneDesiree<1)) {
       u8Etat=ETAT_ARRET;
       
   }
   else if ((u8VitessePedale>=PEDALE_LIMITE)&&(ConsigneDesiree>=1)) {
    u8Etat=ETAT_DEMARRAGE;
    bChronoArretPedalage         =  false;
   }

   if (s16CourantMotUnitAmp>COURANT_MOTEUR_MAX) u8Etat=ETAT_ARRET;
  break;

  
 case ETAT_DEMARRAGE:
   if (u8VitessePedale<PEDALE_LIMITE) {
       u8Etat=ETAT_ARRET;
       bChronoArretPedalage         =  true;
       u32ChronoArrretDebut   =  millis(); 
   }
    else if ((u8VitessePedale>=PEDALE_LIMITE) && (s16CourantMotUnitAmp>=COURANT_DEMARRAGE_LIMITE) && (u8RotationRoue<VITESSE_DEMARRAGE_LIMITE))
       u8Etat=ETAT_ARRET; // cas du retropedalage 
   else if ((u8VitessePedale>=PEDALE_LIMITE) && (s16CourantMotUnitAmp<COURANT_DEMARRAGE_LIMITE) && (u8RotationRoue<VITESSE_DEMARRAGE_LIMITE))
      u8Etat=ETAT_DEMARRAGE;
   else if ((u8VitessePedale>=PEDALE_LIMITE) &&  (u8RotationRoue>=VITESSE_DEMARRAGE_LIMITE)) 
       u8Etat=ETAT_REGULATION  ;
   if (s16CourantMotUnitAmp>COURANT_MOTEUR_MAX) u8Etat=ETAT_BUG;
  break;
  
 case ETAT_REGULATION:
   if (u8VitessePedale<PEDALE_LIMITE) {
       u8Etat=ETAT_ARRET;
       bChronoArretPedalage         =  true;
       u32ChronoArrretDebut   =  millis();
       commande=0;
       commande_old=0;
       commande_sat=0;
       commande_sat_old=0;
       Current=0;
       Current_old=0;
   }
   else if ((u8VitessePedale>=PEDALE_LIMITE) && (s16CourantMotUnitAmp<=s16courantLimit) && (u8RotationRoue<ConsigneDesiree-VITESSE_REGULATION_SEUIL))
      u8Etat=ETAT_REGULATION ;

   else if ((u8VitessePedale>=PEDALE_LIMITE) && (s16CourantMotUnitAmp<=s16courantLimit) && (u8RotationRoue>=ConsigneDesiree-VITESSE_REGULATION_SEUIL)){
    
      u8Etat=ETAT_REGULATION ;
   }
   else if ((u8VitessePedale>=PEDALE_LIMITE) && (s16CourantMotUnitAmp>s16courantLimit)  ){
     
      u8Etat=ETAT_REGULATION ;       
   }
   else if (u8RotationRoue > 83){
    u8Etat=ETAT_ARRET;
   }
   if (s16CourantMotUnitAmp>COURANT_MOTEUR_MAX) u8Etat=ETAT_BUG;
  break;
  }
}


//-----------------------------------------------------------------------------
// Affichage sur les 10 voyants du boitier de commande
//-----------------------------------------------------------------------------

void afficheValeurLed(int valeur) {
     byte high_nombre[]   = {7,6,5,4,3,2};
     byte low_nombre[]      = {10,11,12,13};
     int i,temp,nombre[10];
     temp=valeur;
     for (i=0;i<10;i++){   // itère pour le nombre de digit attendus
           nombre[i]=temp & 1;  // prend le LSB et le sauve dans nombre
           temp = temp >> 1;    // décalage d'un bit sur la droite
     }
    for (i=0;i<4;i++) digitalWrite(low_nombre[i],nombre[i]==1?HIGH:LOW);
    for (i=0;i<6;i++) digitalWrite(high_nombre[i],nombre[i+4]==1?HIGH:LOW);     
}

void gestionPWM() {
byte gain=2;

switch(u8Etat){
  case ETAT_ARRET:
   i8PWM=0;
  break;
  
  case ETAT_DEMARRAGE:
   i8PWM=PWM_DEMARRAGE;
  break;


  //-----------------------------------------------------------------------------
  // REGULATION en VITESSE, on realise un correcteur proportionnel 
  // PWM=K(Consigne-vitesse rotation roue)
  // Experimentallement un gain supérieur à 2 conduit à un système oscillant (à vide)
  //-----------------------------------------------------------------------------

  case ETAT_REGULATION:
    CurrentConsigned=2*(((float)ConsigneDesiree)/100)*0.17*parametre_reglable*u8VitessePedale;
   
   Current=(CurrentConsigned-s16CourantMotUnitAmp);
   commande=commande_old + (kp*Current) + (kp*(1-(Ts/Ti))*Current_old) + ((Ts/Ti)*(commande_sat_old-commande_old));
   commande_old=commande;
   Current_old=Current;
     
   
   if(commande > 100)
   {
     commande_sat=100;
   }
   else if(commande < 0)
   {
     commande_sat=0;
   }
   else
   {
     commande_sat=commande;
   }
   
   commande_sat_old=commande_sat;
   PWM_PID=commande_sat;
  
   i8PWM=PWM_PID;
  break;
  
  case ETAT_BUG:
     i8PWM=0;
     u8Etat=ETAT_ARRET;
  break;
     
}


//afficheValeurLed(ConsigneDesiree);


}


