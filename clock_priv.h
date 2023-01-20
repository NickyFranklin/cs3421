#ifndef CLOCK_PRIV_H_INCLUDED
#define CLOCK_PRIV_H_INCLUDED

struct Clock {
  uint16_t counter;
};

static void reset();
static void tick(int ticks);
static void init();

#endif