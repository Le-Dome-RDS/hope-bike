   /* 
    hnb_drive.ino : master drive
    Copyright (C) 2015-2016  François TROQUE, Arnaud BOULAY

    hnb_drive_beta.ino : beta testing version by Sylvain GARNAVAULT
    Version 0.99  BY JMR

    version for hardware v2 by Jean-Marc Routoure. GREYC. Unicaen

    

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


#include "Arduino.h"
#include "heltec.h"    //carte d'interface avec écran OLED intégré
#include <Adafruit_ADS1015.h>   //Convertisseur analogique-numérique 12 bits
#include <Adafruit_MCP4725.h>   //convertisseur numérique-analogique 12 bits
#include "MCP7940.h"   //horloge temps réél intégré pour la durée d'utilisation


//****************************************************************************
// GLOBAL USER VARIABLES
//****************************************************************************

int CONSIGNE[] = {50,67,83};       // Vitesse grosso-modo en charge, la vitesse 10 correspond à 3 km/h et c'est lineaire
// ici, on a donc 15, 20 et 25 km/h
#define ABSOLUTE_MAX_SPEED  80  // seuil maximum pour la commande moteur
#define LOW_LEVEL  340          // Seuil bas de la batterie par pas de 0.1V
#define HIGH_LEVEL 420          // Seuil haut de la batterie par pas de 0.1V

int MODE=1;                    // Mode de fonctionnement 0=Normal, 1=Debug, 2=test_devices
int DEBUG=0;                   // Envoi d'info sur le port série
// 1 : debug infos will be printed on the display in a normal behavior
// 2 : keyboard, display and I2C devices will be tested

byte speedLevel = 1;              // niveau de vitesse
byte ConsigneDesiree = 0;         // consigne choisie







// Gestion de l'horloge à 5 ms
volatile int interruptCounter = 0; //ce compteur est incrémenté toutes les 5 ms
int numberOfInterrupts = 0; //ce compteur est réinitialisé à chaque calcul de la vitesse. omega=nombre de tour/(numberofInterrupt*5ms)

hw_timer_t * timer = NULL;
portMUX_TYPE timerMux = portMUX_INITIALIZER_UNLOCKED;

void IRAM_ATTR onTimer() {
  portENTER_CRITICAL_ISR(&timerMux);
     interruptCounter++;
  portEXIT_CRITICAL_ISR(&timerMux);
 }


//****************************************************************************
// SETUP PROGRAM
//****************************************************************************



void setup() {
  //initialisation d'une horloge interne à 5 ms
  timer = timerBegin(0, 80, true);            //precaler 80
  timerAttachInterrupt(timer, &onTimer, true);
  timerAlarmWrite(timer, 5000, true);        // 5ms
  timerAlarmEnable(timer);
  // -------------------------------------------------------------
  
  
  setup_i2c();      
  //setup_keyboard();
  //setup_display();  

  
}




//****************************************************************************
// MAIN LOOP
//****************************************************************************


long k=0;
int16_t         u16adcRouePrecedent=0,u16adcRoueMaintenant=0; //valeur lue sur l'ADC
int16_t         u16Batterie=0;                                //Niveau de la batterie en V;
int16_t         u16Courant=0;                                 //Valeur du courant débité en mA;
int16_t         u16CourantMoteurInitial     = 0;            // Valeur du courant au démarrage du vélo
int16_t         u16Pedalier=0;                                //Valeur lue sur l'

int16_t         u16VitesseVelo     = 0;                       // vitesse de rotation de la roue déduite      
boolean         pedalage=false;                             // Est égale à 1 si un pédalage est détecté
byte            u8Etat= 0;                                  // Etat du velo
// chronometre pour mettre à zero la consigne si l'utilisateur ne pedale pas pendant plus de 5 secondes
boolean         bChronoArretPedalage         =  false;      // est ce que le chronometre a demarré
unsigned long   u32ChronoArrretDebut   =  millis();         //valeur du temps au demarrage du chronometre
int8_t          i8PWMTemp              = 0;                 // la valeur PWM  temporaire utilisee pour rendre continue PWM
int8_t          i8PWM = 0;                                  // CRITURE BIONET la valeur PWM  envoyée au moteur 



void loop() {
byte front=0;


if ((MODE==1)) {
  //**************************************************************************
  // FONCTIONNEMENT NORMAL
  //**************************************************************************

  //------------------------ calcul de la vitesse de la roue
  portENTER_CRITICAL(&timerMux);
       numberOfInterrupts= numberOfInterrupts + interruptCounter ;  
       interruptCounter=0;
  portEXIT_CRITICAL(&timerMux); 
  // recherche d'un front sur la roue
  
  u16adcRoueMaintenant = litVitesseRoue();    //2ms
  if (u16adcRouePrecedent<100 && u16adcRoueMaintenant>150) front=1; 
  u16adcRouePrecedent=u16adcRoueMaintenant;
  if (front==1){
    u16VitesseVelo=1493000/numberOfInterrupts; //en m/h
    numberOfInterrupts=0;
  }
  if (numberOfInterrupts>300) u16VitesseVelo=0;

  // mesure des informations, calcul et affichage
  
  switch(k%5){
    case 0: //On mesure la batterie
       u16Batterie=litBatterie();
    break;
    case 1: // on mesure le courant;
       u16Courant=litCourant();
       break;
    case 2: // on mesure le pédalier;
       pedalage=false;
       u16Pedalier=litPedalier();
       if (u16Pedalier>500 && u16Pedalier<1500) pedalage=true;   

       break;
    case 3:// on gere le clavier et la machine d'état;
       gestionEtat();
       gestionPWM();
       break;   
    case 4: // ,On affiche les résultats
      Heltec.display->clear();    //clear the display
      afficheVitesseVelo(u16VitesseVelo);
      afficheBatterie(u16Batterie);
      afficheCourant(u16Courant);
      affichePedalier(u16Pedalier);
      afficheMode(u8Etat);
      afficheCommande(i8PWM);
      Heltec.display->display();  
      if (DEBUG==1) Serial.print("OK"); Serial.print(" ");
    break;
      default:break;
  }
  k++;
  if (DEBUG==1) Serial.print(u16adcRouePrecedent); Serial.print(" ");
  
  if (DEBUG==1) Serial.print(u16VitesseVelo); Serial.print(" ");
  if (DEBUG==1) Serial.print(k); Serial.print(" ");
  if (DEBUG==1) Serial.print(k%5); Serial.println(" ");
  

  
    
      //
      //gestionChronometreArretPedalage();
      //gestionPWM();
      //mesureBatterie();
      //gestion_i2c(); // reading of the I2C ADC  
      //serialDebug();
      //k++;
    }
  else if (MODE==2) {
    //test_i2c();
    //test_display();
    //test_keyboard();
    //test_devices();
    //  
  }
  

  
}
