// Definition 

#define ETAT_ARRET                0
#define ETAT_DEMARRAGE            1
#define ETAT_AUGMENTE_PWM         2
#define ETAT_DIMINUE_PWM          3
#define ETAT_REGULATION           4
#define ETAT_ERREUR_CURRENT               5


#define PWM_DEMARRAGE             40

/* Les valeurs limites qui gère la machine d'etat */

#define COURANT_MOTEUR_MAX        13000  // valeur max du courant que peux fournir la batterie

#define COURANT_MOTEUR_LIMITE_0   0  // valeur limite du courant en fonctionnement en vitesse 0
#define COURANT_MOTEUR_LIMITE_1   6000  // valeur limite du courant en fonctionnement en vitesse 1
#define COURANT_MOTEUR_LIMITE_2   8000  // valeur limite du courant en fonctionnement en vitesse 2
#define COURANT_MOTEUR_LIMITE_3   10000  // valeur limite du courant en fonctionnement en vitesse 3

#define VITESSE_1   10000  // valeur de la vitesse 1
#define VITESSE_2   20000  // valeur de la vitesse 2
#define VITESSE_3   25000  // valeur de la vitesse 3

#define COURANT_DEMARRAGE_LIMITE  2000  // valeur limite du courant au demmarrage 
#define VITESSE_DEMARRAGE_LIMITE  5000  // vitesse de la roue à partir de laquelle le velo accelere ??
#define VITESSE_REGULATION_SEUIL  2000   // ecart de vitesse par rapport à la consigne qui provoque la regulation de la vitesse

//-----------------------------------------------------------------------------
// Variable globale
//-----------------------------------------------------------------------------



// définition des PORTS utilisés pour les  boutons
#define BUTTON_LEFT    13               
#define BUTTON_RIGHT   12        
#define BUTTON_OK      14         

// paramètres pour la lecture des boutons
#define DEBOUNCE_DELAY  150       // temporisation pour les boutons 
#define KEYBOARD_ANALOG_INPUT 0   // numéro de port analogique pour les boutons

long lastDebounceTime;            // mesure de temporisation pour les boutons
byte lastButtonPressed;           // mémorisation de la valeur des boutons pour éviter les répétitions
