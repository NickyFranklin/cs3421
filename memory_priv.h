struct Memory {
  int size;
  uint8_t *memIndex;
};

void create(int hexBytes, struct Memory *mem);
void reset(struct Memory *mem);
void set(char hexAddress, char hexBytes, struct Memory *mem);

//Address and count will be parsed separately from all of the bytes
//The hexBytes will be a string of all the hex bytes and then they will be taken
//one by one until they are all gone from the string
void memStartFetch(unsigned int address, unsigned int count, uint8_t *dataPtr, bool *memDonePtr);

bool memParse(FILE *infile, struct Memory *mem);