  /* 
    hnb_drive.ino : master drive
    Copyright (C) 2015-2016  François TROQUE, Arnaud BOULAY

    hnb_drive_beta.ino : beta testing version by Sylvain GARNAVAULT
    Version 0.99  BY JMR

    

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

/* Version 0.99 */
// Machine d'état pour controller le système
// controle de la  vitesse et du courant avec 3 niveaux de vitesse et de couple
// regulation de la vitesse autour d'une consigne ( la vitesse souhaitée) avec un correcteur proportionnel
// Remise à zero de la consigne si l'utilisateur arrete de pédaler pendant plus de 10 s
// gestion du retro-pédalage
// dans hnb_power : void afficheValeurLed(int valeur) permet d'utiliser les 10 leds de l'afficheur pour lire 
// une valeur entière décomposée sur 10 bits
// 

// LE 22/09/2016
// correction d'un bug sur la commande proportionnelle !
// NB : 


//****************************************************************************
// DEFINITION DES VALEURS UTILISATEURS
//****************************************************************************

int GEARS[] = {50,67,83};       // Vitesse grosso-modo en charge, la vitesse 10 correspond à 3 km/h et c'est lineaire
// ici, on a donc 15, 20 et 25 km/h
#define ABSOLUTE_MAX_SPEED  80  // seuil maximum pour la commande moteur
#define LOW_LEVEL  340          // Seuil bas de la batterie par pas de 0.1V
#define HIGH_LEVEL 420          // Seuil haut de la batterie par pas de 0.1V


//****************************************************************************
// SETUP PROGRAM
//****************************************************************************

bool MODE=0;                    // Mode de fonctionnement 0=Normal, 1=Debug

void setup() {
  MODE = (readKeyboard() != 0); // entrer en mode debug TODO un schunte en direct
  Serial.begin(19200);          // open serial port, sets data rate to 19200 bps
  initDisplay();                // initialise les leds
  if (MODE!=0) testDisplay();   // faire le test des leds en mode debug
}

//****************************************************************************
// MAIN LOOP
//****************************************************************************

unsigned long ulTimePrevious;

void loop() {

  unsigned long ulTimeCurrent = millis();

  //**************************************************************************
  // FONCTIONNEMENT NORMAL
  //**************************************************************************

  if (MODE==0) {
    gestionClavier();
    if ((ulTimeCurrent - ulTimePrevious) >= 50) {
      CalculCourant();
      gestionEtat();
      gestionChronometreArretPedalage();
      gestionPWM();
      mesureBatterie();
      gestionBionet();
      //serialDebug();
      ulTimePrevious = ulTimeCurrent;
    }
  }

  //**************************************************************************
  // MODE DEBUG
  //**************************************************************************

  else {
    if ((ulTimeCurrent - ulTimePrevious) >= 50) {
      //gestionBatterie();
      testBionet();
      ulTimePrevious = ulTimeCurrent;
    }
  }
}


