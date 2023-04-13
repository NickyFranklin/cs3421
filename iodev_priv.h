#ifndef IODEV_PRIV_H_INCLUDED
#define IODEV_PRIV_H_INCLUDED

struct IoDev {
  uint8_t reg;
  uint16_t nextInstruction;
  enum {IOWRITE, IOREAD, INVALID5} ioState;
  uint8_t address;
  uint8_t value;
  FILE *file;
  bool boolin;
};

static void reset();
static void load(char* file);
static void initIo();
static void fileReadNext();

#endif