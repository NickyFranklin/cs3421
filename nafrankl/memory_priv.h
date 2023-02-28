#ifndef MEMORY_PRIV_H_INCLUDED
#define MEMORY_PRIV_H_INCLUDED


struct Memory {
  int size;
  uint8_t *memIndex;
  enum {IDLE, MOVE, STORE} state;
  int ticks;
  unsigned int requestAddress;
  unsigned int requestCount;
  uint8_t *dataPtr;
  bool *memDonePtr;
  bool memDone;
};

static void create(int hexBytes);
static void reset();
static void set(int hexAddress, int hexBytes);

#endif

