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
  cache.state = IDLE;
  cache.ticks = 0;
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
  //When writing to memory, pass the validity pointers, change memory function based on count 
}

static void reset() {
  cache.CLO = 0;
  for(int i = 0; i < 8; i++) {
    cache.dataInfo[i] = INVALID
  }
  cache.isOn = false;
  cache.state = IDLE;
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
  if(cache.CLO == calcCLO && cache.dataInfo[newAddress] != invalid) {
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
	cache.state = IDLE;
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
			
			cache.state = STORE;
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
  if(isFastCache(address)) {
    //Gets offset in cache memory
    uint8_t newAddress = address & 7;
    memcpy(dataPtr, cache.data+newAddress, count);
    *(memDonePtr) = true;
	cache.state = IDLE;
	cache.ticks = 0;
  }

  //if the clo is right but the data is invalid
  else if(calcCLO == cache.CLO) {
	updateCache(cache.CLO, &cache.data[0], &cache.dataInfo[0]);
	cache.state = MOVE;
	cache.memDonePtr = memDonePtr;
	cache.dataPtr = dataPtr;
  }
  
  //if the clo is wrong
  else if(calcCLO != cache.CLO) {
	  
  }
}

void cacheDoCycleWork() {
	//Store after a flush
	if(cache.state == STORE) {
		cache.ticks = cache.ticks + 1;
		if(cache.ticks == 5) {
			if(cache.requestAddress != 0xFF) {
				for(int i = 0; i < 8; i++) {
					cache.dataInfo[i] = INVALID;
				}
				uint8_t newAddress = cache.requestAddress & 7;
				memcpy(cache.data+newAddress, cache.dataPtr, 1);
				cache.dataInfo[newAddress] = UPDATED;
				cache.state = IDLE;
				*cache.memDonePtr = true;
				cache.ticks = 0;
				cache.CLO = cache.requestAddress / 8;
			}
			
			else {
				for(int i = 0; i < 8; i++) {
					cache.dataInfo[i] = VALID;
				}
				cache.state = IDLE;
				*cache.memDonePtr = true;
				cache.ticks = 0;
			}
		}
    }
	
	if(cache.state == MOVE) {
		cache.ticks++;
		if(cache.ticks == 5) {
			memcpy(cache.dataPtr, cache.data+newAddress, 1);
			cache.state = IDLE;
			*cache.memDonePtr = true;
			cache.ticks = 0;
		}
	}
	
	
  }
}