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
  //When writing to memory, pass the validity pointers 
}

static void reset() {

}

void cache_dump() {

}

bool cache_parse(FILE *infile) {

}

struct Cache getCache() {

}

bool isFastCache(unsigned int address) {
  uint8_t calcCLO = address/8;
  return calcCLO == cache.CLO
}

void cacheStore(unsigned int address, unsigned int count, uint8_t *dataPtr, bool *memDonePtr) {
  
}

void cacheFetch(unsigned int address, unsigned int count, uint8_t *dataPtr, bool *memDonePtr) {
  
}
