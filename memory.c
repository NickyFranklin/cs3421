#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "memory_priv.h"
#include "memory.h"
#include <string.h>
#include "cache.h"
#include "iodev.h"
#include "cpu.h"
//Declares globals from outside the file
extern struct Clock clock;
extern struct CPU cpu;
extern struct Memory mem;
extern struct InstMemory instMem;
extern struct Cache cache;
extern struct IoDev iodev;

//Fetches data from memory for the cpu
void memStartFetch(unsigned int address, unsigned int count, uint8_t *dataPtr, bool *memDonePtr) {
  /*
  if (1 == count) {
    *dataPtr = mem.memIndex[address];
  }
  else {
    memcpy(dataPtr, mem.memIndex+address, count);
  }
  *memDonePtr = true;
  */
  mem.state = MOVE;
  mem.dataPtr = dataPtr;
  mem.requestCount = count;
  mem.requestAddress = address;
  mem.memDonePtr = memDonePtr;
  //printf("mem start fetch %d\n", getCPUTick());
}

void memStartStore(unsigned int address, unsigned int count, uint8_t *dataPtr, bool *memDonePtr) {
  mem.state = STORE;
  mem.dataPtr = dataPtr;
  mem.requestCount = count;
  mem.requestAddress = address;
  mem.memDonePtr = memDonePtr;
  //printf("mem start store %d\n", getCPUTick());
}

void updateCache(uint8_t CLO, uint8_t *dataPtr, uint8_t *validPtr) {
	mem.requestAddress = CLO * 8;
	mem.dataPtr = dataPtr;
	mem.validPtr = validPtr;
	mem.state = UPDATECACHE;
}

void memFlush(uint8_t CLO, uint8_t *dataPtr, uint8_t *validPtr) {
	mem.requestAddress = CLO * 8;
	mem.dataPtr = dataPtr;
	mem.validPtr = validPtr;
	mem.state = FLUSH;
}

void cacheMove(uint8_t CLO, uint8_t *dataPtr, uint8_t *validPtr, uint8_t oldCLO) {
	mem.requestAddress = CLO * 8;
	mem.dataPtr = dataPtr;
	mem.validPtr = validPtr;
	mem.state = CACHEMOVE;
	mem.requestAddress2 = oldCLO * 8;
}

void memDoCycleWork() {
  //printf("gets to mem cycle work %d\n", getCPUTick());
  if(mem.state == IDLE) {
    //do nothing
  }
  
  else if(mem.state == CACHEMOVE) {
	mem.ticks = mem.ticks + 1;
	if(mem.ticks == 5) {
		uint8_t newAddress = mem.requestAddress2;
		//Writes stuff to memory before reading new stuff from memory
		for(int i = 0; i < 8; i++) {
			if(*(mem.validPtr+i) != INVALID) {
				newAddress = mem.requestAddress2 + i;
				memcpy(mem.memIndex+newAddress, mem.dataPtr+i, 1);
			}
		}
		
		newAddress = mem.requestAddress;
		//Writes stuff to memory before reading new stuff from memory
		for(int i = 0; i < 8; i++) {
			newAddress = mem.requestAddress + i;
			memcpy(mem.dataPtr+i, mem.memIndex+newAddress, 1);
			*(mem.validPtr+i) = VALID;
		}
		
		mem.state = IDLE;
        mem.ticks = 0;
	}
  }
  
  
  else if(mem.state == UPDATECACHE) {
	mem.ticks = mem.ticks + 1;
	if(mem.ticks == 5) {
		uint8_t newAddress = mem.requestAddress;
		for(int i = 0; i < 8; i++) {
			if(*(mem.validPtr+i) != UPDATED) {
				newAddress = mem.requestAddress + i;
				memcpy(mem.dataPtr+i, mem.memIndex+newAddress, 1);
				*(mem.validPtr+i) = VALID;
			}
		}
		mem.state = IDLE;
        mem.ticks = 0;
	}
  }
  
  else if(mem.state == FLUSH) {
    mem.ticks = mem.ticks + 1;
    if(mem.ticks == 5) {
      uint8_t newAddress = mem.requestAddress;
      for(int i = 0; i < 8; i++) {
	if(*(mem.validPtr+i) == UPDATED) {
	  newAddress = mem.requestAddress + i;
	  memcpy(mem.memIndex+newAddress, mem.dataPtr+i, 1);
	}
      }
      mem.state = IDLE;
      mem.ticks = 0;
    }
  }
  
  else if(mem.state == MOVE) {
    mem.ticks = mem.ticks + 1;
    if(mem.ticks == 5) {
      //if(mem.requestCount == 1) {
		memcpy(mem.dataPtr, mem.memIndex+mem.requestAddress, mem.requestCount);
		mem.state = IDLE;
		*(mem.memDonePtr) = true;
		mem.ticks = 0;
	  //}
	  /*
	  else {
		uint8_t newAddress = mem.requestAddress & 7;
		
	  }
	  */
    }
  }

  else if(mem.state == STORE) {
    mem.ticks = mem.ticks + 1;
    if(mem.ticks == 5) {
      //for(int i = 1; i <= mem.requestCount) {
        //if(*(mem.validPtr+(i-1)) == UPDATED) { 
	  //This takes the memory address of the validPtr array and adds one to it each loop to get the next memory address for the next answer
	  memcpy(mem.memIndex+mem.requestAddress, mem.dataPtr, 1);
	//}      
      //}
      mem.state = IDLE;
      *mem.memDonePtr = true;
      mem.ticks = 0;
    }
  }
  
}


void memStartTick() {

}

bool memIsMoreCycleWorkNeeded() {
  
}


//Pretty sure that this is a defunct function from when I started
struct Memory getMem() {
  struct Memory mem;
  return mem;
}

//Allocates space in memory 
static void create(int hexBytes) {
  mem.size = hexBytes;
  mem.memIndex = malloc(sizeof(uint8_t)*hexBytes);
}

//Sets all memory values to 0
static void reset() {
  for(int i = 0; i < mem.size; i++) {
    mem.memIndex[i] = 0;
  }
  mem.state = IDLE;
  mem.ticks = 0;
}

//Prints out sections of memory
void mem_dump(int hexAddress, int hexCount) {
  printf("Addr 00 01 02 03 04 05 06 07 08 09 0A 0B 0C 0D 0E 0F\n");
  //& 15 takes only the part of hexAddress we want to subtract by
  //This is because 15 is 1111 and the AND of 1 is a number and the AND of 0 is 0
  printf("0x%02X ", (hexAddress - (hexAddress & 15)));
  int countingHexAddress = hexAddress;
  //The next loop adds spaces until it is in the correct place
  for(int i = 0; i < (hexAddress & 15); i++) {
    printf("   ");
  }

  //Prints out memory data according to index
  for(int i = 0; i < hexCount; i++) {
    if((countingHexAddress & 15) == 0 && i != 0) {
      printf("\n0x%02X ", (countingHexAddress - (countingHexAddress & 15)));
    }
    printf("%02X ", mem.memIndex[countingHexAddress]);
    countingHexAddress++;
    
  }
  printf("\n\n");
}

//Will be handed stuff to do one byte at a time
//The parse function will be in charge of handling count and determining the address that
//will be changed, this function will simply change things one at a time
static void set(int hexAddress, int hexByte) {
  mem.memIndex[hexAddress] = hexByte;
}

//Parses and sends commands to do certain functions
bool mem_parse(FILE *infile) {
  char str[20];

  if(fscanf(infile, "%s", str) == 1) {
    //create mem
    if(strcmp(str, "create") == 0) {
      if(fscanf(infile, "%s", str) == 1) {
	int memSize = (int) strtol(str, NULL, 16);
	create(memSize);
	return true;
      }
    }
    
    //reset mem
    if(strcmp(str, "reset") == 0) {
      reset();
      return true;
    }
    
    //set mem
    if(strcmp(str, "set") == 0) {
      char str3[20];
      if(fscanf(infile, "%s %s", str, str3) == 2) {
	int setAddress = (int) strtol(str, NULL, 16);
	int setCount = (int) strtol(str3, NULL, 16);
	char str4[20];
	for(int i = 0; i < setCount; i++) {
	  if(fscanf(infile, "%s", str4) == 1) {
            int setByte = (int) strtol(str4, NULL, 16);
	    set(setAddress+i, setByte);
	  }
	}
	return true;
      }
    }

    //dump mem
    if(strcmp(str, "dump") == 0) {
      
      char str2[20];
      if(fscanf(infile, "%s %s", str, str2) == 2) {
        int address = (int) strtol(str, NULL, 16);
	int count = (int) strtol(str2, NULL, 16);
	mem_dump(address, count);
        return true;
      }
      
    }
    
  }

  return false;
}

