#ifndef CPU_PRIV_H_INCLUDED
#define CPU_PRIV_H_INCLUDED


struct CPU {
  uint8_t regs[8];
  uint8_t PC;
  bool hasBeenInitialized;
  bool waitingOnMemory;
};

static void reset();
static void setReg(char* reg, uint8_t hexByte);
static void init();

#endif
