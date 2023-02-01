#ifndef PRIV_INSTRUCT_MEM_H_INCLUDED
#define PRIV_INSTRUCT_MEM_H_INCLUDED


struct InstMemory {
  int size;
  uint32_t *memIndex;
};

static void create(int hexBytes);
static void reset();
static void set(int hexAddress, char[4000] file);

#endif