#undef DEBUG
#include <Arduboy.h>
#include "common.h"
#include "game.h"
//--------------------------------------
Game::Game(Arduboy *_pA, bool *_kp){
  state = eGAME_STT_PLAY;
  keypressed = _kp;
  pA = _pA;
  geState = eGE_STT_IDLE;
  geSeqNow  =  0;
  geTimeNow =  0; 
  geType    =  1; //lightning
  geSeqMax  = 63;
  geTimeMax =  0; 
  geSeqStep =  1; 
  pGE = new GraphicEffect(pA);
}
//--------------------------------------
void Game::reset(void){
  state = eGAME_STT_PLAY;
  geState = eGE_STT_PLAYING;  
  geTimeNow = 0;
  geSeqNow  = 0;
  geDamageTimeNow=0;
  pA->clear();
}
void Game::drawScore(void){
}
//--------------------------------------
void Game::drawTitle(void){
  pA->setCursor(64-14*6/2, 8);
  pA->print(F("some falling block puzzle"));
  pA->setCursor(64-11*6/2, 24);
  pA->print(F("HIT ANY KEY"));
  pA->setCursor(64-17*6/2, 56);
  pA->print(F("Program: koteitan"));
}
bool Game::incGE(void){
  if(geTimeNow>=geTimeMax){
    geTimeNow=0;
    geSeqNow += geSeqStep;
  }else{
    geTimeNow++;
  }
  return geSeqNow>geSeqMax;
}
void Game::loop(void){
  bool b=false;
  switch(state){
    //----------------------------------
    case eGAME_STT_TITLE_IN:
      pA->clear();
      drawTitle();
//      pGE->lightning(geSeqMax-geSeqNow-1);
      if(incGE()) state=eGAME_STT_TITLE;
    return;
    //----------------------------------
    case eGAME_STT_TITLE:
      drawTitle();
      pA->display();
      for(int k=0;k<KEYS;k++) b=b||keypressed[k];
      if(b){
        state=eGAME_STT_TITLE_OUT;
        geSeqNow=0;
        geTimeNow=0;
      }
    return;
    //----------------------------------
    case eGAME_STT_TITLE_OUT:
      drawTitle();
//      pGE->lightning(geSeqNow);
      if(incGE()){
        state  =eGAME_STT_PLAY;
        geState=eGE_STT_PLAYING;
        reset();
      }
    return;
    //----------------------------------
    case eGAME_STT_PLAY:
    case eGAME_STT_DIED:
      if(geState==eGE_STT_PLAYING){
        //do graphic effect
        drawAll();
        switch(geType){
          case 0:
//          pGE->mosaic(geSeqMax-geSeqNow-1);
          break;
          case 1:
//          pGE->lightning(geSeqMax-geSeqNow-1);
          break;
          case 2:
//          pGE->devide(geSeqMax-geSeqNow-1);
          break;
          default:
          break;
        }
        if(incGE()) geState=eGE_STT_IDLE;
      return;
    }//switch(state)
  }

  // move 

  // score

  // draw 
  drawAll();
  
  // inclement anime 

  // state
  if(state==eGAME_STT_DIED){
//    if(t_died--<=0){
//      reset();
//    }
  }
}
void Game::drawAll(){
  pA->clear();
  pA->display();
}
//--------------------------------------
void Game::drawDebug(void){
#if 0
  int i=0;
  char str[20];
#endif
//pA->setCursor(0,i*8);pA->print(getMargin());i++;
#if 0
  pA->setCursor(0,i*8);pA->print(fDbg[i]);i++;
  pA->setCursor(0,i*8);pA->print(fDbg[i]);i++;
  pA->setCursor(0,i*8);pA->print(fDbg[i]);i++;
  pA->setCursor(0,i*8);pA->print(fDbg[i]);i++;
  pA->setCursor(0,i*8);pA->print(pPlayer->d[0]);i++;
  pA->setCursor(0,i*8);pA->print(pPlayer->d[1]);i++;
#endif
}
//--------------------------------------
