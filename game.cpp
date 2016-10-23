#undef DEBUG
#include <Arduboy.h>
#include "common.h"
#include "game.h"


//--------------------------------------
Game::Game(Arduboy *_pA, bool *_kp){
  state = eGAME_STT_PLAY;
  keypressedIn = _kp;
  pA = _pA;
  geState = eGE_STT_IDLE;
  geSeqNow  =  0;
  geTimeNow =  0; 
  geType    =  1; //lightning
  geSeqMax  = 63;
  geTimeMax =  0; 
  geSeqStep =  1; 
  pGE = new GraphicEffect(pA);
  reset();
}
//--------------------------------------
void Game::reset(void){
  state = eGAME_STT_PLAY;
  geState = eGE_STT_IDLE;  
  geTimeNow = 0;
  geSeqNow  = 0;
  geDamageTimeNow=0;
  pA->clear();
  for(int y=0;y<WY/2;y++){
    for(int x=0;x<WX;x++) map[y][x]=0;
  }
  for(int y=WY/2;y<WY;y++){
    for(int x=0;x<WX;x++) map[y][x]=1;
  }
  pb[0]=(int8_t)random(0,BLOCKS);
  px[0]=initpos[pb[0]][0];
  py[0]=initpos[pb[0]][1];
  pa[0]=initpos[pb[0]][2];
  pb[1]=(int8_t)random(0,BLOCKS);
  px[1]=initpos[pb[1]][0];
  py[1]=initpos[pb[1]][1];
  pa[1]=initpos[pb[1]][2];
  nextblock=(int8_t)random(0,BLOCKS);
  for(int8_t p=0;p<PLAYERS;p++){
    memset(&keypressed   [p][0],false,sizeof(bool)*KEYS);
    memset(&keypressedOld[p][0],false,sizeof(bool)*KEYS);
    xkeyRepeatFrames [p] = xkeyRepeatFramesTh [p];
    gravityWaitFrames[p] = gravityWaitFramesTh[p];
    fixDelayFrames   [p] = fixDelayFramesTh   [p];
    spawnDelayFrames [p] = spawnDelayFramesTh [p];
  }
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
      pGE->lightning(geSeqMax-geSeqNow-1);
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
      pGE->lightning(geSeqNow);
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

  // key -> move
  memcpy(&keypressed[0][0], keypressedIn, sizeof(bool)*KEYS);
  int8_t ma[PLAYERS];
  int8_t mx[PLAYERS];
  int8_t my[PLAYERS];
  ma[0]=0;
  mx[0]=0;
  my[0]=0;
  ma[1]=0;
  mx[1]=0;
  my[1]=0;

  for(int8_t p=0;p<PLAYERS;p++){
    if(keypressed[p][KEY_XM]){
      if(keypressedOld[p][KEY_XM]){
        //repeated
        if(xkeyRepeatFrames[p]){
          // not expired -> declement
          xkeyRepeatFrames[p]--;
        }else{
          // expired
          if(moveWaitFramesLR[p]==0){
            mx[0]=-1;
            moveWaitFramesLR[p]=moveWaitFramesLRTh[p];
          }else{
            moveWaitFramesLR[p]--;
          }
        }
      }else{
        //no repeat -> move
        mx[p]=-1;
        xkeyRepeatFrames[p]=xkeyRepeatFramesTh[p]; // reset repeat
      }
    }
    if(keypressed[p][KEY_XP]){
      if(keypressedOld[p][KEY_XP]){
        //repeated
        if(xkeyRepeatFrames[p]){
          // not expired -> declement
          xkeyRepeatFrames[p]--;
        }else{
          // expired
          if(moveWaitFramesLR[p]==0){
            mx[p]=+1;
            moveWaitFramesLR[p]=moveWaitFramesLRTh[p];
          }else{
            moveWaitFramesLR[p]--;
          }
        }
      }else{
        //no repeat -> move
        mx[p]=+1;
        xkeyRepeatFrames[p]=xkeyRepeatFramesTh[p]; // reset repeat
      }
    }
    doesFallWithKey[p]=false;
    if(keypressed[p][KEY_YM]){
      if(!keypressedOld[p][KEY_YM] || moveWaitFramesUD[p]==0){
        my[p]=-1;
        moveWaitFramesUD[p]=moveWaitFramesUDTh[p];
      }else{
        moveWaitFramesUD[p]--;
      }
    }
    if(keypressed[p][KEY_YP]){
      if(!keypressedOld[p][KEY_YP] || moveWaitFramesUD[p]==0){
        my[p]=+1;
        doesFallWithKey[p]=true;
        moveWaitFramesUD[p]=moveWaitFramesUDTh[p];
      }else{
        moveWaitFramesUD[p]--;
      }
    }
    if(keypressed[p][KEY_A]){
      if(!keypressedOld[p][KEY_A] || moveWaitFramesAB[p]==0){
        ma[p]=-1;
        moveWaitFramesAB[p]=moveWaitFramesABTh[p];
      }else{
        moveWaitFramesAB[p]--;
      }
    }
    if(keypressed[p][KEY_B]){
#if 0
      Serial.print("p");
      Serial.print(p);
      Serial.print(":old");
      Serial.print(keypressedOld[p][KEY_B]);
      Serial.print(":new");
      Serial.print(keypressed[p][KEY_B]);
      Serial.print(":frame");
      Serial.print(moveWaitFramesAB[p]);
      Serial.println();
#endif
      if(!keypressedOld[p][KEY_B] || moveWaitFramesAB[p]==0){
        ma[p]=+1;
        moveWaitFramesAB[p]=moveWaitFramesABTh[p];
      }else{
        moveWaitFramesAB[p]--;
      }
    }
  }
  mx[1]=mx[0];
  my[1]=my[0];
  ma[1]=ma[0];

  //fall
  for(int8_t p=0;p<PLAYERS;p++){
    doesFallWithGravity[p]=false;
    if(fixDelayFrames[p]!=fixDelayFramesTh[p]){
      //for waiting fix
      my[p]=+1; //try fix
    }else{
      if(gravityWaitFrames[p]==0){
        //expired
        my[p]=+1; //fall
        doesFallWithGravity[p]=true;
        gravityWaitFrames[p]=gravityWaitFramesTh[p]; //reset
      }else{
        gravityWaitFrames[p]--; //dec
      }
    }
  }

  //move x -> potision
  for(int8_t p=0;p<PLAYERS;p++){
    int8_t pm=p*-2+1;
    if(mx[p]!=0){
      for(int8_t by=0;by<BY;by++){
        int8_t y = (WY-1)*p + pm*(by+py[p]);
        for(int8_t bx=0;bx<BX;bx++){
          int8_t x0 = (WX-1)*p + pm*(bx+px[p]);
          int8_t x1 = x0 + pm*mx[p];
          if(y>=0 && y<WY && x0>=0 && x0<WX && block[pb[p]][pa[p]][by][bx]){
            if(x1<0 || x1>=WX || map[y][x1]!=p){
              mx[p]=0; //cancel
            }//if (x1,y) !map
          }//if (x0,y) block
        }// for bx
      }// for by
      if(spawnDelayFrames[p]==spawnDelayFramesTh[p]){
        px[p]+=mx[p];
      }
    }
  }
  //rotate
  for(int8_t p=0;p<PLAYERS;p++){
    if(ma[p]!=0){
      int8_t pm=p*-2+1;
      int8_t a0 = pa[p];
      int8_t a1 = (pa[p]+ma[p]+ATTS)%ATTS;
      for(int8_t by=0;by<BY;by++){
        int8_t y = (WY-1)*p + pm*(by+py[p]);
        for(int8_t bx=0;bx<BX;bx++){
          int8_t x = (WX-1)*p + pm*(bx+px[p]);
          if(block[pb[p]][a1][by][bx]){
            bool bcoll=false;
            if(y>=0 && y<WY && x>=0 && x<WX && map[y][x]!=p) bcoll=true;
            if((p==0 && y>=WY) || (p==1 && y<0) || x>=WX || x<0) bcoll=true;
            if(bcoll){ // collision
              a1=a0; // cancel
            }
          }
        }
      }
      if(spawnDelayFrames[p]==spawnDelayFramesTh[p]){
        pa[p]=a1;
      }
    }
  }

  //move y -> potision
  bool isTouched[PLAYERS]={false,false};
  for(int8_t p=0;p<PLAYERS;p++){
    int8_t pm=p*-2+1;
    if(my[p]!=0){
      for(int8_t by=0;by<BY;by++){
        int8_t y0 = (WY-1)*p + pm*(by+py[p]);
        int8_t y1 = y0       + pm*    my[p];
        for(int8_t bx=0;bx<BX;bx++){
          int8_t x = (WX-1)*p + pm*(bx+px[p]);
          if(y0>=0 && y0<WY && x>=0 && x<WX && block[pb[p]][pa[p]][by][bx]){
            if(y1>=WY || map[y1][x]!=p){
              //touched
              isTouched[p]=true;
              my[p]=0; //cancel motion
            }else{//if touch
              //go down
//              fixDelayFrames[p]=fixDelayFramesTh[p]; // reset
            }//if touch
          }//if (x,y0) block
        }// for bx
      }// for by
    }//if my
    if(spawnDelayFrames[p]==spawnDelayFramesTh[p]){
      py[p]+=my[p];
    }
  }//p

  //touched -> fix
  bool isfix[PLAYERS]={false,false};
  for(int8_t p=0;p<PLAYERS;p++){
    if(isTouched[p]){
      if(doesFallWithKey[p] || fixDelayFrames[p]==0){
        //fix expired
        isfix[p]=true;
        fixDelayFrames[p]=fixDelayFramesTh[p]; // reset
      }else{
        fixDelayFrames[p]--; //dec
      }
    }
  }
  //fix -> renew map, effectedY
  int8_t effectedY[PLAYERS][BY]={{-1,-1,-1,-1},{-1,-1,-1,-1}};
  for(int8_t p=0;p<PLAYERS;p++){
    if(isfix[p]){
      //copy block -> map
      int8_t pm=p*-2+1;
      for(int8_t by=0;by<BY;by++){
        int8_t y = (WY-1)*p + pm*(by+py[p]);
        for(int8_t bx=0;bx<BX;bx++){
          int8_t x = (WX-1)*p + pm*(bx+px[p]);
          if(y>=0 && y<WY && x>=0 && x<WX && block[pb[p]][pa[p]][by][bx]){
            map[y][x]=(~p)&0x01; // fix
            effectedY[p][by] = y;
            spawnDelayFrames[p]=spawnDelayFramesTh[p]-1; //start spawn count
          }
        }//bx
      }//by
    }//isfix
  }//p

  //delete lines
  for(int8_t p=0;p<PLAYERS;p++){
    int8_t pm=p*-2+1;
    for(int8_t by=0;by<BY;by++){
      int8_t y=effectedY[p][by];
      if(y!=-1){
        bool isdel=true;
        for(int8_t x=0;x<WX;x++){
          if(map[y][x]==p){
            isdel = false;
            break;
          }
        }
        if(isdel){
          //delete line
          for(int8_t y1=y-pm;y1<WY&&y1>=0;y1+=-pm){
            for(int8_t x1=0;x1<WX;x1++){
              map[y1+pm][x1]=map[y1][x1];
            }
          }
          for(int8_t x1=0;x1<WX;x1++){
            map[(WY-1)*p][x1]=p; // top line is empty
          }
        }
      }
    }
  }

  //wait new block spawn
  for(int8_t p=0;p<PLAYERS;p++){
    if(spawnDelayFrames[p]!=spawnDelayFramesTh[p]){
      //is waiting
      if(spawnDelayFrames[p]==0){
        //expired
        //set new block
        pb[p]=nextblock;
        px[p]=initpos[pb[p]][0];
        py[p]=initpos[pb[p]][1];
        pa[p]=initpos[pb[p]][2];
        //set next
        nextblock=(int8_t)random(0,BLOCKS);
        //reset counter
        xkeyRepeatFrames[p] = xkeyRepeatFramesTh[p];
        moveWaitFramesLR[p] = moveWaitFramesLRTh[p];
        moveWaitFramesUD[p] = moveWaitFramesUDTh[p];
        moveWaitFramesAB[p] = moveWaitFramesABTh[p];
        fixDelayFrames  [p] = fixDelayFramesTh  [p];
        spawnDelayFrames[p] = spawnDelayFramesTh[p];
      }else{
        spawnDelayFrames[p]--;
      }//is expired
    }//is waiting
  }//p


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
  for(int p=0;p<PLAYERS;p++){
    memcpy(&keypressedOld[p][0],&keypressed[p][0],sizeof(bool)*KEYS);
  }
}
void Game::drawAll(){
  pA->clear();
  if(state==eGAME_STT_PLAY){
    int wx0=24;
    int wy0= 4;
    //map ---------------
    //  horizontal line
    for(int y=0;y<WY-1;y++){
      for(int x=0;x<WX;x++){
        if(map[y][x]^map[y+1][x]){
          pA->drawLine(wx0+CX*x,wy0+CY*y+CY,wx0+CX*x+CX,wy0+CY*y+CY,1);
        }
      }
    }
    //  vertical line
    for(int y=0;y<WY;y++){
      for(int x=0;x<WX-1;x++){
        if(map[y][x]^map[y][x+1]){
          pA->drawLine(wx0+CX*x+CX,wy0+CY*y,wx0+CX*x+CX,wy0+CY*y+CY,1);
        }
      }
    }
    //block-------------
    for(int p=0;p<PLAYERS;p++){
      int8_t b=pb[p];
      int8_t a=pa[p];
      for(int8_t by=0;by<=BY;by++){
        for(int8_t bx=0;bx<=BX;bx++){
          char h  = (bx==BX||by==BY)?0:block[b][a][by  ][bx  ];
          char hx = (bx== 0||by==BY)?0:block[b][a][by  ][bx-1];
          char hy = (bx==BX||by== 0)?0:block[b][a][by-1][bx  ];
#if 0
          Serial.println("--------------");
          Serial.print("(");
          Serial.print(bx);
          Serial.print(",");
          Serial.print(by);
          Serial.print(")->(");
          Serial.print((int)(h));
          Serial.print(",");
          Serial.print((int)(hx));
          Serial.print(")->(");
          Serial.print((int)(hy));
          Serial.print(",");
          Serial.print((int)(h^hx));
          Serial.print(",");
          Serial.print((int)(h^hy));
          Serial.println(")");
#endif
          //vertical line
          int8_t d = p*-2+1;
          int8_t x = d*(px[p]+bx) + WX*p;
          int8_t y = d*(py[p]+by) + WY*p;
#if 0
          if(h) pA->drawPixel(wx0+CX*x+CX/2*d,wy0+CY*y+CY/2*d,1);
#endif
          if(h^hx && x>0 && x<WX && y>=0 && y<WY){
            pA->drawLine(
              wx0+CX*x+ 0,
              wy0+CY*y+ 0,
              wx0+CX*x+ 0,
              wy0+CY*y+CY*d,1);
          }
          //horizontal line
          if(h^hy && y>0 && y<WY && x>=0 && x<WX){
            pA->drawLine(
              wx0+CX*x+ 0,
              wy0+CY*y+ 0,
              wx0+CX*x+CX*d,
              wy0+CY*y+ 0,1);
          }
        }// for bx
      }// for by
    }//for p
    //border ---------------
    pA->drawRect(wx0,wy0,CX*WX+1,CY*WY+1,1);
    //display ---------------
    pA->display();
  }//  if(state==eGAME_STT_PLAY)
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
