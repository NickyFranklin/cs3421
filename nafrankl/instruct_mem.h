#ifndef INSTRUCT_MEMORY_H_INCLUDED
#define INSTRUCT_MEMORY_H_INCLUDED
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>

void instMem_dump(int hexAddress, int hexCount);
bool instMem_parse(FILE *infile);
struct InstMemory getInstMem();
uint32_t imemFetch(uint8_t address);

#endif