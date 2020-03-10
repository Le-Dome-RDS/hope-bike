//-----------------------------------------------------------------------------
// Gestion de l'état du velo 
//-----------------------------------------------------------------------------

/* ici on gère l'état du vélo en fonction du pédalage, du courant et de la rotation 
 *  de la roue
 */

void gestionEtat(){
int s16courantLimit=0;

switch (speedLevel){
  case 0: s16courantLimit=COURANT_MOTEUR_LIMITE_0;u16Consigne=0;u8Etat=ETAT_ARRET;break;
  case 1: s16courantLimit=COURANT_MOTEUR_LIMITE_1;u16Consigne=VITESSE_1;break;
  case 2: s16courantLimit=COURANT_MOTEUR_LIMITE_2;u16Consigne=VITESSE_2;break;
  case 3: s16courantLimit=COURANT_MOTEUR_LIMITE_3;u16Consigne=VITESSE_3;break;
}

// if (u16Courant>COURANT_MOTEUR_MAX) u8Etat=ETAT_ARRET;
if (u8Erreur>0) u8Etat=ETAT_ERREUR;

if (bPedalage) {
  
switch(u8Etat){
  //---------------------- Velo à l'arret 
  case ETAT_ARRET:
   
   if (u16Consigne>0) {u8Etat=ETAT_DEMARRAGE;}
   break;
  //---------------------- 
  case ETAT_DEMARRAGE:
   if ( (u16Courant>=COURANT_DEMARRAGE_LIMITE) && (u16Vitesse<VITESSE_DEMARRAGE_LIMITE))
       u8Etat=ETAT_ARRET; // cas du retropedalage 
   else if (u16Vitesse>=VITESSE_DEMARRAGE_LIMITE) 
       u8Etat=ETAT_AUGMENTE_PWM ;
  break;
  //---------------------- 
   case ETAT_AUGMENTE_PWM:
   if ( (u16Courant<s16courantLimit) && (u16Vitesse>=u16Consigne-VITESSE_REGULATION_SEUIL)){
      u8Etat=ETAT_REGULATION ;
      i16PWMTemp=i16PWM;
   }
   else if (u16Courant>=s16courantLimit) 
      u8Etat=ETAT_DIMINUE_PWM ;
   if (u16Vitesse<VITESSE_DEMARRAGE_LIMITE/2)
      u8Etat=ETAT_DEMARRAGE;
  break;

  //---------------------- 
  case ETAT_DIMINUE_PWM:
  if ( (u16Vitesse<u16Consigne-VITESSE_REGULATION_SEUIL)&&(u16Courant<s16courantLimit) )
      u8Etat=ETAT_AUGMENTE_PWM;
   else if (u16Vitesse>=u16Consigne-VITESSE_REGULATION_SEUIL){
      u8Etat=ETAT_REGULATION ;
      i16PWMTemp=i16PWM;
   } 
  break;
  //---------------------- 
    case ETAT_REGULATION:
   if (  (u16Courant<=s16courantLimit) && (u16Vitesse<u16Consigne-VITESSE_REGULATION_SEUIL))
    u8Etat=ETAT_AUGMENTE_PWM;
   else if ( (u16Courant>s16courantLimit)  )   
      u8Etat=ETAT_DIMINUE_PWM ;
   else if (u16Vitesse>u16Consigne-VITESSE_REGULATION_SEUIL)
      u8Etat=ETAT_DIMINUE_PWM ;
   break;
  //---------------------- 
 

  }
}  
else 
  u8Etat=ETAT_ARRET;


}


void gestionPWM() {

switch(u8Etat){
  case ETAT_ARRET: i16PWM=0; break;
  case ETAT_DEMARRAGE: i16PWM=PWM_DEMARRAGE; break;
  case ETAT_AUGMENTE_PWM:   i16PWM=i16PWM+4;  break;
  case ETAT_DIMINUE_PWM:    i16PWM=i16PWM-4;  break;
  //-----------------------------------------------------------------------------
  // REGULATION en VITESSE, on realise un correcteur proportionnel 
  // PWM=K(Consigne-vitesse rotation roue)
  //-----------------------------------------------------------------------------
  case ETAT_REGULATION:  i16PWM=i16PWMTemp+( u16Consigne-u16Vitesse )/500; break;
  case ETAT_ERREUR:i16PWM=0;
  default:    i16PWM=0;  break;
  }
  
  if (i16PWM>1023)i16PWM=1023;
  if (i16PWM<0)i16PWM=0;
  dac12bits.setVoltage(4*i16PWM,true);

  
}
