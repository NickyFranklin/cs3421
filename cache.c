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
extern struct IoDev iodev;

static void initCache() {
  cache.isOn = false;
  cache.CLO = 0;
  for(int i = 0; i < 8; i++) {
    cache.data[i] = 0;
    cache.dataInfo[i] = INVALID;
  }
  cache.cacheState = IDLE2;
  cache.ticks = 0;
}

static void on() {
  cache.isOn = true;
}

static void off() {
  cache.isOn = false;
  //This will cause a bug if you don't wait at least 5 ticks until you edit memory again
  memFlush(cache.CLO, &cache.data[0], &cache.dataInfo[0]);
}

static void reset() {
  cache.CLO = 0;
  for(int i = 0; i < 8; i++) {
    cache.dataInfo[i] = INVALID;
  }
  cache.isOn = false;
  cache.cacheState = IDLE2;
  cache.ticks = 0;
}

bool getCacheStatus() {
  return cache.isOn;
}

void cache_dump() {
  printf("CLO        : 0x%02X\n", cache.CLO);
  printf("cache data :");
  for(int i = 0; i < 8; i++) {
    printf(" 0x%02X", cache.data[i]);
  }
  printf("\nFlags      :");

  switch(cache.dataInfo[0]) {
  case VALID:
    printf("   V");
    break;
    
  case INVALID:
    printf("   I");
    break;
    
  case UPDATED:
    printf("   W");
    break;
  } 
      
  for(int i = 1; i < 8; i++) {
    switch(cache.dataInfo[i]) {
    case VALID:
      printf("    V");
      break;
      
    case INVALID:
      printf("    I");
      break;
    
    case UPDATED:
      printf("    W");
      break;
    } 
  }
  printf("\n\n");
}

bool cache_parse(FILE *infile) {
  char str[20];
  if(fscanf(infile, "%s", str) == 1) {
    if(strcmp(str, "reset") == 0) {
      reset();
      return true;
    }

    if(strcmp(str, "off") == 0) {
      off();
      return true;
    }

    if(strcmp(str, "on") == 0) {
      on();
      return true;
    }

    if(strcmp(str, "dump") == 0) {
      cache_dump();
      return true;
    }
    
  }
  return false;
}

struct Cache getCache() {
  struct Cache cache;
  initCache();
  return cache;
}

bool isFastCache(unsigned int address) {
  uint8_t calcCLO = address/8;
  uint8_t newAddress = address & 7;
  if(cache.CLO == calcCLO && cache.dataInfo[newAddress] != INVALID) {
    return true;
  }
  else {
    return false;
  }
}

void cacheStore(unsigned int address, unsigned int count, uint8_t *dataPtr, bool *memDonePtr) {
  uint8_t calcCLO = address/8;
  cache.requestAddress = address;
  //Easy Logic
  if(cache.CLO == calcCLO) {
    uint8_t newAddress = address & 7;
    memcpy(cache.data+newAddress, dataPtr, 1);
    *(memDonePtr) = true;
    cache.cacheState = IDLE2;
    cache.ticks = 0;
    cache.dataInfo[newAddress] = UPDATED;
  }
  
  //Hard logic :sob:
  else {
    if(calcCLO != cache.CLO) {
      bool updatedValues = false;
      for(int i = 0; i < 8; i++) {
	if(cache.dataInfo[i] == UPDATED) {
	  updatedValues= true;
	}
      }
      
      //If there are values that need to be flushed
      if(updatedValues == true) {
	//memFlush will take the CLO data and store the eight bytes in the appropriate areas
	memFlush(cache.CLO, &cache.data[0], &cache.dataInfo[0]);
	//Store state will be relevant in docyclework where it will take five ticks and then add the data to the cache
	
	cache.cacheState = STORE2;
	cache.memDonePtr = memDonePtr;
	cache.dataPtr = dataPtr;
	
      }
      
      //if there are not values that need to be flushed
      else {
	if(cache.requestAddress != 0xFF) {
	  for(int i = 0; i < 8; i++) {
	    cache.dataInfo[i] = INVALID;
	  }
	  uint8_t newAddress = cache.requestAddress & 7;
	  memcpy(cache.data+newAddress, cache.dataPtr, 1);
	  cache.dataInfo[newAddress] = UPDATED;
	  *memDonePtr = true;
	  cache.CLO = calcCLO;
	}
	
      }
      
    }
	
  }
  
}

void cacheFetch(unsigned int address, unsigned int count, uint8_t *dataPtr, bool *memDonePtr) {
  uint8_t calcCLO = address/8;
  cache.requestAddress = address;
  if(address == 0xFF) {
	for(int i = 0; i < 8; i++) {
		cache.dataInfo[i] = INVALID;
	}		
	*(memDonePtr) = true;
	cache.cacheState = IDLE2;
	cache.ticks = 0;
  }
  
  else if(isFastCache(address)) {
    //Gets offset in cache memory
    uint8_t newAddress = address & 7;
    memcpy(dataPtr, cache.data+newAddress, count);
    *(memDonePtr) = true;
	cache.cacheState = IDLE2;
	cache.ticks = 0;
  }

  //if the clo is right but the data is invalid
  else if(calcCLO == cache.CLO) {
	updateCache(cache.CLO, &cache.data[0], &cache.dataInfo[0]);
	cache.cacheState = MOVE2;
	cache.memDonePtr = memDonePtr;
	cache.dataPtr = dataPtr;
  }
  
  //if the clo is wrong
  else if(calcCLO != cache.CLO) {
	uint8_t oldCLO = cache.CLO;
	cache.CLO = calcCLO;
	cacheMove(cache.CLO, &cache.data[0], &cache.dataInfo[0], oldCLO);
	cache.cacheState = MOVE2;
	cache.memDonePtr = memDonePtr;
	cache.dataPtr = dataPtr;
  }
}

void cacheDoCycleWork() {
  //Store after a flush
  if(cache.cacheState == STORE2) {
    cache.ticks = cache.ticks + 1;
    if(cache.ticks == 5) {
      if(cache.requestAddress != 0xFF) {
	for(int i = 0; i < 8; i++) {
	  cache.dataInfo[i] = INVALID;
	}
	uint8_t newAddress = cache.requestAddress & 7;
	memcpy(cache.data+newAddress, cache.dataPtr, 1);
	cache.dataInfo[newAddress] = UPDATED;
	cache.cacheState = IDLE2;
	*cache.memDonePtr = true;
	cache.ticks = 0; 
	cache.CLO = cache.requestAddress / 8;
      }
      else {
	for(int i = 0; i < 8; i++) {
	  if(cache.dataInfo[i] == UPDATED) {
	    cache.dataInfo[i] = VALID;
	  }
	}
	cache.cacheState = IDLE2;
	*cache.memDonePtr = true;
	cache.ticks = 0;
      }
    }
  }
  
  if(cache.cacheState == MOVE2) {
    cache.ticks++;
    if(cache.ticks == 5) {
      uint8_t newAddress = cache.requestAddress & 7;
      memcpy(cache.dataPtr, cache.data+newAddress, 1);
      cache.cacheState = IDLE2;
      *cache.memDonePtr = true;
      cache.ticks = 0;
    }
  }
  
  
}
