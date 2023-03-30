#ifndef CACHE_PRIV_H_INCLUDED
#define CACHE_PRIV_H_INCLUDED
#include <stdbool.h>
enum {VALID, INVALID, UPDATED} dataStatus;

struct Cache {
  uint8_t data[8];
  uint8_t CLO;
  uint8_t dataInfo[8];
  bool isOn;
}

static void reset();
static void initCache();
static void on();
static void off();
static void flush();








#endif