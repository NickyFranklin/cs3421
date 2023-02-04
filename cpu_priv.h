#ifndef CPU_PRIV_H_INCLUDED
#define CPU_PRIV_H_INCLUDED

enum State {WAIT, FETCH};
enum Instruction{LW = 5, SW = 6};

struct CPU {
  uint8_t regs[8];
  uint8_t PC;
  bool hasBeenInitialized;
  enum State state;
  uint32_t command;
};

static void reset();
static void setReg(char* reg, uint8_t hexByte);
static void initCpu();

#endif
