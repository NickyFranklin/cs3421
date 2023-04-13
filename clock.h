#ifndef CLOCK_H_INCLUDED
#define CLOCK_H_INCLUDED

#include <stdio.h>
void clock_dump();
bool clock_parse(FILE *infile);
struct Clock getClock();
int getClockTick();

#endif