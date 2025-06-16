#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SH110X.h>
#include "rom/ets_sys.h"

Adafruit_SH1107 display = Adafruit_SH1107(64, 128, &Wire);

//variable globale
float x=0;
float y=0;
float dx=0;
float dy=0;
float v=2;

int const width= 128*2;
int const height= 64;
int const rayon = 3;


//fonctions

void trajectoire(){
  x+=v*dx;
  y+=v*dy;
}

void rebond(){
  if(x<=rayon || x>=width-rayon){
    dx*=-1;
  }
  if(y<=rayon || y>=height-rayon){
    dy*=-1;
  }
}

void setup() {

  // Address 0x3C default
  Serial.begin(115200);
  display.begin(0x3C, true); 

  // Clear the buffer.
  display.clearDisplay();
  display.setRotation(1);

  //settings display
  randomSeed(analogRead(0));
  

  //mouvenement et position balle initial
  x=random(20,60);
  y=random(20,60);
  dx=random(1,6);
  while(dy==0){
    dy=random(-6,7);
  }
  Serial.println(x);
  Serial.println(y);


  dx=dx/sqrt(dx*dx+dy*dy);
  dy=dy/sqrt(dx*dx+dy*dy);

  Serial.println(dx);
  Serial.println(dy);

}

void loop() {

  display.clearDisplay();
  rebond();
  trajectoire();
  display.fillCircle(x,y,rayon,SH110X_WHITE);

  // Serial.println(x);
  // Serial.println(y);
  display.display();

}
