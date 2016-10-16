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
#define WY   ( 14) // blocks per a world
#define CX   (  4) // pixel per a cell
#define CY   (  4) // pixel per a cell
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
  bool *keypressed;
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
  uint8_t nextblock;   // next
  
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
  {3,0,0},{3,0,0},{3,0,0},{3,0,0},{3,0,0},{3,0,0},{3,0,0}
};

