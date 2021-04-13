
//-----------------------------------------------------------------------------
// Gestion de l'état du velo 
//-----------------------------------------------------------------------------

/* ici on gère l'état du vélo en fonction du pédalage, du courant et de la rotation 
 *  de la roue
 */

void gestionEtatEtPWM(){
int s16courantLimit=0;

if ((MODE==0)||(MODE==1)){
switch (speedLevel){
  case 0: s16courantLimit=COURANT_MOTEUR_LIMITE_0;u16Consigne=0;u8Etat=ETAT_ARRET;break;
  case 1: s16courantLimit=COURANT_MOTEUR_LIMITE_1;u16Consigne=VITESSE_1;break;
  case 2: s16courantLimit=COURANT_MOTEUR_LIMITE_2;u16Consigne=VITESSE_2;break;
  case 3: s16courantLimit=COURANT_MOTEUR_LIMITE_3;u16Consigne=VITESSE_3;break;
  case 4: s16courantLimit=COURANT_MOTEUR_LIMITE_4;u16Consigne=VITESSE_4;break;
}

// protection du moteur
if (i16Courant>COURANT_MOTEUR_MAX) u8Etat=ETAT_ETEINT_MOTEUR;

  
switch(u8Etat){
  //---------------------- Velo à l'arret 
  case ETAT_ARRET:
     if ((u16Consigne>0)&&(bPedalage)) {u8Etat=ETAT_DEMARRAGE;}
   break;
  //---------------------- 
  case ETAT_DEMARRAGE:
   if ((u16Vitesse>=VITESSE_DEMARRAGE_LIMITE)&&(bPedalage)) {
       u8Etat=ETAT_AUGMENTE_PWM ;
       // C'est la vitesse du vélo au moment de demarrage qui détermine la valeur du PWM à appliquer
       i16PWM=0.9*vitesse2PWM();
   }
   
   else if (!(bPedalage))
       u8Etat=ETAT_ARRET;    
   break;
  //---------------------- 
   case ETAT_AUGMENTE_PWM:
     if (u16Vitesse>=u16Consigne) 
      u8Etat=ETAT_DIMINUE_PWM ;
     else if (i16Courant>=s16courantLimit) 
      u8Etat=ETAT_DIMINUE_PWM ;
     else if (u16Vitesse==0)
      u8Etat=ETAT_ETEINT_MOTEUR;
     else if (!(bPedalage)) 
      u8Etat=ETAT_ROUE_LIBRE;
     else if ( u16Vitesse<0.8*PWM2Vitesse() ) // Pour limiter le calage  mais est ce que cela marche ? 
      u8Etat=ETAT_DIMINUE_PWM;
      
     // si le PWM est supérieur de 20% à ce qu'il faudrait et que le courant est faible
     // A tester
     //else if ( (i16PWM>1.2*vitesse2PWM() ) && (u16Courant<1000 ) ) {
     // u8Etat=ETAT_DECROCHAGE_MOTEUR;
     // tempsDecrochageMoteur=millis();
     //}  
   break;

  //---------------------- 
  case ETAT_DIMINUE_PWM:
  if ( (u16Vitesse<u16Consigne)&&(i16Courant<s16courantLimit) )
      u8Etat=ETAT_AUGMENTE_PWM;
  else if (u16Vitesse==0)
      u8Etat=ETAT_ETEINT_MOTEUR;
  else if (i16PWM==0)
      u8Etat=ETAT_ARRET;
  else if (!(bPedalage)) 
      u8Etat=ETAT_ROUE_LIBRE;
  break;
  //---------------------- 
  case ETAT_ROUE_LIBRE : 
  if (u16Vitesse<VITESSE_DEMARRAGE_LIMITE)
      u8Etat=ETAT_ARRET;
  else if (bPedalage){
      u8Etat=ETAT_AUGMENTE_PWM;
      //A la sortie de la roue libre, c'est la vitesse du velo qui permet de calculer le pwm à appliquer
      i16PWM=vitesse2PWM()*0.9;     
      }    
  break;
  //---------------------- 
  case ETAT_DECROCHAGE_MOTEUR:
    if ((millis()-tempsDecrochageMoteur)>500) {
      u8Etat=ETAT_AUGMENTE_PWM;
      //A la sortie de la roue libre, c'est la vitesse du velo qui permet de calculer le pwm à appliquer
      // En le divisant par 2 pour limiter la puissance 
      i16PWM=vitesse2PWM()*0.9;  
      }
  break;
  
  //---------------------- 
  case ETAT_ETEINT_MOTEUR:
  if (i16PWM==0) u8Etat=ETAT_ARRET;
  break;
  default:
  break;
  }
  


  
  int16_t i16DeltaPWM=0;
  // Calcul de la valeur du deltaPWM en fonction de la valeur du courant
  // pour limiter le calage du moteur
  if (i16Courant<0.2*s16courantLimit) i16DeltaPWM=4;   //< 50 W
  else if  ((i16Courant>=0.2*s16courantLimit)&&(i16Courant<0.4*s16courantLimit)) i16DeltaPWM=3; 
  else if  ((i16Courant>=0.4*s16courantLimit)&&(i16Courant<0.6*s16courantLimit)) i16DeltaPWM=2; 
  else if  ((i16Courant>=0.6*s16courantLimit)&&(i16Courant<0.8*s16courantLimit)) i16DeltaPWM=1; 
  else if  ((i16Courant>=0.8*s16courantLimit)&&(i16Courant<s16courantLimit)) i16DeltaPWM=0; 
  
  
  
  switch(u8Etat){
  case ETAT_ARRET:         i16PWM=0;                  break;
  case ETAT_DEMARRAGE:     i16PWM=PWM_DEMARRAGE;      break;
  case ETAT_AUGMENTE_PWM:  i16PWM=i16PWM+i16DeltaPWM; break;
  case ETAT_DIMINUE_PWM:   i16PWM=i16PWM-4;           break;
  case ETAT_ETEINT_MOTEUR: i16PWM=i16PWM-20;          break;
  case ETAT_ROUE_LIBRE:    i16PWM=0;                  break;
  case ETAT_ERREUR:        i16PWM=0;                  break;
  case ETAT_DECROCHAGE_MOTEUR:        i16PWM=0;       break;
  default:                                            break;
    }
  }

  if (i16PWM>2047)i16PWM=2047;
  if (i16PWM<0)i16PWM=0;
  dac12bits.setVoltage(2*i16PWM,true);
}
  
