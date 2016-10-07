#pragma once
#include <Arduboy.h>
#include "graphiceffect.h"

typedef enum{
  eGAME_STT_TITLE_IN  = 0,
  eGAME_STT_TITLE     = 1,
  eGAME_STT_TITLE_OUT = 2,
  eGAME_STT_PLAY      = 3,
  eGAME_STT_DIED      = 4,
  eGAME_STTS          = 5
}eGAME_STT;
typedef enum{
  eGE_STT_IDLE    = 0,
  eGE_STT_PLAYING = 1,
  eGE_STTS        = 2
}eGE_STT;
class Game{
  public:
  Arduboy *pA;
  bool *keypressed;
  eGAME_STT state;
  eGE_STT geState;
  GraphicEffect *pGE;
  int geSeqNow  = 0;
  int geSeqMax  = 0;
  int geTimeNow = 0;
  int geTimeMax = 0;
  int geSeqStep = 0;
  int geType    = 0;
  int geTypeMax = 3;
  int geDamageTimeNow = 0;
  int geDamageTimeMax = 4;
  Game(Arduboy *_pA, bool *_kp);
  void reset    (void);
  void drawScore(void);
  void drawAll  (void);
  void loop     (void);
  void drawDebug(void);
  void drawTitle(void);
  bool incGE(void);
  float fDbg[10];
};
