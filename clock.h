#include <stdio.h>
void clock_dump(struct Clock *clock);
bool clock_parse(FILE *infile, struct Clock *clock, struct CPU *cpu);
struct Clock getClock();