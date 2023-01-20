#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "cpu_priv.h"
#include "cpu.h"
#include <string.h>
#include "memory.h"

extern struct Clock clock;
extern struct CPU cpu;
extern struct Memory mem;

static void initCpu() {
  for(int i = 0; i < 8; i++) {
    cpu.regs[i] = 0;
  }
  cpu.PC = 0;
  cpu.hasBeenInitialized = true;
  cpu.waitingOnMemory = false;
}

struct CPU getCpu() {
  struct CPU cpu;
  initCpu();
  return cpu;
}


static void reset() {
  for(int i = 0; i < 8; i++) {
    cpu.regs[i] = 0;
  }
  cpu.PC = 0;
  
}

static void setReg(char *reg, uint8_t hexByte) {
  if(!strcmp(reg, "RA")) {
    cpu.regs[0] = hexByte;
  }
  
  else if(!strcmp(reg, "RB")) {
    cpu.regs[1] = hexByte;
  }

  else if(!strcmp(reg, "RC")) {
    cpu.regs[2] = hexByte; 
  }

  else if(!strcmp(reg, "RD")) {
    cpu.regs[3] = hexByte;
  }

  else if(!strcmp(reg, "RE")) {
    cpu.regs[4] = hexByte;
  }

  else if(!strcmp(reg, "RF")) {
    cpu.regs[5] = hexByte;
  }

  else if(!strcmp(reg, "RG")) {
    cpu.regs[6] = hexByte;
  }

  else if(!strcmp(reg, "RH")) {
    cpu.regs[7] = hexByte;
  }

  else if(!strcmp(reg, "PC")) {
    cpu.PC = hexByte;
  }
  
}

void cpu_dump() {
  printf("PC: 0x%02X\n", cpu.PC);
  printf("RA: 0x%02X\n", cpu.regs[0]);
  printf("RB: 0x%02X\n", cpu.regs[1]);
  printf("RC: 0x%02X\n", cpu.regs[2]);
  printf("RD: 0x%02X\n", cpu.regs[3]);
  printf("RE: 0x%02X\n", cpu.regs[4]);
  printf("RF: 0x%02X\n", cpu.regs[5]);
  printf("RG: 0x%02X\n", cpu.regs[6]);
  printf("RH: 0x%02X\n", cpu.regs[7]);
  printf("\n");
}

bool cpu_parse(FILE *infile) {
  char str[20];
  if(fscanf(infile, "%s", str) == 1) {

    if(strcmp(str, "reset") == 0) {
      reset();
      return true;
    }
    
    if(strcmp(str, "dump") == 0) {
      cpu_dump();
      return true;
    }
    
    if(strcmp(str, "set") == 0) {
      if(fscanf(infile, "%s", str) == 1) {
	if(strcmp(str, "reg") == 0) {
	  char parseReg[3];
	  uint8_t hexByte;
	  if(fscanf(infile, "%s %hhX", parseReg, &hexByte) == 2) {
	    setReg(parseReg, hexByte);
            return true;
	  }
	}
      }
    }
  } 
  return false;  
}

void cpuDoCycleWork() {
  uint8_t fetchByte;
  bool fetchDone;
  memStartFetch(cpu.PC, 1, &fetchByte, &fetchDone);
  for(int i = 7; i > 0; i--) {
    cpu.regs[i] = cpu.regs[i-1];
  }
  cpu.regs[0] = fetchByte;
  cpu.PC++;
}
