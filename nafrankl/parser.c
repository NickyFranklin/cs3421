#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include "cpu.h"
#include "cpu_priv.h"
#include "memory.h"
#include "memory_priv.h"
#include "clock.h"
#include "clock_priv.h"
#include "priv_instruct_mem.h"
#include "instruct_mem.h"
#include "cache.h"
#include "cache_priv.h"
/*
Name: Nicky Franklin
Class: CS3421
Date last modified: 4-1-2023
Description: This program is a basic emulator of a clock, a cpu, and memory.
The program takes in values from a file and parses those values to execute certain commands.
Commands will then be sent to the corresponding device to do what ever functions that are to be parsed.
Device so far include:
CPU: setReg, dump, reset
Memory: set, dump, reset, create
Clock: tick, dump, reset
Instruction Memory:
Cahche:
 */
//Declares Global variables
struct CPU cpu;
struct Memory mem;
struct Clock clock;
struct InstMemory instMem;
struct Cache cache;

int main(int argc, char* argv[]) {
  //If a file name is not given, then it will not continue
  if(argc < 2) {
    printf("Please give a file name\n");
    return 0;
  }
  //Initializes variables
  clock = getClock();
  cpu = getCpu();
  FILE* infile;
  char* filename = argv[1];
  infile = fopen(filename, "r");
  if(NULL == infile) {
    printf("File could not be read\n");
    return 0;
  }
  char cmd[20];
  bool success = false;
  //Parses devices to use. Sends commands to other parsers based on input
  //If a parse command fails, it gets sent to the shadow realm (an error prints out)
  while(fscanf(infile, "%s", cmd) == 1) {
    success = false;
    //cpu
    if(strcmp(cmd, "cpu") == 0) {
      success = cpu_parse(infile);
      
      if(!success) {
	printf("cpu failure\n");
	return 0;
      }
    }

    //clock
    if(strcmp(cmd, "clock") == 0) {
      success = clock_parse(infile);
      if(!success) {
	printf("clock failure\n");
	return 0;
      }
    }

    if(strcmp(cmd, "imemory") == 0) {
      success = instMem_parse(infile);
      if(!success) {
	printf("instruction memory failure\n");
	return 0;
      }
    }

    //Cache
    if(strcmp(cmd, "cache") == 0) {
      success = cache_parse(infile);
      if(!success) {
	printf("cache failure\n");
	return 0;
      }
    }
    
    //memory
    if(strcmp(cmd, "memory") == 0) {
      success = mem_parse(infile);
      if(!success) {
	printf("memory failure\n");
	return 0;
      }
    }

    
  }
  return 0;
  //This does nothing
  free(mem.memIndex);
}
