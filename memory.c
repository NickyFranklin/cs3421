#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "memory_priv.h"
#include "memory.h"
#include <string.h>

void memStartFetch(unsigned int address, unsigned int count, uint8_t *dataPtr, boll *memDonePtr) {
  if (1 == count) {
    *dataPtr = memPtr[address];
  }
  else {
    memcpy(dataPtr, memPtr+address, count);
  }
  *memDonePtr = true;
  
}

void create(int hexBytes, struct Memory *mem) {
  mem->size = hexBytes;
  mem->memIndex = malloc(sizeof(uint8_t)*hexBytes);
}

void reset(struct Memory *mem) {
  for(int i = 0; i < mem->size; i++) {
    mem->memIndex[i] = 0;
  }
}

//Parse function will pass char to this function
void mem_dump(char hexAddress, char hexCount, struct Memory *mem) {
  printf("Addr 00 01 02 03 04 05 06 07 08 09 0A 0B 0C 0D 0E 0F\n");
  printf("0x%02X ", (hexAddress - (hexAddress % 16)));
  int countingHexAddress = hexAddress;
  //The next loop adds spaces until it is in the correct place
  for(int i = 0; i < (hexAddress % 16); i++) {
    printf("   ");
  }
  
  for(int i = 0; i < hexCount, i++) {
    if(countingHexAddress % 16 == 0) {
      printf("\n0x%02X ", (countingHexAddress - (countingHexAddress % 16)));
    }
    printf("%02X ", mem->memIndex[countingHexAddress]);
    countingHexAddress++;
    
  }
  printf("\n\n");
}

//Will be handed stuff to do one byte at a time
//The parse function will be in charge of handling count and determining the address that
//will be changed, this function will simply change things one at a time
void set(char hexAddress, char hexByte, struct Memory *mem) {
  
}
