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
  bool modeInfini;
} __attribute__((packed)) BallData;

typedef struct WinData {
  uint8_t id = 2;
} __attribute__((packed)) WinData;

typedef struct StartData {
  uint8_t id = 3;
} __attribute__((packed)) StartData;

BallData ballMessageSent = {1, 0, 0, 0, 0, false};

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
//cleia
bool infini=false;
int temps=0;
hw_timer_t* timer = NULL;


// Pad
#include "pad.h"
Pad pad;

//Balle
#include "balle.h"
Balle balle;

//Temps écoulé (mode infini)
void IRAM_ATTR temps_int(){
  temps++;
}


void setup() {
  //Initialisation
  pinMode(BUTTON_A, INPUT_PULLUP);
  //cleia
  pinMode(BUTTON_B, INPUT_PULLUP);
  timer=timerBegin(1000000);
  timerAttachInterrupt(timer,&temps_int);
  timerAlarm(timer,1000000,true,0);  

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
    if(infini){
      affiche_temps();
    }
    balle.drawBalle();
    pad.drawPad();
    display.display();
  }
}

void waitConnexion(){
  String text="En attende de P2...";

  printTextOnScreen(text, 0);

  while(!connected) {
    sendStartData();
    delay(500);
  }
}

void waitStart(){
  String text="Appuyez sur A pour commencer...";

  printTextOnScreen(text, 0);

  balle.randBall();

  while(!gameStarted){
    int buttonInput = !digitalRead(BUTTON_A);
    //cleia
    int inputB= !digitalRead(BUTTON_B);

    if(buttonInput) {
      sendBallData();
      gameStarted = true;

    }
    //cleia
    if(inputB){
      gameStarted=true;
      infini=true;
      temps=0;
      sendBallData();
    }

  }
}

void printTextOnScreen(String text, int time){
  display.clearDisplay();
  int textWidth = text.length() * 6;
  int x = (displayWidth - textWidth) / 2;
  int y = displayHeight / 2;
  display.setCursor(x, y);
  display.setTextColor(WHITE);
  display.print(text);
  display.display();

  Serial.println(text);

  delay(time);
}


String convert_temps(){
  return String(temps/60)+":"+String(temps%60);
}

void affiche_temps(){
  display.setCursor(100,0);
  display.print(convert_temps());
}

