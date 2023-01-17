#include <stdio.h>
void cpu_dump(struct CPU cpu);
bool cpu_parse(FILE *infile, struct CPU cpu);
void cpu_start_tick();
void cpuDoCycleWork();
struct CPU get();