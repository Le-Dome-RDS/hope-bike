


// Definition 

#define NB_LED_MOUTH              4    // Nombre de led disponible pour l'affichage du niveau de batterie

#define COURANT_REPOS             602  // JMR : ses valeurs sont déduites de mesure effectuées sur le vélo 1
// TBD : Il faudrait mesurer le courant lorsque le moteur est à l'arret...
#define COURANT_RESOLUTION        18  //      


#define ETAT_ARRET                0
#define ETAT_DEMARRAGE            1
#define ETAT_AUGMENTE_PWM         2
#define ETAT_DIMINUE_PWM          3
#define ETAT_REGULATION           4
#define ETAT_SUR_INTENSITE        5

#define ETAT_BUG                  6


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


// définition des id des boutons
#define btnNONE   0               // aucun bouton n'est appuyé
#define btnUP     1               // bouton pour augmenter l'assistance
#define btnDOWN   2               // bouton pour réduire l'assistance
#define btnSTOP   3               // bouton pour stopper l'assistance

// paramètres pour la lecture des boutons
#define DEBOUNCE_DELAY  150       // temporisation pour les boutons 
#define KEYBOARD_ANALOG_INPUT 0   // numéro de port analogique pour les boutons

long lastDebounceTime;            // mesure de temporisation pour les boutons
byte lastButtonPressed;           // mémorisation de la valeur des boutons pour éviter les répétitions

const byte MAX_SPEED_GEAR = (sizeof(GEARS)/sizeof(int));



byte            u8Etat              = ETAT_ARRET; // Etat du velo
byte            u8VitessePedale    = 0;          // INFO BIONET vitesse de pédalage 
byte            u8RotationRoue     = 0;          // INFO BIONET vitesse de rotation de la roue     

// chronometre pour mettre à zero la consigne si l'utilisateur ne pedale pas pendant plus de 5 secondes
boolean         bChronoArretPedalage         =  false;      // est ce que le chronometre a demarré
unsigned long   u32ChronoArrretDebut   =  millis();    //valeur du temps au demarrage du chronometre
 
unsigned int    u16NiveauBatterie     = 0;        // INFO  niveau de la batterie
 
unsigned int    u16CourantMoteur     = 0;        // Valeur du courant en dizieme d'ampere
                                        
int             s16CourantMotUnitAmp = 0;        // INFO BIONETValeur du courant issu de la CAN code sur 10bit


int8_t          i8PWMTemp              = 0;        // la valeur PWM  temporaire utilisee pour rendre continue PWM
int8_t          i8PWM = 0;                         // CRITURE BIONET la valeur PWM  envoyée au moteur 
