////  -------------- Gestion de la vitesse du velo ------------------ 
void affiche_tableau_vitesse_velo() {
  for (int i=0;i<4;i++) {
    Serial.print(u16TableauVitesse[i]);
    Serial.print(" ");
  }
  Serial.println(u16TableauVitesse[4]);
}

////  --------------
void ajoute_un_element_et_decale_vitesse_velo(uint16_t nouveau){
  for (int i=0;i<4;i++) u16TableauVitesse[i]=u16TableauVitesse[i+1];
  u16TableauVitesse[4]=nouveau;
}
////  --------------
uint16_t median_vitesse_velo(){
  return (u16TableauVitesse[2]);
}
////  --------------
uint16_t moyenne_vitesse_velo(){
  long calcul=0;
  for (int i=0;i<5;i++){
    calcul=calcul+u16TableauVitesse[i];
  }
  return(calcul/5);
}

////  -------------- Gestion du pedalier ------------------ 

void affiche_tableau_pedalier() {
  for (int i=0;i<4;i++) {
    Serial.print(u16TableauPedalier[i]);
    Serial.print(" ");
  }
  Serial.println(u16TableauPedalier[4]);
}

void ajoute_un_element_et_decale_pedalier(uint16_t nouveau){
  for (int i=0;i<4;i++) u16TableauPedalier[i]=u16TableauPedalier[i+1];
  u16TableauPedalier[4]=nouveau;
}

uint16_t median_pedalier(){
  return (u16TableauPedalier[2]);
}

uint16_t moyenne_pedalier(){
  long calcul=0;
  for (int i=0;i<5;i++){
    calcul=calcul+u16TableauPedalier[i];
  }
  return(calcul/5);
}
////  -------------- Gestion du courant ------------------ 

void affiche_tableau_courant() {
  for (int i=0;i<4;i++) {
    Serial.print(u16TableauCourant[i]);
    Serial.print(" ");
  }
  Serial.println(u16TableauCourant[4]);
}

void ajoute_un_element_et_decale_courant(uint16_t nouveau){
  for (int i=0;i<4;i++) u16TableauCourant[i]=u16TableauCourant[i+1];
  u16TableauCourant[4]=nouveau;
}

uint16_t median_courant(){
  return (u16TableauCourant[2]);
}

uint16_t moyenne_courant(){
  long calcul=0;
  for (int i=0;i<5;i++){
    calcul=calcul+u16TableauCourant[i];
  }
  return(calcul/5);
}
