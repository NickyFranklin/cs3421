#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "cpu_priv.h"
#include "cpu.h"
#include <string.h>

void init(struct CPU cpu) {
  for(int i = 0; i < 8; i++) {
    cpu.regs[i] = 0;
  }
  cpu.PC = 0;
  cpu.hasBeenInitialized = true;
  cpu.waitingOnMemory = false;
}

void reset(struct CPU cpu) {
  for(int i = 0; i < 8; i++) {
    cpu.regs[i] = 0;
  }
  cpu.PC = 0;
}

void setReg(char *reg, uint8_t hexByte, struct CPU cpu) {
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

void cpu_dump(struct CPU cpu) {
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

bool cpu_parse(FILE *infile, struct CPU cpu) {
  char str[20];
  if(fscanf(infile, "%s", str) == 1) {

    if(strcmp(str, "reset") == 0) {
      reset(cpu);
      return true;
    }
    
    if(strcmp(str, "dump")) {
      cpu_dump(cpu);
      return true;
    }
    
    if(strcmp(str, "set") == 0) {
      if(fscanf(infile, "%s", str) == 1) {
	if(strcmp(str, "reg") == 0) {
	  char parseReg[3];
	  uint8_t hexByte;
	  if(fscanf(infile, "%s %hhX", parseReg, &hexByte) == 2) {
	    setReg(parseReg, hexByte, cpu);
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
  // memStartFetch(cpu.PC, 1, &fetchByte, &fetchDone);
}

int main(int argc, char* argv[]) {
  struct CPU cpu;
  init(cpu);
  cpu_dump(cpu);
  return 0;
}
