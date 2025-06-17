// Ecran
#include <Adafruit_SH110X.h>

#define WHITE SH110X_WHITE

#if defined(ESP32) && \
    !defined(ARDUINO_ADAFRUIT_FEATHER_ESP32S2) && \
    !defined(ARDUINO_ADAFRUIT_FEATHER_ESP32S3) && \
    !defined(ARDUINO_ADAFRUIT_FEATHER_ESP32S3_NOPSRAM)
  #define BUTTON_A 15
  #define BUTTON_B 32
  #define BUTTON_C 14
#endif

Adafruit_SH1107 display = Adafruit_SH1107(64, 128, &Wire);

unsigned short int const displayWidth = 128;
unsigned short int const displayHeight = 64;


// ESP-NOW
#include <esp_now.h>
#include <WiFi.h>

uint8_t peersMAC[] = {0x94, 0xB9, 0x7E, 0x6B, 0x12, 0xE0};

typedef struct BallData {
  uint8_t id = 1;
  float ballX;
  float ballY;
  float ballDX;
  float ballDY;
  int ballSpeed;
} __attribute__((packed)) BallData;

typedef struct WinData {
  uint8_t id = 2;
} __attribute__((packed)) WinData;

typedef struct StartData {
  uint8_t id = 3;
} __attribute__((packed)) StartData;

BallData ballMessageSent = {1, 0, 0, 0, 0};

WinData winMessageSent = {2};
WinData winMessageRecieved;

WinData startMessageSent = {3};
WinData startMessageRecieved;
bool connected = false;


//Partie
bool gameStarted = false;
int scoreP1 = 0;
int scoreP2 = 0;
unsigned short int const winScore = 10;


// Pad
#include "pad.h"
Pad pad;

//Balle
#include "balle.h"
Balle balle;


void setup() {
  //Initialisation
  pinMode(BUTTON_A, INPUT_PULLUP);

  Serial.begin(115200);

  display.begin(0x3C, true);
  display.setRotation(1);
  display.clearDisplay();
  display.display();

  WiFi.mode(WIFI_STA);
  if (esp_now_init() != ESP_OK) {
    Serial.println("Erreur d'initialisation ESP-NOW");
    return;
  }
  esp_now_register_send_cb(onDataSent);
  esp_now_register_recv_cb(onDataRecv);
  esp_now_peer_info_t peerInfo = {};
  memcpy(peerInfo.peer_addr, peersMAC, 6);
  peerInfo.channel = 0;
  peerInfo.encrypt = false;
  esp_now_add_peer(&peerInfo);

  waitConnexion();

  waitStart();

}

void loop() {
  if(gameStarted){
    //PAD
    pad.movePad();

    //Balle
    balle.moveBalle();
    balle.verifiRebondsBords();
    balle.verifiRebondsPad();

    // Draw des elements
    display.clearDisplay();
    balle.drawBalle();
    pad.drawPad();
    display.display();
  }
}

void waitConnexion(){
  String text="En attende de P2...";

  printTextOnScreen(text, 100, 1);

  while(!connected) {
    sendStartData();
    delay(500);
  }
}

void waitStart(){
  String text="Appuyez sur A pour\ncommencer...";

  printTextOnScreen(text, 100, 0.5);

  balle.randBall();

  while(!gameStarted){
    int buttonInput = !digitalRead(BUTTON_A);

    if(buttonInput) {
      sendBallData();

      gameStarted = true;

    }
  }
}

void printTextOnScreen(String text, int time, float size) {
  display.clearDisplay();

  const int lineSpacing = 5; // Espacement vertical entre les lignes
  const int charHeight = 8;  // Hauteur d'un caractère de base (pour le lineHeight)

  // Séparer le texte en lignes par '\n'
  std::vector<String> lines;
  int start = 0;
  while (start < text.length()) {
    int newlineIndex = text.indexOf('\n', start);
    if (newlineIndex == -1) newlineIndex = text.length();
    lines.push_back(text.substring(start, newlineIndex));
    start = newlineIndex + 1;
  }

  // Calculer la hauteur totale
  int lineHeight = charHeight * size + lineSpacing;
  int totalTextHeight = lines.size() * lineHeight - lineSpacing;
  int startY = (displayHeight - totalTextHeight) / 2;

  // Afficher chaque ligne centrée
  for (size_t i = 0; i < lines.size(); i++) {
    String line = lines[i];
    int16_t x1, y1;
    uint16_t w, h;

    display.setTextSize(size);
    display.getTextBounds(line, 0, 0, &x1, &y1, &w, &h);

    int x = (displayWidth - w) / 2;
    int y = startY + i * lineHeight;

    display.setCursor(x, y);
    display.setTextColor(WHITE);
    display.print(line);
  }

  display.display();
  Serial.println(text);
  delay(time);
}



