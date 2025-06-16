#ifndef PAD_H
#define PAD_H

#include <Adafruit_SH110X.h>

class Pad {
private:
  unsigned short int const padWidth = 3;
  unsigned short int const padHeight = 16;

  unsigned short int const padX = displayWidth - padWidth;
  unsigned short int padY;
  
  unsigned short int const padSpeed = 3;

public:
  Pad();
  void drawPad();
  void movePad();

  int getPadX();
  int getPadY();
  int getPadWidth();
  int getPadHeight();
};

#endif