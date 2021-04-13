// Definition 

#define ETAT_ARRET                0   // le velo est à l'arret
#define ETAT_DEMARRAGE            1   // le vele demarre 
#define ETAT_AUGMENTE_PWM         2   // on augmente le PWM 
#define ETAT_DIMINUE_PWM          3   // on diminie le PWM
#define ETAT_REGULATION           4   // n'est plus utilisé
#define ETAT_ERREUR               5   // une erreur es detecté (plus utilisé)
#define ETAT_ETEINT_MOTEUR        6   // on eteint le moteur en diminuant progressivement le PWM
#define ETAT_ROUE_LIBRE           7   // on a arreter de pédaler mais la vitesse du velo n'est pas nulle.
#define ETAT_DECROCHAGE_MOTEUR         8   // la carte de commmande a decroche ( passage du PWM à 0)

#define AUCUNE_ERREUR             0
#define ERREUR_BATTERIE           1
#define ERREUR_COURANT            2
#define ERREUR_PEDALIER           3
#define ERREUR_VITESSE            4

#define PWM_DEMARRAGE             70

/* Les valeurs limites qui gère la machine d'etat */

#define COURANT_MOTEUR_MAX        9000  // valeur max du courant que peux fournir la batterie

#define COURANT_MOTEUR_LIMITE_0   0  // valeur limite du courant en fonctionnement en vitesse 0
#define COURANT_MOTEUR_LIMITE_1   4000  // valeur limite du courant en fonctionnement en vitesse 1
#define COURANT_MOTEUR_LIMITE_2   5000  //150W// valeur limite du courant en fonctionnement en vitesse 2
#define COURANT_MOTEUR_LIMITE_3   6000  //200W// valeur limite du courant en fonctionnement en vitesse 3
#define COURANT_MOTEUR_LIMITE_4   7000  //250W// valeur limite du courant en fonctionnement en vitesse 4 Attention risque de décrochage

#define VITESSE_1   15000  // valeur de la vitesse 1
#define VITESSE_2   20000  // valeur de la vitesse 2
#define VITESSE_3   25000  // valeur de la vitesse 3
#define VITESSE_4   30000  // valeur de la vitesse 4


#define COURANT_DEMARRAGE_LIMITE  2000  // valeur limite du courant au demmarrage 
#define VITESSE_DEMARRAGE_LIMITE  7000  // vitesse de la roue à partir de laquelle le velo accelere ??
#define VITESSE_REGULATION_SEUIL  3000   // ecart de vitesse par rapport à la consigne qui provoque la regulation de la vitesse

// dans un moteur la vitesse est proportionnelle à la tension appliquée
// a une vitesse du velo doit correspondre une valeur du PWM
// On fait l'hypothèse ici que v=25 km/h pour i16PWM=2047

uint16_t PWM2Vitesse(){
  int16_t calcul;
  if (i16PWM<=700) calcul=24.44*i16PWM-265;
  else if (i16PWM>700) calcul = 15.18*i16PWM+265;
  if (calcul<0) calcul=0;
return(calcul);
}

uint16_t vitesse2PWM(){
  uint16_t calcul;
  if (u16Vitesse<=16843) calcul=u16Vitesse/24.4+10.8;
  else if (u16Vitesse>16843)calcul=u16Vitesse/15.2-452;
  return(calcul);
}
