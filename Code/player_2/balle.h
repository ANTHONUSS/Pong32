#ifndef BALLE_H
#define BALLE_H

class Balle{
private:
  float ballX;
  float ballY;
  float ballDX;
  float ballDY;
  int ballSpeed;
  int const ballRadius = 3;

  int const canvasWidth= 128*2;
  int const canvasHeight= 64;

public:
  Balle();

  void drawBalle();
  void moveBalle();
  void verifiRebondsBords();
  void verifiRebondsPad();

  float getBalleX();
  float getBalleY();
  float getBalleDX();
  float getBalleDY();
  int getBalleSpeed();

  void setBalleX(float x);
  void setBalleY(float y);
  void setBalleDX(float dx);
  void setBalleDY(float dy);
  void setBalleSpeed(int speed);

};

#endif