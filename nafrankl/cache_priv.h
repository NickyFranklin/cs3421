#ifndef CACHE_PRIV_H_INCLUDED
#define CACHE_PRIV_H_INCLUDED
#include <stdbool.h>

struct Cache {
  uint8_t data[8];
  uint8_t CLO;
  uint8_t dataInfo[8];
  bool isOn;
  uint8_t *memPtr;
  bool memDone;
  bool *memDonePtr;
  enum {IDLE2, MOVE2, STORE2} cacheState;
  int ticks;
  uint8_t *dataPtr;
  uint8_t requestAddress;
};

static void reset();
static void initCache();
static void on();
static void off();
static void flush();








#endif