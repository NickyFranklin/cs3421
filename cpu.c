#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "cpu_priv.h"
#include "cpu.h"
#include <string.h>
#include "memory.h"
#include "instruct_mem.h"
#include "cache.h"

//Declares the global variables in the file
extern struct Clock clock;
extern struct CPU cpu;
extern struct Memory mem;
extern struct InstMemory instMem;
extern struct Cache cache;
extern struct IoDev iodev;

//initializes the cpu and everything to 0
static void initCpu() {
  for(int i = 0; i < 8; i++) {
    cpu.regs[i] = 0;
  }
  cpu.PC = 0;
  cpu.hasBeenInitialized = true;
  
  cpu.state = FETCH;
  cpu.memDone = false;
  cpu.memDonePtr = &cpu.memDone;
  cpu.ticks = 0;
  cpu.moreWork = false;
  cpu.TC = 0;
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
  cpu.memDone = false;
  cpu.ticks = 0;
  cpu.moreWork = false;
  cpu.TC = 0;
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
  printf("TC: %d\n", cpu.TC);
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
  if(cpu.state != HALT) {
    cpu.TC++;
  }
  if(cpu.state == WAIT) {
    if(cpu.memDone == true) {
      if(cpu.state != HALT) {
	cpu.state = FETCH;
      }
      //use cpu command to perform operation
      cpu.memDone = false;
    }
  }
  
  //If the cpu hasn't waited at all, then it will fetch a new instruction
  
  if(cpu.state == FETCH || cpu.state == WORK) {
    cpu.command = imemFetch(cpu.PC);
    //Getting instruction bits and ANDing by binary 111 just
    //incase the 32bit value has something it shouldn't
    int instruction = ((cpu.command >> 17) & 7);
    
    if(instruction == ADD) {
      int destReg = ((cpu.command >> 14) & 7);
      int sourceReg = ((cpu.command >> 11) & 7);
      int targetReg = ((cpu.command >> 8) & 7);
      uint8_t twosComplimentSource = cpu.regs[sourceReg];
      uint8_t twosComplimentTarget = cpu.regs[targetReg];
      if((cpu.regs[sourceReg] & 128) == 1) {
	twosComplimentSource = (~twosComplimentSource) + 1;
      }
      
      if((cpu.regs[targetReg] & 128) == 1) {
	twosComplimentTarget = (~twosComplimentTarget) + 1;
      }
      cpu.regs[destReg] = twosComplimentSource + twosComplimentTarget;
      cpu.moreWork = true;
      cpu.PC++;
    }
    
    else if(instruction == ADDI) {
      int destReg = ((cpu.command >> 14) & 7);
      int sourceReg = ((cpu.command >> 11) & 7);
      int immediate = ((cpu.command) & 255);
      uint8_t twosComplimentSource = cpu.regs[sourceReg];
      uint8_t twosComplimentImmediate = immediate;
      if((cpu.regs[sourceReg] & 128) == 1) {
	twosComplimentSource = (~twosComplimentSource) + 1;
      }
      
      if((immediate & 128) == 1) {
	twosComplimentImmediate = (~twosComplimentImmediate) + 1;
      }
      cpu.regs[destReg] = twosComplimentSource + twosComplimentImmediate;
      cpu.moreWork = true;
      cpu.PC++;
    }

    else if(instruction == MUL) {
      cpu.ticks++;
      if(cpu.ticks == 2) {
	int destReg = ((cpu.command >> 14) & 7);
	int sourceReg = ((cpu.command >> 11) & 7);
	int firstHalf = cpu.regs[sourceReg] & 15;
	int secondHalf = (cpu.regs[sourceReg] >> 4) & 15;
	cpu.regs[destReg] = firstHalf * secondHalf;
	//cpu.memDone = true;
	cpu.PC++;
	cpu.ticks = 0;
      }
      cpu.moreWork = true;
      /*
      cpu.state = WORK;
      if(cpu.memDone == true) {
	cpu.PC++;
	cpu.state = FETCH;
      }
      cpu.memDone = false;
      */
    }

    else if(instruction == INV) {
      int destReg = ((cpu.command >> 14) & 7);
      int sourceReg = ((cpu.command >> 11) & 7);
      cpu.regs[destReg] = ~cpu.regs[sourceReg];
      cpu.PC++;
      cpu.moreWork = true;
    }

    else if(instruction == B) {
      int branchInst = ((cpu.command >> 14) & 7);
      int sourceReg = ((cpu.command >> 11) & 7);
      int targetReg = ((cpu.command >> 8) & 7);
      int immediate = ((cpu.command) & 255);
      cpu.ticks++;
      if(branchInst == BEQ) {
	if(cpu.regs[sourceReg] != cpu.regs[targetReg]) {
	  cpu.PC++;
	  cpu.ticks = 0;
	  cpu.moreWork = true;
	}

	if(cpu.regs[sourceReg] == cpu.regs[targetReg] && cpu.ticks == 2) {
	  cpu.PC = immediate;
	  cpu.ticks = 0;
	  cpu.moreWork = true;
	}
      }

      else if(branchInst == BNEQ) {
	if(cpu.regs[sourceReg] == cpu.regs[targetReg]) {
	  cpu.PC++;
	  cpu.ticks = 0;
	  cpu.moreWork = true;
	}
	
	if(cpu.regs[sourceReg] != cpu.regs[targetReg] && cpu.ticks == 2) {
	  cpu.PC = immediate;
	  cpu.ticks = 0;
	  cpu.moreWork = true;
	}

      }

      else if(branchInst == BLT) {
	if(cpu.regs[sourceReg] >= cpu.regs[targetReg]) {
	  cpu.PC++;
	  cpu.ticks = 0;
	  cpu.moreWork = true;
	}
	
	if(cpu.regs[sourceReg] < cpu.regs[targetReg] && cpu.ticks == 2) {
	  cpu.PC = immediate;
	  cpu.ticks = 0;
	  cpu.moreWork = true;
	}

      }

      
    }

    else if(instruction == SW) {
      int targetReg = ((cpu.command >> 8) & 7);
      int sourceReg = ((cpu.command >> 11) & 7);
      //figure out what goes in cpu.regs[] later
      if(getCacheStatus()) {
		cacheStore(cpu.regs[targetReg], 1, &cpu.regs[sourceReg], &cpu.memDone);
      }
      
      else {
		memStartStore(cpu.regs[targetReg], 1, &cpu.regs[sourceReg], &cpu.memDone);
      }
      cpu.state = WAIT;
      cpu.PC++;
      cpu.moreWork = false;
    }

    else if(instruction == LW) {
      int targetReg = ((cpu.command >> 8) & 7);
      int destinationReg = ((cpu.command >> 14) & 7);  
      if(getCacheStatus()) {
		cacheFetch(cpu.regs[targetReg], 1, &cpu.regs[destinationReg], &cpu.memDone);
      }
      
      else {
		memStartFetch(cpu.regs[targetReg], 1, &cpu.regs[destinationReg], &cpu.memDone);
      }
      cpu.state = WAIT;
      cpu.PC++;
      cpu.moreWork = false;
    }

    else if(instruction == HALT) {
      cpu.state = HALTED;
      cpu.PC++;
      cpu.moreWork = false;
    }
    


    
  }
  
}

//Isn't really needed
void cpu_start_tick() {
  
}

uint16_t getCPUTick() {
  return cpu.TC;
}

bool cpuIsMoreCycleWork() {
  return cpu.moreWork;
}
