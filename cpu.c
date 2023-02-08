#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "cpu_priv.h"
#include "cpu.h"
#include <string.h>
#include "memory.h"
#include "instruct_mem.h"

//Declares the global variables in the file
extern struct Clock clock;
extern struct CPU cpu;
extern struct Memory mem;
extern struct InstMemory instMem;

//initializes the cpu and everything to 0
static void initCpu() {
  for(int i = 0; i < 8; i++) {
    cpu.regs[i] = 0;
  }
  cpu.PC = 0;
  cpu.hasBeenInitialized = true;
  cpu.state = FETCH;
  *cpu.memPtr = false;
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
  cpu.state = FETCH;
  *cpu.memPtr = false; 
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
    cpu.state = FETCH;
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

void cpuDoCycleWork() {
  if(cpu.state == WAIT) {
    if(*cpu.memDonePtr == true) {
      cpu.state = FETCH;
      //use cpu command to perform operation
      *cpu.memDonePtr = false;
    }
  }
  
  //If the cpu hasn't waited at all, then it will fetch a new instruction
  else if(cpu.state == FETCH) {
    cpu.command = imemFetch(cpu.PC);
    cpu.PC++;
    //Getting instruction bits and ANDing by binary 111 just
    //incase the 32bit value has something it shouldn't
    int instruction = ((cpu.command >> 17) & 7);
    if(instruction == SW) {
      int targetReg = ((cpu.command >> 8) & 7);
      int sourceReg = ((cpu.command >> 11) & 7);
      //figure out what goes in cpu.regs[] later
      memStartStore(cpu.regs[targetReg], 1, &cpu.regs[sourceReg], cpu.memDonePtr);
      cpu.state = WAIT;
    }

    else if(instruction == LW) {
      int targetReg = ((cpu.command >> 8) & 7);
      int destinationReg = ((cpu.command >> 14) & 7);
      memStartFetch(cpu.regs[targetReg], 1, &cpu.regs[destinationReg], cpu.memDonePtr);
      cpu.state = WAIT;
    }
  }
  
}

//Isn't really needed
void cpu_start_tick() {
  
}


bool cpuIsMoreCycleWork() {

}
