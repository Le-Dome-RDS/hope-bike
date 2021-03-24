   /* 
    hnb_drive.ino : master drive
    Copyright (C) 2015-2016  François TROQUE, Arnaud BOULAY

    hnb_drive_beta.ino : beta testing version by Sylvain GARNAVAULT
    Version 0.99  BY JMR

    version for hardware v2 by Sylvain Legargy & Jean-Marc Routoure. GREYC.
    Universite de Caen-Normandie/CNRS. 2021
    

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

/* Version v2 0.99 */
// Machine d'état pour controller le système
// controle de la  vitesse et du courant avec 3 niveaux de vitesse et de couple
// regulation de la vitesse autour d'une consigne ( la vitesse souhaitée) avec un correcteur proportionnel
// Remise à zero de la consigne si l'utilisateur arrete de pédaler pendant plus de 10 s
// gestion du retro-pédalage
// 

// LE 22/09/2016
// correction d'un bug sur la commande proportionnelle !
// NB : 

// 15/03/2017 : gestion des sur-intensite par création d'un nouvel état avec diminution rapide du PWM
// 24/02/2020 : utilisation avec la nouvelle carte développée au GREYC avec base de Heltech32
// 03/03/2021 : TBD : filtre médian sur la mesure de vitesse et de pédalier
//              ajout d'un état pour repasser à 0 tranquillement
// 10/03/2021 : détection de la roue libre
// 22/03/2021 : modulation du deltaPWM pour limiter l'augmentation du WM pour les courants élevés


#include <Adafruit_ADS1015.h>   //Convertisseur analogique-numérique 12 bits
#include <Adafruit_MCP4725.h>   //convertisseur numérique-analogique 12 bits
#include "Arduino.h"
#include "heltec.h"    //carte d'interface avec écran OLED intégrébyte             u8BoutonGauche
#include "MCP7940.h"   //horloge temps réél intégré pour la durée d'utilisation

//****************************************************************************
// SETUP PROGRAM
//****************************************************************************

void setup() {  
  setup_i2c(); 
  setup_display();     
  setup_keyboard();
}

//****************************************************************************
// Variable Globale 
//****************************************************************************
const int PIN_LED =  25; //25
const int VERSION_HARD =1;   //VERSION_HARD=2 pour la version du 24/02/2020
#define ETAT_VITESSE_DEBUT        0
#define ETAT_VITESSE_1MES_OK      1
#define ETAT_VITESSE_2MES_OK      2
#define ETAT_VITESSE_3MES_OK      3
#define ETAT_VITESSE_ERREUR       4
 // Mesures
 uint16_t         u16adcRoueAvant=0,u16adcRoueMaintenant=0; //valeur lue sur l'ADC
 
 uint16_t         u16Batterie=0;                                //Niveau de la batterie en V;
 uint16_t         u16Courant=0;                                 //Valeur du courant débité en mA;
 uint16_t         u16CourantMoteurInitial     = 0;            // Valeur du courant au démarrage du vélo
 uint16_t         u16Pedalier=0;                                //Valeur lue sur le pedalier
 uint16_t         u16Vitesse     = 0;                       // vitesse de rotation de la roue moyenne
 uint16_t         u16TableauVitesse[5];                      // tableau pour calculer la valeur médiane de la vitesse
 uint16_t         u16TableauPedalier[5];                      // tableau pour calculer la valeur médiane de la vitesse
 uint16_t         u16TableauCourant[5];                      // tableau pour calculer la valeur médiane de la vitesse
 
 
 boolean          bPedalage=false;                             // Est égale à true si un pédalage est détecté
 //Gestion des erreurs
 byte             u8Etat= 0;                                  // Etat du velo
 byte             u8Erreur=0;
 
 //Consigne et tension moteur
 int16_t          i16PWM            = 0;                                 
 uint16_t         u16Consigne       = 0;         // consigne choisie
 byte             speedLevel        = 0;         // niveau de vitesse

 // Divers
 long             tempsFront=millis(); ///  Durée entre le front sur le capteur de vitesse de roue
 long             tempsMesureBatterie=millis(); // Toutes les secondes, on va lire la tension batterie
 long             tempsAfficheEcran=millis(); // Toutes les 20 ms, on met à jour l'écran
 long             tempsRAZEcran=millis();     // Toutes les 10s, on reinitialise l'écran
 long             tempsDecrochageMoteur=millis(); // En cas de décrochage moteur, on attend 500m pour redemarrer;
 long             k=0;
 bool             bTestLed;
 // Mode de fonctionnement 0=Normal, 1=Debug, 2=test_devices, 3 test 
 // 1 : debug infos will be printed on the display in a normal behavior
 // 3 : controle du pwm par les touches
 int              MODE=0;             
 int              SERIAL_DEBUG=0;                   // Envoi d'info sur le port série si SERIAL_DEBUG=1;
 //int              PEDAL=0;   // bPedalage=true si PEDAL=1 ;  



 //------------------------ Estimation de la vitesse de la roue

uint16_t estim_vitesse_velo(){
  long mesureVitesse=u16Vitesse;
  byte front=0;

  // Detection du front
  if (k==0) {
    u16adcRoueAvant=litVitesseRoue();
    mesureVitesse=0;
  }
  else {
  u16adcRoueMaintenant=litVitesseRoue();
  if ((u16adcRoueAvant<100) && (u16adcRoueMaintenant>500) ) front=1; 
  u16adcRoueAvant=u16adcRoueMaintenant;
  }
  
  if (front==1) { // On affiche le résultat et on fait clignoter la LED.
    long duree=millis()-tempsFront; // durée entre 2 front montant
    tempsFront=millis();
    bTestLed = !bTestLed;   
    digitalWrite(PIN_LED, bTestLed);
    mesureVitesse=7465000/duree;
    front=0;
  }
  // Si pas de front pendant 3s, on fait passer la vitesse à 0;
  if ((millis()-tempsFront)>3000) mesureVitesse=0;
  
  return mesureVitesse; 
}  
  

void loop() {
// Execution toutes les 5 ms

if ((MODE==1)||(MODE==0)) {
  //**************************************************************************
  // FONCTIONNEMENT NORMAL
  //**************************************************************************
  // Toutes les 5 ms, on va lire la vitesse de la roue
  // l'estimation de la vitesse est le résultat d'un filrage média sur 5 valeurs
  
  ajoute_un_element_et_decale_vitesse_velo(estim_vitesse_velo());
  u16Vitesse=median_vitesse_velo();
  
  switch(k%6){
    case 0: //On mesure la batterie toutes les secondes
       if ((millis()-tempsMesureBatterie)>1000) {
         u16Batterie=litBatterie();
         tempsMesureBatterie=millis();
       }
       
    break;
    case 1: // on mesure le courant;
       ajoute_un_element_et_decale_courant(litCourant());
       u16Courant=median_courant();;
       
       break;
    case 2: // on mesure le pédalier;
       bPedalage=false;
        ajoute_un_element_et_decale_pedalier(litPedalier());
        u16Pedalier=median_pedalier();
              
       if (u16Pedalier>100 && u16Pedalier<600) bPedalage=true;   
       //if (PEDAL==1) bPedalage=true;
       break;
    case 3:// on gere le clavier et la machine d'état;
       gestionEtat();
       gestionPWM();
       gestionClavier();
       break;   
    case 4: // ,On affiche les résultats toutes les 20 ms
      if ((millis()-tempsAfficheEcran)>20) {
       tempsAfficheEcran=millis(); 
       Heltec.display->clear();    //clear the display
       afficheVitesseVelo();
       afficheBatterie();
       afficheConsigne();
       afficheCourant();
       affichePedalier();
       afficheEtat();
       affichePWM();
       Heltec.display->display();  
      }
      break;
    case 6: //On réinitialise l'écran toutes les 10s
      if ((millis()-tempsRAZEcran)>10000) {
       tempsRAZEcran=millis(); 
       setup_display();
      }
    default:break;
    }
  k++;
  
  if ((SERIAL_DEBUG==1))serialDebug();
  }
  else if (MODE==2) {
    //test_i2c();
    //test_display();
    //test_keyboard();
    //test_devices();
    //  
  }
  // On controle le pwm avec les touches haut et bas
  else if (MODE==3) {
      u16Batterie=litBatterie();
      u16Courant=litCourant();
      
      gestionClavier();
      gestionPWM();
      Heltec.display->clear();    //clear the display
      afficheVitesseVelo();
      afficheBatterie();
      afficheCourant();
      affichePedalier();
      affichePWM();
      
      Heltec.display->display();  
  }
  

  
}
