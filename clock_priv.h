struct Clock {
  uint16_t counter;
};

void reset();
void tick(int ticks);
void init();
bool clock_parse(File *infile);