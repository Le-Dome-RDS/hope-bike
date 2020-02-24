void serialDebug() {
  Serial.println("------------------------------");
  Serial.println();
  
  Serial.print("Commande Moteur Reelle [1] : ");
  Serial.print(u16PWM);
  //Serial.print(" / Commande Moteur : ");
  //Serial.print(CommandeMoteur);
  Serial.print(" / Consigne Desiree : ");
  Serial.println(ConsigneDesiree);
  
  Serial.print("Courant Moteur [2] en mA ");
  
  Serial.println(" mA");

  //Serial.print("Vitesse Pédalage [7] : ");
  //Serial.println(u16Pedale);

  Serial.print("Rotation Roue [8] : ");
  Serial.println(u16VitesseVelo);

  Serial.print("Niveau batterie [15]: ");
  Serial.println(u16Batterie);

  Serial.println();
}
