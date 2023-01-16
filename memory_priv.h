void create(int hexBytes);
void reset();
void set(char *hexAddress, char *hexCount, char *hexBytes);

//Address and count will be parsed separately from all of the bytes
//The hexBytes will be a string of all the hex bytes and then they will be taken
//one by one until they are all gone from the string
void memStartFetch(unsigned int address, unsigned int count, uint8_t *dataPtr, bool *memDonePtr);

bool memParse(File *infile);