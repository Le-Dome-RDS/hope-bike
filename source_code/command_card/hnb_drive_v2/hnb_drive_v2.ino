   /* 
    hnb_drive.ino : master drive
    Copyright (C) 2015-2016  François TROQUE, Arnaud BOULAY

    hnb_drive_beta.ino : beta testing version by Sylvain GARNAVAULT
    Version 0.99  BY JMR

    version for hardware v2 by Sylvain Legargy & Jean-Marc Routoure. GREYC.
    Universite de Caen-Normandie/CNRS.
    

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
// 24/02/2020 : utilisation azvec lma nouvelle carte développée au GREYC avec base de Heltech32

#include <Adafruit_ADS1015.h>   //Convertisseur analogique-numérique 12 bits
#include <Adafruit_MCP4725.h>   //convertisseur numérique-analogique 12 bits
#include "Arduino.h"
#include "heltec.h"    //carte d'interface avec écran OLED intégré
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
const int VERSION_HARD =2;   //VERSION_HARD=2 pour la version du 24/02/2020

 // Mesures
 uint16_t         u16adcRoueAvantAvant=0,u16adcRoueAvant=0,u16adcRoueMaintenant=0; //valeur lue sur l'ADC
 uint16_t         u16Batterie=0;                                //Niveau de la batterie en V;
 uint16_t         u16Courant=0;                                 //Valeur du courant débité en mA;
 uint16_t         u16CourantMoteurInitial     = 0;            // Valeur du courant au démarrage du vélo
 uint16_t         u16Pedalier=0;                                //Valeur lue sur le pedalier
 uint16_t         u16Vitesse     = 0;                       // vitesse de rotation de la roue déduite      
 boolean          bPedalage=false;                             // Est égale à true si un pédalage est détecté
 //Gestion des erreurs
 byte             u8Etat= 0;                                  // Etat du velo
 byte             u8Erreur=0;
 byte             u8CompteurErreurBatterie=0;                         //le nombre de fois ou une lecture incorrecte s'est produite sur la batterie.
 byte             u8CompteurErreurVitesse=0;                         //le nombre de fois ou une lecture incorrecte s'est produite sur la batterie.
 byte             u8CompteurErreurCourant=0;                         //le nombre de fois ou une lecture incorrecte s'est produite sur la batterie.
 byte             u8CompteurErreurPedalier=0;                         //le nombre de fois ou une lecture incorrecte s'est produite sur la batterie.
 //Consigne et tension moteur
 int16_t          i16PWMTemp              = 0;                 // la valeur PWM  temporaire utilisee pour rendre continue PWM
 int16_t          i16PWM = 0;                                  // CRITURE BIONET la valeur PWM  envoyée au moteur 
 uint16_t         u16Consigne = 0;         // consigne choisie
 byte             speedLevel = 0;              // niveau de vitesse

 // Divers
 long             tempsFront=millis();;
 long             k=0;
 bool             bTestLed;
 int              MODE=0;             // Mode de fonctionnement 0=Normal, 1=Debug, 2=test_devices
 // 1 : debug infos will be printed on the display in a normal behavior
 // 2 : keyboard, display and I2C devices will be tested
int              SERIAL_DEBUG=1;                   // Envoi d'info sur le port série
int               DEBUG=0 ;   // bPedalage=true; speedLevel=2;  


void loop() {
if (DEBUG==1)speedLevel=3;
byte front=0;

if ((MODE==1)||(MODE==0)) {
  //**************************************************************************
  // FONCTIONNEMENT NORMAL
  //**************************************************************************
  if (k==0) {
    u16adcRoueAvantAvant=litVitesseRoue();
    u16adcRoueAvant=litVitesseRoue();
  }
  //------------------------ calcul de la vitesse de la roue
  // recherche d'un front sur la roue
  // 2 lectures pour être sur qua la seconde est correcte
  //u16adcRoueMaintenant = litVitesseRoue();    //2ms
  u16adcRoueMaintenant = litVitesseRoue();    //2ms
  u8CompteurErreurVitesse=0;
  while ((u16adcRoueMaintenant==4095)&&u8CompteurErreurVitesse<=5) {u16adcRoueMaintenant = litVitesseRoue();u8CompteurErreurVitesse++;}    //2ms
  
  if (u8CompteurErreurVitesse>5) u8Erreur=4; //EREUR_VITESSE
  u16adcRoueMaintenant = litVitesseRoue();    //2ms
  if ((u16adcRoueAvantAvant<100) && (u16adcRoueAvant>500) && (u16adcRoueMaintenant>500) ) front=1; 
  //if ((u16adcRoueAvant>500) && (u16adcRoueMaintenant>500) ) front=1; 
  u16adcRoueAvantAvant=u16adcRoueAvant;
  u16adcRoueAvant=u16adcRoueMaintenant;
  
  
  if (front==1) {
    long duree=millis()-tempsFront; // durée entre 2 front montant
    tempsFront=millis();
    bTestLed = !bTestLed;   
    digitalWrite(PIN_LED, bTestLed);
    long calcul=7465000/duree;
    //if (abs(u16Vitesse-calcul)<10000) u16Vitesse=calcul;
    u16Vitesse=calcul;
    front=0;
  }
 
  if ((millis()-tempsFront)>3000) u16Vitesse=0;
  delay(5);
  switch(k%5){
    case 0: //On mesure la batterie
       u16Batterie=litBatterie();
       u16Batterie=litBatterie();
    break;
    case 1: // on mesure le courant;
       u16Courant=litCourant();
       u16Courant=litCourant();
       break;
    case 2: // on mesure le pédalier;
       bPedalage=false;
       u16Pedalier=litPedalier();
       u16Pedalier=litPedalier();       
       if (u16Pedalier>300 && u16Pedalier<1500) bPedalage=true;   
       if (DEBUG==1) bPedalage=true;
       break;
    case 3:// on gere le clavier et la machine d'état;
       gestionEtat();
       gestionPWM();
       gestionClavier();
       break;   
    case 4: // ,On affiche les résultats
      Heltec.display->clear();    //clear the display
      afficheVitesseVelo();
      afficheBatterie();
      afficheConsigne();
      afficheCourant();
      affichePedalier();
      afficheEtat();
      afficheCommande();
      afficheMode();
      Heltec.display->display();  
      break;
    default:break;
    }
  k++;
  delay(2);
  if ((SERIAL_DEBUG==1))serialDebug();
  }
  else if (MODE==2) {
    //test_i2c();
    //test_display();
    //test_keyboard();
    //test_devices();
    //  
  }
  

  
}
