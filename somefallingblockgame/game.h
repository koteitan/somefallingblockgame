#pragma once
#include <Arduboy.h>
#include "graphiceffect.h"
#include "common.h"
/*
 cell  = minimum unit
 block = falling stuff consisting of 4 cells
 map   = pattern of all cells
*/
// for map
#define DIMS (  2) // number of dimension
#define SX   (128) // pixel per a screen
#define SY   ( 64) // pixel per a screen
#define WX   ( 10) // blocks per a world
#define WY   ( 18) // blocks per a world
#define CX   (  3) // pixel per a cell
#define CY   (  3) // pixel per a cell
// for block
#define BLOCKS (7) // types of blocks
#define ATTS   (4) // types of attitude of a block (0, 90, 180, -90)
#define BX     (4) // x size of block definition
#define BY     (4) // x size of block definition 
// for player
#define PLAYERS (2)
// scale change
#define WY2SY ((float)SY/WY)
#define WX2SX ((float)SX/WX)
#define SY2WY ((float)WY/SY)
#define SX2WX ((float)WX/SX)

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
  // for arduboy
  Arduboy *pA;
  bool *keypressedIn;
  bool keypressed[PLAYERS][KEYS];
  bool keypressedOld[PLAYERS][KEYS];
  Game(Arduboy *_pA, bool *_kp);

  //Graphic Effect
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
  //for this game
  eGAME_STT state;
  char map[WY][WX];
  uint8_t pb[PLAYERS]; // player block
  int8_t  px[PLAYERS]; // player x
  int8_t  py[PLAYERS]; // player y
  int8_t  pa[PLAYERS]; // player attitude
  uint8_t nextblock[PLAYERS];   // next
  int8_t xkeyRepeatFrames   [2]={ 0, 0};// time with key L R
  int8_t xkeyRepeatFramesTh [2]={ 4, 4};// time with key L R until repeat
  int8_t moveWaitFramesLR   [2]={ 0, 0};// time with key down
  int8_t moveWaitFramesUD   [2]={ 0, 0};// time with key down
  int8_t moveWaitFramesAB   [2]={ 0, 0};// time with key down
  int8_t moveWaitFramesLRTh [2]={ 2, 2};// time with key down until repeat 
  int8_t moveWaitFramesUDTh [2]={ 3, 3};// time with key down until repeat 
  int8_t moveWaitFramesABTh [2]={10,10};// time with key down until repeat 
  int8_t gravityWaitFrames  [2]={ 0, 0};// time from last motion
  int8_t gravityWaitFramesTh[2]={60,60};// time from last motion until next fall
  int8_t fixDelayFrames     [2]={ 0, 0};// time from touch ground
  int8_t fixDelayFramesTh   [2]={15,15};// time from touch ground until fix
  int8_t spawnDelayFrames   [2]={10,10};// time from delete line
  int8_t spawnDelayFramesTh [2]={10,10};// time from delete line until next block spawns
  bool doesFallWithGravity [2]={false,false};
  bool doesFallWithKey     [2]={false,false};
  void reset    (void);
  void drawAll  (void);
  void loop     (void);
  void drawDebug(void);
  void drawTitle(void);
  bool incGE(void);
  float fDbg[10];
};

// (block[b][t][y]>>x) & 0x01 = is set (x,y) of type b block in turn t
const uint8_t block[BLOCKS][ATTS][BY][BX]={
  {//T
    {{0,0,0,0},{1,1,1,0},{0,1,0,0},{0,0,0,0}},
    {{0,1,0,0},{0,1,1,0},{0,1,0,0},{0,0,0,0}},
    {{0,0,0,0},{0,1,0,0},{1,1,1,0},{0,0,0,0}},
    {{0,1,0,0},{1,1,0,0},{0,1,0,0},{0,0,0,0}}
  },{//J
    {{0,0,0,0},{1,1,1,0},{0,0,1,0},{0,0,0,0}},
    {{0,1,1,0},{0,1,0,0},{0,1,0,0},{0,0,0,0}},
    {{0,0,0,0},{1,0,0,0},{1,1,1,0},{0,0,0,0}},
    {{0,1,0,0},{0,1,0,0},{1,1,0,0},{0,0,0,0}}
  },{//L
    {{0,0,0,0},{1,1,1,0},{1,0,0,0},{0,0,0,0}},
    {{1,1,0,0},{0,1,0,0},{0,1,0,0},{0,0,0,0}},
    {{0,0,0,0},{0,0,1,0},{1,1,1,0},{0,0,0,0}},
    {{0,1,0,0},{0,1,0,0},{0,1,1,0},{0,0,0,0}}
  },{//Z
    {{0,0,0,0},{1,1,0,0},{0,1,1,0},{0,0,0,0}},
    {{0,0,1,0},{0,1,1,0},{0,1,0,0},{0,0,0,0}},
    {{0,0,0,0},{1,1,0,0},{0,1,1,0},{0,0,0,0}},
    {{0,0,1,0},{0,1,1,0},{0,1,0,0},{0,0,0,0}}
  },{//S
    {{0,0,0,0},{0,1,1,0},{1,1,0,0},{0,0,0,0}},
    {{1,0,0,0},{1,1,0,0},{0,1,0,0},{0,0,0,0}},
    {{0,0,0,0},{0,1,1,0},{1,1,0,0},{0,0,0,0}},
    {{1,0,0,0},{1,1,0,0},{0,1,0,0},{0,0,0,0}}
  },{//I
    {{0,0,0,0},{1,1,1,1},{0,0,0,0},{0,0,0,0}},
    {{0,0,1,0},{0,0,1,0},{0,0,1,0},{0,0,1,0}},
    {{0,0,0,0},{1,1,1,1},{0,0,0,0},{0,0,0,0}},
    {{0,0,1,0},{0,0,1,0},{0,0,1,0},{0,0,1,0}}
  },{//O
    {{0,0,0,0},{0,1,1,0},{0,1,1,0},{0,0,0,0}},
    {{0,0,0,0},{0,1,1,0},{0,1,1,0},{0,0,0,0}},
    {{0,0,0,0},{0,1,1,0},{0,1,1,0},{0,0,0,0}},
    {{0,0,0,0},{0,1,1,0},{0,1,1,0},{0,0,0,0}}
  }
};
//initpos[b][d] = initial d th dimensional position for block b.
// d=0:x, 1:y, 2:attitude
const int8_t initpos[BLOCKS][3]={
  // T       J       L       Z       S       I       O
  {3,-1,0},{3,-1,0},{3,-1,0},{3,-1,0},{3,-1,0},{3,-1,0},{3,-1,0}
};

