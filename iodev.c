#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "iodev.h"
#include "iodev_priv.h"
#include <string.h>
#include "memory.h"
#include "cpu.h"
#include "clock.h"

extern struct Clock clock;
extern struct CPU cpu;
extern struct Memory mem;
extern struct InstMemory instMem;
extern struct Cache cache;
extern struct IoDev iodev;

static void initIo() {
  iodev.reg = 0;
  iodev.nextInstruction = 65000;
  iodev.ioState = INVALID5;
  iodev.address = 0;
  iodev.value = 55;
  iodev.tick = 0;
}

struct IoDev GetIo() {
  struct IoDev iodev;
  initIo();
  return iodev;
}

static void reset() {
  iodev.reg = 0;
  iodev.tick = 0;
}

void iodump() {
  printf("IO Device: 0x%02X \n\n", iodev.reg);
}

bool ioparse(FILE *infile) {
  char str[2000];

  if(fscanf(infile, "%s", str) == 1) {
    if(strcmp(str, "load") == 0) {
      if(fscanf(infile, "%s", str) == 1) {
	load(str);
	fileReadNext();
	return true;
      }
    }

    if(strcmp(str, "reset") == 0) {
      reset();
      return true;
    }
    
    if(strcmp(str, "dump") == 0) {
      iodump();
      return true;
    }
    
  }
  return false;
}

static void load(char* file) {
  iodev.file = fopen(file, "r");
}

static void fileReadNext() {
  char cmd[1000];
  if(fscanf(iodev.file, "%s", cmd) == 1) {
    iodev.nextInstruction = atoi(cmd);
    if(fscanf(iodev.file, "%s", cmd) == 1) {
      if(strcmp(cmd, "read") == 0) {
	iodev.ioState = IOREAD;
	uint8_t adr;
	if(fscanf(iodev.file, "%hhX", &adr) == 1) {
	  iodev.address = adr;
	}
      }
      else {
	iodev.ioState = IOWRITE;
	uint8_t adr1, adr2;
	if(fscanf(iodev.file, "%hhX %hhX", &adr1, &adr2) == 2) {
	  iodev.address = adr1;
	  iodev.value = adr2;
	}
      }

      
    }
  }
}

void IoDoCycleWork() {
  iodev.tick++;
  //printf("%d\n", iodev.tick);
  if(iodev.tick == iodev.nextInstruction) {
    //come back here later if cpu is executing commands while iodev should be instead
    
    switch(iodev.ioState) {
    case IOREAD:
      //printf("gets here \n");
      memStartFetch(iodev.address, 1, &(iodev.reg), &(iodev.boolin));
      fileReadNext();
      break;

    case IOWRITE:
      memStartStore(iodev.address, 1, &(iodev.value), &(iodev.boolin));
      fileReadNext();
      break;

    case INVALID5:
      break;
      
    }
  }
}
