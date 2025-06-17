Balle::Balle(){
  randBall();
}

void Balle::randBall(){
  randomSeed(analogRead(0));
  ballX=random(20,60);
  ballY=random(20,60);

  ballDX=random(1,6);
  ballDY=random(-5,6);
  while(ballDY==0){
    ballDY=random(-5,6);
  }
  float normFunc = sqrt(pow(ballDX, 2)+pow(ballDY, 2));
  ballDX=ballDX/normFunc;
  ballDY=ballDY/normFunc;

  ballSpeed=random(4,6);
}



void Balle::drawBalle(){
  display.fillCircle(ballX, ballY, ballRadius, WHITE);
}


void Balle::moveBalle(){
  ballX+=ballSpeed*ballDX;
  ballY+=ballSpeed*ballDY;
}



void Balle::verifiRebondsBords(){
  if(ballX>=canvasWidth-ballRadius){
    ballDX*=-1;
  }
  if(ballY<=ballRadius || ballY>=canvasHeight-ballRadius){
    ballDY*=-1;
  }
}


void Balle::verifiRebondsPad(){
  if(ballX-ballRadius <= pad.getPadX()+pad.getPadWidth()) {
    
    if(ballY + ballRadius >= pad.getPadY() &&
      ballY - ballRadius <= pad.getPadY() + pad.getPadHeight()){
      ballDX*=-1;
    } else {
      
      scoreP2++;
      sendWinData();

      if(scoreP2 == winScore) {
        String text = "P2 à gagné !";
        scoreP1=0;
        scoreP2=0;
        ballX=64;
        ballDX=0;
        ballY=-100;
        gameStarted = false;
        printTextOnScreen(text, 4000, 1);


        waitStart();
      } else {
        ballX=64;
        ballDX=0;
        ballY=-100;
        String text = "P1 - " + String(scoreP1) + " | " + String(scoreP2) + " - P2";
        gameStarted = false;
        printTextOnScreen(text, 2000, 1);
        gameStarted = true;

        balle.randBall();
        sendBallData();
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





