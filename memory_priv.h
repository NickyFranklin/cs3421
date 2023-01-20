#ifndef MEMORY_PRIV_H_INCLUDED
#define MEMORY_PRIV_H_INCLUDED


struct Memory {
  int size;
  uint8_t *memIndex;
};

static void create(int hexBytes);
static void reset();
static void set(int hexAddress, int hexBytes);

#endif

