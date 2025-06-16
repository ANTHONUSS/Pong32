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

  void randBall();
  void drawBalle();
  void moveBalle();
  void verifiRebondsBords();
  void verifiRebondsPad();

  float getBalleX();
  float getBalleY();
  float getBalleDX();
  float getBalleDY();
  int getBalleSpeed();

};

#endif