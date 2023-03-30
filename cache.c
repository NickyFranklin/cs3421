#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "cache_priv.h"
#include "cache.h"
#include <string.h>
#include "memory.h"
#include "cpu.h"

extern struct Clock clock;
extern struct CPU cpu;
extern struct Memory mem;
extern struct InstMemory instMem;
extern struct Cache cache;

static void initCache() {
  cache.isOn = false;
  cache.CLO = 0;
  for(int i = 0; i < 8; i++) {
    cache.data[i] = 0;
    cache.dataInfo[i] = INVALID;
  }
}

static void on() {
  cache.isOn = true;
}

static void off() {
  cache.isOn = false;
  cache.flush();
}

static void flush() {
  //When doing a read, set cpu data pointer to the spot in the cache where it should be
  //When the cache updates, the cpu data should change automatically with it
  //When writing to memory, 
}

void cacheDoCycleWork() {
  
}
