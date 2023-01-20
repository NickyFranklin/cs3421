#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "cpu_priv.h"
#include "cpu.h"
#include <string.h>
#include "memory.h"

//Declares the global variables in the file
extern struct Clock clock;
extern struct CPU cpu;
extern struct Memory mem;

//initializes the cpu and everything to 0
static void initCpu() {
  for(int i = 0; i < 8; i++) {
    cpu.regs[i] = 0;
  }
  cpu.PC = 0;
  cpu.hasBeenInitialized = true;
  cpu.waitingOnMemory = false;
}

//makes a cpu for the parser
struct CPU getCpu() {
  struct CPU cpu;
  initCpu();
  return cpu;
}

//Resets all values to 0 on the cpu
static void reset() {
  for(int i = 0; i < 8; i++) {
    cpu.regs[i] = 0;
  }
  cpu.PC = 0;
  
}

//Checks which register is being set and then updates it with the correct byte of data
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

//Prints out the cpu information
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

//Looks through the file and executes commands based on what is typed, returns true if a command is executed successfully
bool cpu_parse(FILE *infile) {
  char str[20];
  //reads word and determines command to execute
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

//On clock cycles, it shifts all registers down and then puts data from memory into RA
void cpuDoCycleWork() {
  uint8_t fetchByte;
  bool fetchDone;
  for(int i = 7; i > 0; i--) {
    cpu.regs[i] = cpu.regs[i-1];
  }
  memStartFetch(cpu.PC, 1, &fetchByte, &fetchDone);
  cpu.regs[0] = fetchByte;
  cpu.PC++;
}
