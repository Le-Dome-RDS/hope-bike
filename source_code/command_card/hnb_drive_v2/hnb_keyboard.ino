//****************************************************************************
// GESTION DU CLAVIER DE COMMANDE
//****************************************************************************

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


void setup_keyboard(){
  
}



//-----------------------------------------------------------------------------
// ACTIONS DES BOUTONS
//-----------------------------------------------------------------------------

void gestionClavier() {
  byte buttonPressed = readKeyboard();
  unsigned long debounceTime = millis();

  if ((buttonPressed != lastButtonPressed) && ((debounceTime - lastDebounceTime) > DEBOUNCE_DELAY)) {
    lastDebounceTime  = debounceTime;
    lastButtonPressed = buttonPressed;
    
    if (buttonPressed != btnNONE) blink_once_display();
    
    switch (buttonPressed) {
      case btnUP :
        goFaster();
        break;
      case btnSTOP :
        stopSpeed(); 
        break;    
      case btnDOWN : 
        goLower();
        break;        
      }
    ConsigneDesiree = (speedLevel!=0) ? GEARS[speedLevel-1] : 0;
    }
}

int readKeyboard() {
  int adc_key_in = analogRead(KEYBOARD_ANALOG_INPUT);
  byte val = btnNONE;
  if (adc_key_in > 685 && adc_key_in < 720) val = btnUP;
  if (adc_key_in > 510 && adc_key_in < 560) val = btnSTOP;
  if (adc_key_in > 190 && adc_key_in < 230) val = btnDOWN;
  return val; 
}

//-----------------------------------------------------------------------------
// ACTIONS DES BOUTONS
//-----------------------------------------------------------------------------

#define NB_LED_BROWS 6

void goFaster() {
  if (speedLevel< MAX_SPEED_GEAR) speedLevel++;
  else speedLevel = 0;  
  speed_display(speedLevel*NB_LED_BROWS/MAX_SPEED_GEAR);
}

void goLower() {
  if (speedLevel>0) speedLevel--;
  speed_display(speedLevel*NB_LED_BROWS/MAX_SPEED_GEAR);
}

void stopSpeed() {
  speedLevel = 0;
  speed_display(0);
}
