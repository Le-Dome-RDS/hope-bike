void serialDebug() {
  Serial.println("------------------------------");
  Serial.println();
  
  Serial.print("Commande Moteur Reelle [1] : ");
  Serial.print(i8PWM);
  //Serial.print(" / Commande Moteur : ");
  //Serial.print(CommandeMoteur);
  Serial.print(" / Consigne Desiree : ");
  Serial.println(ConsigneDesiree);
  
  Serial.print("Courant Moteur [2] : ");
  Serial.print(u16CourantMoteur);
  Serial.print(" / en Ampères : ");
  Serial.print(s16CourantMotUnitAmp);
  Serial.println(" A");

  Serial.print("Vitesse Pédalage [7] : ");
  Serial.println(u8VitessePedale);

  Serial.print("Rotation Roue [8] : ");
  Serial.println(u8RotationRoue);

  Serial.print("Niveau batterie [15]: ");
  Serial.println(u16NiveauBatterie);

  Serial.println();
}

