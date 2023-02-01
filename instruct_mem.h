#ifndef INSTRUCT_MEMORY_H_INCLUDED
#define INSTRUCT_MEMORY_H_INCLUDED

void instMem_dump(int hexAddress, int hexCount);
bool instMem_parse(FILE *infile);
struct InstMemory getInstMem();


#endif