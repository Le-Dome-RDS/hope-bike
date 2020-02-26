#define BUTTON_GAUCHE 13
#define BUTTON_GAUCHE 14
#define BUTTON_GAUCHE 27



long tempsGauche=0;
long tempsDroit=0;
long tempsMilieu=0;


//****************************************************************************
// GESTION DU CLAVIER DE COMMANDE
//****************************************************************************


void setup_keyboard(){
  pinMode(BUTTON_GAUCHE, INPUT);
  pinMode(BUTTON_DROITE, INPUT);
  pinMode(BUTTON_MILIEU, INPUT);
  
}
//-----------------------------------------------------------------------------
// ACTIONS DES BOUTONS
//-----------------------------------------------------------------------------

void gestionClavier() {
  int button_left_state,button_right_state,button_ok_state;
  
  byte buttonPressed = readKeyboard();
  unsigned long debounceTime = millis();


 // if ((buttonPressed != lastButtonPressed) && ((debounceTime - lastDebounceTime) > DEBOUNCE_DELAY)) {
 //   lastDebounceTime  = debounceTime;
 //   lastButtonPressed = buttonPressed;
    
//    if (buttonPressed != btnNONE) blink_once_display();
    
//    switch (buttonPressed) {
//      case btnUP :
//        goFaster();
//        break;
//      case btnSTOP :
//        stopSpeed(); 
//        break;    
//      case btnDOWN : 
//        goLower();
//        break;        
//      }
//    }
}

int readKeyboard() {
  //int adc_key_in = analogRead(KEYBOARD_ANALOG_INPUT);
  //byte val = btnNONE;
  //if (adc_key_in > 685 && adc_key_in < 720) val = btnUP;
  //if (adc_key_in > 510 && adc_key_in < 560) val = btnSTOP;
  //if (adc_key_in > 190 && adc_key_in < 230) val = btnDOWN;
  return 0; 
}

//-----------------------------------------------------------------------------
// ACTIONS DES BOUTONS
//-----------------------------------------------------------------------------


void goFaster() {
  //if (speedLevel< MAX_SPEED_GEAR) speedLevel++;
  //else speedLevel = 0;  
  //speed_display(speedLevel*NB_LED_BROWS/MAX_SPEED_GEAR);
}

void goLower() {
  if (speedLevel>0) speedLevel--;
  //speed_display(speedLevel*NB_LED_BROWS/MAX_SPEED_GEAR);
}

void stopSpeed() {
  speedLevel = 0;
  //speed_display(0);
}
