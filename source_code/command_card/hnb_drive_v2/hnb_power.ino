//****************************************************************************
// GESTION DU MOTEUR ET DE LA BATTERIE
//****************************************************************************
//

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
  battery_display(level);
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
  case 2: s16courantLimit=COURANT_MOTEUR_LIMITE_2;break;
  case 3: s16courantLimit=COURANT_MOTEUR_LIMITE_3;break;
  
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
       u8Etat=ETAT_AUGMENTE_PWM ;
   if (s16CourantMotUnitAmp>COURANT_MOTEUR_MAX) u8Etat=ETAT_BUG;
  break;
  
  case ETAT_AUGMENTE_PWM:
   if (u8VitessePedale<PEDALE_LIMITE) {
       u8Etat=ETAT_ARRET;
       bChronoArretPedalage         =  true;
       u32ChronoArrretDebut   =  millis(); 
   }
   else if ((u8VitessePedale>=PEDALE_LIMITE) && (s16CourantMotUnitAmp<s16courantLimit) && (u8RotationRoue<ConsigneDesiree-VITESSE_REGULATION_SEUIL))
      u8Etat=ETAT_AUGMENTE_PWM;
   else if ((u8VitessePedale>=PEDALE_LIMITE) && (s16CourantMotUnitAmp<s16courantLimit) && (u8RotationRoue>=ConsigneDesiree-VITESSE_REGULATION_SEUIL)){
      u8Etat=ETAT_REGULATION ;
      i8PWMTemp=i8PWM;
   }
   else if ((u8VitessePedale>=PEDALE_LIMITE) && (s16CourantMotUnitAmp>=s16courantLimit)  )
      u8Etat=ETAT_DIMINUE_PWM ;    
  
   if (s16CourantMotUnitAmp>COURANT_MOTEUR_MAX) u8Etat=ETAT_SUR_INTENSITE;
 
  break;

case ETAT_DIMINUE_PWM:
   if (u8VitessePedale<PEDALE_LIMITE) {
       u8Etat=ETAT_ARRET;
       bChronoArretPedalage         =  true;
       u32ChronoArrretDebut   =  millis(); 
   }
   else if ((u8VitessePedale>=PEDALE_LIMITE) && (s16CourantMotUnitAmp<s16courantLimit) && (u8RotationRoue<ConsigneDesiree-VITESSE_REGULATION_SEUIL))
      u8Etat=ETAT_AUGMENTE_PWM;
   else if ((u8VitessePedale>=PEDALE_LIMITE) && (s16CourantMotUnitAmp<s16courantLimit) && (u8RotationRoue>=ConsigneDesiree-VITESSE_REGULATION_SEUIL)){
      u8Etat=ETAT_REGULATION ;
      i8PWMTemp=i8PWM;
   }
   else if ((u8VitessePedale>=PEDALE_LIMITE) && (s16CourantMotUnitAmp>=s16courantLimit)  )
      u8Etat=ETAT_DIMINUE_PWM ;       
   if (s16CourantMotUnitAmp>COURANT_MOTEUR_MAX) u8Etat=ETAT_SUR_INTENSITE;
 
  break;

  
  case ETAT_REGULATION:
   if (u8VitessePedale<PEDALE_LIMITE) {
       u8Etat=ETAT_ARRET;
       bChronoArretPedalage         =  true;
       u32ChronoArrretDebut   =  millis(); 
   }
   else if ((u8VitessePedale>=PEDALE_LIMITE) && (s16CourantMotUnitAmp<=s16courantLimit) && (u8RotationRoue<ConsigneDesiree-VITESSE_REGULATION_SEUIL))
      u8Etat=ETAT_AUGMENTE_PWM;

   else if ((u8VitessePedale>=PEDALE_LIMITE) && (s16CourantMotUnitAmp<=s16courantLimit) && (u8RotationRoue>=ConsigneDesiree-VITESSE_REGULATION_SEUIL)){
    
      u8Etat=ETAT_REGULATION ;
   }
   else if ((u8VitessePedale>=PEDALE_LIMITE) && (s16CourantMotUnitAmp>s16courantLimit)  ){
     
      u8Etat=ETAT_DIMINUE_PWM ;       
   }
   if (s16CourantMotUnitAmp>COURANT_MOTEUR_MAX) u8Etat=ETAT_SUR_INTENSITE;
  break;
  
  case ETAT_SUR_INTENSITE:
   if (u8VitessePedale<PEDALE_LIMITE) {
       u8Etat=ETAT_ARRET;
       bChronoArretPedalage         =  true;
       u32ChronoArrretDebut   =  millis(); 
   }
   else if ((u8VitessePedale>=PEDALE_LIMITE) && (s16CourantMotUnitAmp<s16courantLimit) && (u8RotationRoue<ConsigneDesiree-VITESSE_REGULATION_SEUIL))
      u8Etat=ETAT_AUGMENTE_PWM;
   else if ((u8VitessePedale>=PEDALE_LIMITE) && (s16CourantMotUnitAmp<s16courantLimit) && (u8RotationRoue>=ConsigneDesiree-VITESSE_REGULATION_SEUIL)){
      u8Etat=ETAT_REGULATION ;
      i8PWMTemp=i8PWM;
   }
   else if ((u8VitessePedale>=PEDALE_LIMITE) && (s16CourantMotUnitAmp>=s16courantLimit)  )
      u8Etat=ETAT_DIMINUE_PWM ;       
   if (s16CourantMotUnitAmp>COURANT_MOTEUR_MAX) u8Etat=ETAT_SUR_INTENSITE;
 
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

  case ETAT_AUGMENTE_PWM:
   i8PWM=i8PWM+1;
  break;

  case ETAT_DIMINUE_PWM:
    i8PWM=i8PWM-1;
    if (i8PWM<0) i8PWM=0;
  break;

  // Diminution franche de la commande quand une surintensité moteur est détectée
  
   case ETAT_SUR_INTENSITE:
    i8PWM=i8PWM-5;
    if (i8PWM<0) i8PWM=0;
  break;
  
  //-----------------------------------------------------------------------------
  // REGULATION en VITESSE, on realise un correcteur proportionnel 
  // PWM=K(Consigne-vitesse rotation roue)
  // Experimentallement un gain supérieur à 2 conduit à un système oscillant (à vide)
  //-----------------------------------------------------------------------------

  case ETAT_REGULATION:
    if (abs(gain*( ConsigneDesiree- u8RotationRoue ))<=5) 
         i8PWM=i8PWMTemp+gain*( ConsigneDesiree-u8RotationRoue );
    else if (gain*( ConsigneDesiree- u8RotationRoue )>5) i8PWM=i8PWMTemp+5;
    else if (gain*( ConsigneDesiree- u8RotationRoue )<-5) i8PWM=i8PWMTemp-5;
  break;
  
  case ETAT_BUG:
     i8PWM=0;
     u8Etat=ETAT_ARRET;
  break;
     
}


//afficheValeurLed(ConsigneDesiree);


}
