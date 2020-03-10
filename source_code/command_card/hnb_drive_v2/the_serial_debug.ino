void serialDebug() {
  Serial.print(" u8Etat: ");
  Serial.print(u8Etat);

  Serial.print(" u8Erreur: ");
  Serial.print(u8Erreur);
  
  Serial.print(" i16PWM: ");
  Serial.print(i16PWM);
  Serial.print(" Consigne : ");
  Serial.print(u16Consigne);
  
  Serial.print(" u16Vitesse:");
  Serial.print(u16Vitesse);
  
  Serial.print(" u16Courant:");
  Serial.print(u16Courant);
  
  Serial.print(" u16Pedalier:");
  Serial.print(u16Pedalier);

  

  Serial.print(" u16adcRoueMaintenant:");
  Serial.print(u16adcRoueMaintenant);

  Serial.print(" TempsFront:");
  Serial.print(millis()-tempsFront);

  Serial.print(" u16Batterie:");
  Serial.println(u16Batterie);
}
