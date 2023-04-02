#ifndef CPU_H_INCLUDED
#define CPU_H_INCLUDED

#include <stdio.h>
void cpu_dump();
bool cpu_parse(FILE *infile);
void cpu_start_tick();
void cpuDoCycleWork();
bool cpuIsMoreCycleWork();
struct CPU getCpu();

#endif

