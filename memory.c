#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "cpu.h"
#include "cpu_priv.h"
#include <string.h>

void memStartFetch(unsigned int address, unsigned int count, uint8_t *dataPtr, boll *memDonePtr) {
  if (1 == count) {
    *dataPtr = memPtr[address];
  }
  else {
    memcpy(dataPtr, memPtr+address, count);
  }
  *memDonePtr = true;
  
}
