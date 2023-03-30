#ifndef CACHE_H_INCLUDED
#define CACHE_H_INCLUDED

#include <stdio.h>
void cache_dump();
bool cache_parse(FILE *infile);
struct Cache getCache();
bool isFastCache(unsigned int address);
void cacheStore(usigned int address, unsigned int count, uint8_t *dataPtr, bool *memDonePtr);

void cacheFetch(usigned int address, unsigned int count, uint8_t *dataPtr, bool *memDonePtr);

#endif