#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "memory_priv.h"
#include "memory.h"
#include <string.h>


void memStartFetch(unsigned int address, unsigned int count, uint8_t *dataPtr, bool *memDonePtr, struct Memory *mem) {
  if (1 == count) {
    *dataPtr = mem->memIndex[address];
  }
  else {
    memcpy(dataPtr, mem->Index[address], count);
  }
  *memDonePtr = true;
  
}

struct Memory getMem() {
  struct Memory mem;
  return mem;
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
void mem_dump(int hexAddress, int hexCount, struct Memory *mem) {
  printf("Addr 00 01 02 03 04 05 06 07 08 09 0A 0B 0C 0D 0E 0F\n");
  //& 15 takes only the part of hexAddress we want to subtract by
  //This is because 15 is 1111 and the AND of 1 is a number and the AND of 0 is 0
  printf("0x%02X ", (hexAddress - (hexAddress & 15)));
  int countingHexAddress = hexAddress;
  //The next loop adds spaces until it is in the correct place
  for(int i = 0; i < (hexAddress & 15); i++) {
    printf("   ");
  }
  
  for(int i = 0; i < hexCount; i++) {
    if((countingHexAddress & 15) == 0 && i != 0) {
      printf("\n0x%02X ", (countingHexAddress - (countingHexAddress & 15)));
    }
    printf("%02X ", mem->memIndex[countingHexAddress]);
    countingHexAddress++;
    
  }
  printf("\n\n");
}

//Will be handed stuff to do one byte at a time
//The parse function will be in charge of handling count and determining the address that
//will be changed, this function will simply change things one at a time
void set(int hexAddress, int hexByte, struct Memory *mem) {
  mem->memIndex[hexAddress] = hexByte;
}

bool mem_parse(FILE *infile, struct Memory *mem) {
  char str[20];

  if(fscanf(infile, "%s", str == 1)) {
    //create mem
    if(strcmp(str, "create") == 0) {
      if(fscanf(infile, "%s", str == 1)) {
	int memSize = (int) strtol(str, NULL, 16);
	create(memSize, mem);
	return true;
      }
    }
    
    //reset mem
    if(strcmp(str, "reset") == 0) {
      reset(mem);
      return true;
    }
    
    //set mem
    if(strcmp(str, "set") == 0) {
      char str3[20];
      if(fscanf(infile, "%s %s", str, str3)) {
	int setAddress = (int) strtol(str, NULL, 16);
	int setCount = (int) strtol(str3, NULL, 16);
	char str4[20];
	for(int i = 0; i < setCount; i++) {
	  if(fscanf(infile, "%s", str4) == 1) {
	    int setByte = (int) strtol(str4, NULL, 16);
	    set(setAddress, setByte, mem);
	  }
	}
	return true;
      }
    }

    //dump mem
    if(strcmp(str, "dump") == 0) {
      char str2[20];
      if(fscanf(infile, "%s %s", str, str2 == 2)) {
	int address = (int) strtol(str, NULL, 16);
	int count = (int) strtol(str2, NULL, 16);
	mem_dump(address, count, mem);
      }
      return true;
    }
    
  }

  return false;
}

int main() {
  struct Memory mem;
  create(0x10000, &mem);
  reset(&mem);
  set(0x34, 0xAB, &mem);
  set(0xE4, 0x42, &mem);
  mem_dump(0x32, 0xFF, &mem);
  free(mem.memIndex);
}
