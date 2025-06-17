void sendStartData(){
  esp_err_t res = esp_now_send(peersMAC, (uint8_t *)&startMessageSent, sizeof(startMessageSent));
  if (res != ESP_OK) {
    Serial.println("Erreur lors de l'envoi");
  }
}

void sendWinData() {
  esp_err_t res = esp_now_send(peersMAC, (uint8_t *)&winMessageSent, sizeof(winMessageSent));
  if (res != ESP_OK) {
    Serial.println("Erreur lors de l'envoi");
  }
}


void onDataSent(const uint8_t* mac, esp_now_send_status_t status) {
  Serial.println();
  Serial.print("Envoi vers ");
  for(int i=0;i<6;i++){ Serial.printf("%02X",mac[i]); if(i<5) Serial.print(":"); }
  Serial.print(" -> ");
  Serial.println(status == ESP_NOW_SEND_SUCCESS ? "Succès" : "Échec");
}

void onDataRecv(const esp_now_recv_info_t *info, const uint8_t *incomingData, int len) {
  uint8_t id = incomingData[0];

  if (id == 1 && len == sizeof(BallData)) {
    memcpy(&ballMessageRecieved, incomingData, sizeof(ballMessageRecieved));
    Serial.println();
    Serial.println("Donnée récupérée : ballData");
    
    onId1Recieved();

  } else if (id == 2 && len == sizeof(WinData)) {
    memcpy(&winMessageRecieved, incomingData, sizeof(winMessageRecieved));
    Serial.println();
    Serial.println("Donnée récupérée : P2 à gagné");

    onId2Recieved();
    
  } else if (id == 3 && len == sizeof(StartData)) {
    memcpy(&startMessageRecieved, incomingData, sizeof(startMessageRecieved));
    Serial.println();
    Serial.println("Donnée récupérée : P2 Connecté, envoi à P1...");

    connected = true;
    sendStartData();

  } else {
    Serial.println("Type inconnu ou taille incorrecte");
  }
}



void onId1Recieved(){
  balle.setBalleX(ballMessageRecieved.ballX-128);
  balle.setBalleY(ballMessageRecieved.ballY);
  balle.setBalleDX(ballMessageRecieved.ballDX);
  balle.setBalleDY(ballMessageRecieved.ballDY);
  balle.setBalleSpeed(ballMessageRecieved.ballSpeed);
  infini=ballMessageRecieved.modeInfini;
  temps=0;

  gameStarted = true;
}

void onId2Recieved(){
  scoreP2++;

  if(infini){
    scoreP1=0;
    scoreP2=0;
    gameStarted = false;
    infini=false;
    printTextOnScreen(convert_temps(), 2000);

    waitStart();
  } else if(scoreP2 == winScore) {
    String text = "P2 à gagné !";
    scoreP1=0;
    scoreP2=0;
    gameStarted = false;
    printTextOnScreen(text, 4000);

    waitStart();
  } else {
    String text = "P1 - " + String(scoreP1) + " | " + String(scoreP2) + " - P2";
    gameStarted = false;
    printTextOnScreen(text, 2000);
    gameStarted = true;
  }
}