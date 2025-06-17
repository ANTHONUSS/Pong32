Balle::Balle(){}


void Balle::drawBalle(){
  display.fillCircle(ballX, ballY, ballRadius, WHITE);
}


void Balle::moveBalle(){
  ballX+=ballSpeed*ballDX;
  ballY+=ballSpeed*ballDY;
}



void Balle::verifiRebondsBords(){
  if(ballX<-128+ballRadius){
    ballDX*=-1;
  }
  if(ballY<=ballRadius || ballY>=canvasHeight-ballRadius){
    ballDY*=-1;
  }
}


void Balle::verifiRebondsPad(){
  if(ballX+ballRadius >= pad.getPadX()) {
    
    if(ballY + ballRadius >= pad.getPadY() &&
      ballY - ballRadius <= pad.getPadY() + pad.getPadHeight()){
      ballDX*=-1;
    } else {
      
      scoreP1++;
      sendWinData();

      if(infini){
      scoreP1=0;
      scoreP2=0;
      gameStarted = false;
      infini=false;
      printTextOnScreen(convert_temps(), 2000);
      waitStart();
      
    }else if(scoreP1 == winScore) {
        String text = "P1 à gagné !";
        scoreP1=0;
        scoreP2=0;
        ballX=0;
        ballDX=0;
        ballY=-100;
        gameStarted = false;
        printTextOnScreen(text, 4000, 1);


        waitStart();
      } else {
        ballX=0;
        ballDX=0;
        ballY=-100;
        String text = "P1 - " + String(scoreP1) + " | " + String(scoreP2) + " - P2";
        gameStarted = false;
        printTextOnScreen(text, 2000, 1);
        gameStarted = true;
      }
    }
    
  }
}


float Balle::getBalleX(){
  return ballX;
}

float Balle::getBalleY(){
  return ballY;
}

float Balle::getBalleDX(){
  return ballDX;
}

float Balle::getBalleDY(){
  return ballDY;
}

int Balle::getBalleSpeed(){
  return ballSpeed;
}

void Balle::setBalleX(float x){
  ballX = x;
}

void Balle::setBalleY(float y){
  ballY = y;
}

void Balle::setBalleDX(float dx){
  ballDX = dx;
}

void Balle::setBalleDY(float dy){
  ballDY = dy;
}

void Balle::setBalleSpeed(int speed){
  ballSpeed = speed;
}





