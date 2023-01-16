#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "cpu.h"
#include "cpu_priv.h"
#include <string.h>


void init(CPU cpu) {
  for(int i = 0; i < 8; i++) {
    cpu.regs[i] = 0;
  }
  cpu.PC = 0;
  cpu.hasBeenInitialized = true;
  cpu.waitingOnMemory = false;
}

void reset(CPU cpu) {
  for(int i = 0; i < 8; i++) {
    cpu.regs[i] = 0;
  }
  cpu.PC = 0;
}

void setReg(char *reg, uint8_t hexByte, CPU cpu) {
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
    cpu.PC = 0;
  }
  
}

void cpu_dump(CPU cpu) {
  printf("PC: 0x%X\n", cpu.PC);
  printf("RA: 0x%X\n", cpu.regs[0]);
  printf("RB: 0x%X\n", cpu.regs[1]);
  printf("RC: 0x%X\n", cpu.regs[2]);
  printf("RD: 0x%X\n", cpu.regs[3]);
  printf("RE: 0x%X\n", cpu.regs[4]);
  printf("RF: 0x%X\n", cpu.regs[5]);
  printf("RG: 0x%X\n", cpu.regs[6]);
  printf("RH: 0x%X\n", cpu.regs[7]);
  printf("\n");
}

bool cpu_parse(File *infile, CPU cpu) {
  char str[40];
  if(scanf(infile, "%s", &str) == 1) {

    if(strcmp(str, "reset") == 0) {
      cpu.reset(cpu);
      return true;
    }
    
    if(strcmp(str, "dump")) {
      cpu.dump(cpu);
      return true;
    }
    
    if(strcmp(str, "set") == 0) {
      if(scanf(infile, "%s", &str) == 1) {
	if(strcmp(str, "reg") == 0) {
	  char *parseReg[2];
	  uint8_t hexByte;
	  if(scanf(infile, "%s %X", &parseReg, &hexByte) == 2) {
	    cpu.setReg(parseReg, hexByte, cpu);
            return true;
	  }
	}
      }
    }
  } 
  
}

void cpuDoCycleWork() {
  uint8_t fetchByte;
  bool fetchDone;
  memStartFetch(cpu.PC, 1, &fetchByte, &fetchDone);
}
