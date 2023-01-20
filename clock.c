#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "clock_priv.h"
#include "clock.h"
#include <string.h>
#include "cpu.h"

static void init() {
  clock.counter = 0;
}

struct Clock getClock() {
  struct Clock clock;
  init();
  return clock;
}

static void reset() {
  clock.counter = 0;
}

static void tick(int ticks, struct Clock *clock, struct CPU *cpu) {
  clock.counter += ticks;
  for(int i = 0; i < ticks; i++) {
    cpuDoCycleWork();
  }
}

bool clock_parse(FILE *infile, struct Clock *clock, struct CPU *cpu) {
  char str[40];
  if(fscanf(infile, "%s", str) == 1) {
    if(strcmp(str, "reset") == 0) {
      reset();
      return true;
    }

    if(strcmp(str, "tick") == 0) {
      int ticks;
      if(fscanf(infile, "%d", &ticks) == 1) {
	tick(ticks);
	return true;
      }
    }

    if(strcmp(str, "dump") == 0) {
      clock_dump();
      return true;
    }
  }
  return false;
}

void clock_dump(struct Clock *clock) {
  printf("Clock: %d\n\n", clock.counter);
}




