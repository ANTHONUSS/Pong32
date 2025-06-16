Pad::Pad() {
  padY = displayHeight/2 - (padHeight+1)/2;
}

void Pad::drawPad() {
  display.fillRect(padX, padY, padWidth, padHeight, WHITE);
}

void Pad::movePad(){
  unsigned short int joystickPos = analogRead(A3);

  if (padY < displayHeight - padHeight && joystickPos<1600) {
    padY+=padSpeed;
  } else if (padY > 0 && joystickPos>2000) {
    padY-=padSpeed;
  }
}

int Pad::getPadX(){
  return padX;
}

int Pad::getPadY(){
  return padY;
}

int Pad::getPadWidth(){
  return padWidth;
}

int Pad::getPadHeight(){
  return padHeight;
}