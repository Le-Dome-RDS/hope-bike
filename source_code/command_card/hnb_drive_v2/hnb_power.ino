
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
  case 4: s16courantLimit=COURANT_MOTEUR_LIMITE_4;u16Consigne=VITESSE_4;break;
}

// protection du moteur
if (u16Courant>COURANT_MOTEUR_MAX) u8Etat=ETAT_ETEINT_MOTEUR;

  
switch(u8Etat){
  //---------------------- Velo à l'arret 
  case ETAT_ARRET:
     if ((u16Consigne>0)&&(bPedalage)) {u8Etat=ETAT_DEMARRAGE;}
   break;
  //---------------------- 
  case ETAT_DEMARRAGE:
   if ( (u16Courant>=COURANT_DEMARRAGE_LIMITE) && (u16Vitesse<VITESSE_DEMARRAGE_LIMITE))
       u8Etat=ETAT_ARRET; // cas du retropedalage 
   else if ((u16Vitesse>=VITESSE_DEMARRAGE_LIMITE)&&(bPedalage)) 
       u8Etat=ETAT_AUGMENTE_PWM ;
   else if (!(bPedalage))
       u8Etat=ETAT_ARRET;    
  break;
  //---------------------- 
   case ETAT_AUGMENTE_PWM:
     if ( (u16Courant<s16courantLimit) && (u16Vitesse>=u16Consigne)) 
      u8Etat=ETAT_DIMINUE_PWM ;
     else if (u16Courant>=s16courantLimit) 
      u8Etat=ETAT_DIMINUE_PWM ;
     else if (u16Vitesse==0)
      u8Etat=ETAT_ETEINT_MOTEUR;
     else if (!(bPedalage)) {
      u8Etat=ETAT_ROUE_LIBRE;
      i16PWMTemp=i16PWM;
     }
     else if ((u16Vitesse<12000)&&(i16PWM>550)){
      u8Etat=ETAT_DECROCHAGE_MOTEUR;
      tempsDecrochageMoteur=millis();
     }  
   break;

  //---------------------- 
  case ETAT_DIMINUE_PWM:
  if ( (u16Vitesse<u16Consigne)&&(u16Courant<s16courantLimit) )
      u8Etat=ETAT_AUGMENTE_PWM;
  else if (u16Vitesse==0)
      u8Etat=ETAT_ETEINT_MOTEUR;
  else if (i16PWM==0)
      u8Etat=ETAT_ARRET;
  else if (!(bPedalage)) {
      u8Etat=ETAT_ROUE_LIBRE;
      i16PWMTemp=i16PWM;     
      }
  break;
  //---------------------- 
  case ETAT_ROUE_LIBRE : 
  if (u16Vitesse<VITESSE_DEMARRAGE_LIMITE)
      u8Etat=ETAT_ARRET;
  else if (bPedalage){
      u8Etat=ETAT_AUGMENTE_PWM;
      i16PWM=2*i16PWMTemp/3;
      }    
  break;

  case ETAT_DECROCHAGE_MOTEUR:
    if ((millis()-tempsDecrochageMoteur)>500) u8Etat=ETAT_ARRET;
  break;
  
  //---------------------- 
  case ETAT_ETEINT_MOTEUR:
  if (i16PWM==0) u8Etat=ETAT_ARRET;
  break;
  default:
  break;
  }
  
}


void gestionPWM() {
if ((MODE==0)||(MODE==1)){
  
  int16_t i16DeltaPWM=0;
  // Calcul de la valeur du deltaPWM en fonction de la valeur du courant
  // pour limiter le calage du moteur
  if (u16Courant<1400) i16DeltaPWM=5;   //< 50 W
  else if  ((u16Courant>=1400)&&(u16Courant<2800)) i16DeltaPWM=4; // < 100 W
  else if  ((u16Courant>=2800)&&(u16Courant<4200)) i16DeltaPWM=3; // < 150 W
  else if  ((u16Courant>=4200)&&(u16Courant<5600)) i16DeltaPWM=2; // < 200 W
  else if  ((u16Courant>=5600)&&(u16Courant<7000)) i16DeltaPWM=1; // < 250 W
  else if  (u16Courant>=7000) i16DeltaPWM=0; // > 250 W
  
  
  
  switch(u8Etat){
  case ETAT_ARRET:         i16PWM=0;              break;
  case ETAT_DEMARRAGE:     i16PWM=PWM_DEMARRAGE;  break;
  case ETAT_AUGMENTE_PWM:  i16PWM=i16PWM+i16DeltaPWM;  break;
  case ETAT_DIMINUE_PWM:   i16PWM=i16PWM-4;       break;
  case ETAT_ETEINT_MOTEUR: i16PWM=i16PWM-10;      break;
  case ETAT_ROUE_LIBRE:    i16PWM=0;              break;
  case ETAT_ERREUR:        i16PWM=0;              break;
  case ETAT_DECROCHAGE_MOTEUR:        i16PWM=0;              break;
  default:                                        break;
  }
  if (i16PWM>1023)i16PWM=1023;
  if (i16PWM<0)i16PWM=0;
  dac12bits.setVoltage(4*i16PWM,true);
} 
else if (MODE==3){
  if (i16PWM>1023)i16PWM=1023;
  if (i16PWM<0)i16PWM=0;
  dac12bits.setVoltage(4*i16PWM,true);
  }
}
  
