#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "clock.h"
#include "clock_priv.h"
#include <string.h>

void init(Clock clock) {
  clock.counter = 0;
}

void reset(Clock clock) {
  clock.counter = 0;
}

void tick(int ticks, Clock clock) {
  clock.counter += ticks;
}

bool clock_parse(File *infile, Clock clock) {
  char str[40];
  if(fscanf(infile, "%s", &str) == 1) {
    if(strcmp(str, "reset") == 0) {
      clock.reset(Clock clock);
      return true;
    }

    if(strcmp(str, "tick") == 0) {
      int ticks;
      if(fscanf(infile, "%d", &ticks) == 1) {
	clock.tick(ticks, Clock clock);
	return true;
      }
      return false;
    }

    if(strcmp(str, "dump") == 0) {
      clock.dump(Clock clock);
      return true;
    }
  }
  return false;
}

void dump(Clock clock) {
  printf("Clock: %d\n\n", counter);
}


