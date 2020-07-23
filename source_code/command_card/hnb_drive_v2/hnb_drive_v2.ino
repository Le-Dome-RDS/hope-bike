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
 //uint16_t         u16adcRoue[500];
 uint16_t         u16Batterie=0;                                //Niveau de la batterie en V;
 uint16_t         u16Courant=0;                                 //Valeur du courant débité en mA;
 uint16_t         u16CourantMoteurInitial     = 0;            // Valeur du courant au démarrage du vélo
 uint16_t         u16Pedalier=0;                                //Valeur lue sur le pedalier
 uint16_t         u16Vitesse     = 0;                       // vitesse de rotation de la roue moyenne
 uint16_t         u16Vitesse1  = 0;                       // vitesse de rotation de la roue mesure 1
 uint16_t         u16Vitesse2  = 0;                       // vitesse de rotation de la roue mesure 2
 uint16_t         u16Vitesse3  = 0;                       // vitesse de rotation de la roue mesure 3
  
 boolean          bPedalage=false;                             // Est égale à true si un pédalage est détecté
 //Gestion des erreurs
 byte             u8Etat= 0;                                  // Etat du velo
 byte             u8EtatVitesse =0 ;                          // Etat de la vitesse de la roue
 byte             u8Erreur=0;
 //byte             u8CompteurErreurBatterie=0;                         //le nombre de fois ou une lecture incorrecte s'est produite sur la batterie.
 //byte             u8CompteurErreurVitesse=0;                         //le nombre de fois ou une lecture incorrecte s'est produite sur la batterie.
 //byte             u8CompteurErreurCourant=0;                         //le nombre de fois ou une lecture incorrecte s'est produite sur la batterie.
 //byte             u8CompteurErreurPedalier=0;                         //le nombre de fois ou une lecture incorrecte s'est produite sur la batterie.
 //Consigne et tension moteur
// int16_t          i16PWMTemp              = 0;                 // la valeur PWM  temporaire utilisee pour rendre continue PWM
 int16_t          i16PWM = 0;                                  // CRITURE BIONET la valeur PWM  envoyée au moteur 
 uint16_t         u16Consigne = 0;         // consigne choisie
 byte             speedLevel = 0;              // niveau de vitesse

 // Divers
 long             tempsFront=millis(); ///  Durée entre !é front sur le capteur de vitesse de roue
 long             k=0;
 bool             bTestLed;
 int              MODE=1;             // Mode de fonctionnement 0=Normal, 1=Debug, 2=test_devices, 3 test 
 // 1 : debug infos will be printed on the display in a normal behavior
 // 3 : controle du pwm par les touches
int              SERIAL_DEBUG=0;                   // Envoi d'info sur le port série
int               DEBUG=0;   // bPedalage=true;  


//------------------------ calcul de la vitesse de la roue

uint16_t EstimVitesseRoueTBD(){
  long mesureVitesse,estimVitesse;
  byte front=0;

  // Detection du front
  if (k==0) {
    u16adcRoueAvant=litVitesseRoue();
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
    // gestion des états
    float ecart=100*abs(mesureVitesse-u16Vitesse)/mesureVitesse;
    switch(u8EtatVitesse){
     case ETAT_VITESSE_DEBUT:u8EtatVitesse=ETAT_VITESSE_1MES_OK;estimVitesse=0;break;
     case ETAT_VITESSE_1MES_OK:
       if (ecart<25) u8EtatVitesse=ETAT_VITESSE_2MES_OK;
       else (u8EtatVitesse=ETAT_VITESSE_ERREUR);
     break;
     case ETAT_VITESSE_2MES_OK:
       if (ecart<25) u8EtatVitesse=ETAT_VITESSE_3MES_OK;
       else (u8EtatVitesse=ETAT_VITESSE_2MES_OK);
     break;
     case ETAT_VITESSE_3MES_OK:
       if (ecart>=25) u8EtatVitesse=ETAT_VITESSE_2MES_OK;     
     break;
    } 
    if (ecart<25) switch (u8EtatVitesse){
      case ETAT_VITESSE_1MES_OK:u16Vitesse1=mesureVitesse;break;
      case ETAT_VITESSE_2MES_OK:u16Vitesse2=u16Vitesse1;u16Vitesse1=mesureVitesse;break;
      case ETAT_VITESSE_3MES_OK:u16Vitesse3=u16Vitesse2;u16Vitesse2=u16Vitesse1;u16Vitesse1=mesureVitesse;break;
    }
    else  switch(u8EtatVitesse){
      case ETAT_VITESSE_1MES_OK:u16Vitesse1=estimVitesse;break;
      case ETAT_VITESSE_2MES_OK:u16Vitesse2=u16Vitesse1;u16Vitesse1=estimVitesse;break;
      case ETAT_VITESSE_3MES_OK:u16Vitesse3=u16Vitesse2;u16Vitesse2=u16Vitesse1;u16Vitesse1=estimVitesse;break;
    }
    // Calcul de la vitesse
  }
  if ((millis()-tempsFront)>3000) {estimVitesse=0;u8EtatVitesse=ETAT_VITESSE_DEBUT;}
  return estimVitesse; 
}
 
//------------------------ calcul de la vitesse de la roue

uint16_t EstimVitesseRoue(){
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
  if ((millis()-tempsFront)>3000) mesureVitesse=0;
  
  return mesureVitesse; 
}  
  


void loop() {
// Execution toutes les 5 ms
  
if (DEBUG==1) {bPedalage=true;}

if ((MODE==1)||(MODE==0)) {
  //**************************************************************************
  // FONCTIONNEMENT NORMAL
  //**************************************************************************
  u16Vitesse=EstimVitesseRoue();
  
  switch(k%5){
    case 0: //On mesure la batterie
       u16Batterie=litBatterie();
       
    break;
    case 1: // on mesure le courant;
       u16Courant=litCourant();
       
       break;
    case 2: // on mesure le pédalier;
       bPedalage=false;
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
      affichePWM();
      
      Heltec.display->display();  
      break;
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
      //afficheVitesseVelo();
      afficheBatterie();
      afficheCourant();
      affichePedalier();
      affichePWM();
      
      Heltec.display->display();  
  }
  

  
}
