#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include "priv_instruct_mem.h"
#include "instruct_mem.h"
#include <string.h>

//Declares Globals
extern struct Clock clock;
extern struct CPU cpu;
extern struct Memory mem;
extern struct InstMemory instMem;

struct InstMemory getInstMem() {
  struct InstMemory instMem;
  return instMem;
}

uint32_t imemFetch(uint8_t address) {
  return instMem.instMemIndex[address];
}

static void create(int hexWords) {
  instMem.size = hexWords;
  instMem.instMemIndex = malloc(sizeof(uint32_t)*hexWords);
}

static void reset() {
  for(int i = 0; i < instMem.size; i++) {
    instMem.instMemIndex[i] = 0;
  }
}

void instMem_dump(int hexAddress, int hexCount) {
  printf("Addr     0     1     2     3     4     5     6     7\n");

  //The binary for 7 is 0111, when AND'ed with the hexAddress, it will give us the increment
  //of which we would like to increment the address on the left side of the dump
  printf("0x%02X ", (hexAddress - (hexAddress & 7)));

  int countingHexAddress = hexAddress;
  for(int i = 0; i < (hexAddress & 7); i++) {
    printf("      ");
  }

  //Prints out the data
  for(int i = 0; i < hexCount; i++) {
    if((countingHexAddress & 7) == 0 && i != 0) {
      printf("\n0x%02x ", (countingHexAddress - (countingHexAddress & 7)));
    }
    printf("%05X ", instMem.instMemIndex[countingHexAddress]);
    countingHexAddress++;
    
  }
  printf("\n\n");
}

static void setI(long hexAddress, char file[]) {
  FILE* infile;
  infile = fopen(file, "r");
  char cmd[20];
  if(NULL == infile) {
    printf("File count not be read\n");
  }
  while(fscanf(infile, "%s", cmd) == 1) {
    instMem.instMemIndex[hexAddress] = strtol(cmd, NULL, 16);
    hexAddress++;
  }
  fclose(infile);
  
}

bool instMem_parse(FILE *infile) {
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
    if(strcmp(str,"reset") == 0) {
      reset();
      return true;
    }
    
    //set
    if(strcmp(str, "set") == 0) {
      char str2[30];
      char str3[4000];
      if(fscanf(infile, "%s", str2) == 1) {
	long hexAddress = strtol(str2, NULL, 16);
	if(fscanf(infile,"%s", str2) == 1) {
	  if(strcmp(str2, "file") == 0) {
	    if(fscanf(infile, "%s", str3) == 1) {
	      setI(hexAddress, str3);
	      return true;
	    }
	  }
	}
	
      }
      
    }

    if(strcmp(str, "dump") == 0) {
      char str4[20];
      if(fscanf(infile, "%s %s", str, str4) == 2) {
	int address = (int) strtol(str, NULL, 16);
	int count = (int) strtol(str4, NULL, 16);
	instMem_dump(address, count);
	return true;
      }
    }

  }

  return false;
  
}

