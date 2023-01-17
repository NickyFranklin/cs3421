struct Clock {
  uint16_t counter;
};

void reset(struct Clock *clock);
void tick(int ticks, struct Clock *clock);
void init(struct Clock *clock);
