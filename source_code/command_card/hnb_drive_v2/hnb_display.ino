//****************************************************************************
// OLED screen
//****************************************************************************
void blink_once_display(){
  
}


void speed_display(int speed){
  Heltec.display->drawString(0, 0, "la vitesse");
}

void battery_display(int battery){
  
  Heltec.display->drawString(0, 20, "la batterie");
}

void debug_display(){
  
}

void setup_display() {
  Heltec.begin(true /*DisplayEnable Enable*/, false /*LoRa Disable*/, false /*Serial Enable*/);
  Heltec.display->flipScreenVertically();
  Heltec.display->setFont(ArialMT_Plain_10);
}

void test_display() {
  
}
