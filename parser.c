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

struct CPU cpu;
struct Memory mem;
struct Clock clock;

int main(int argc, char* argv[]) {
  if(argc < 2) {
    printf("Please give a file name\n");
    return 0;
  }

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
  free(mem.memIndex);
}
