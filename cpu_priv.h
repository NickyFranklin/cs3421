#ifndef CPU_PRIV_H_INCLUDED
#define CPU_PRIV_H_INCLUDED
#include <stdbool.h>

enum {LW = 5, SW = 6, ADD = 0, ADDI = 1, MUL = 2, INV = 3, B = 4, HALT = 7} Instruction;
enum {BEQ = 0, BNEQ = 1, BLT = 2} BranchInstruction;
struct CPU {
  uint8_t regs[8];
  uint8_t PC;
  bool hasBeenInitialized;
  enum {WAIT, FETCH} state;
  uint32_t command;
  uint8_t *memPtr;
  bool memDone;
  bool *memDonePtr;  
};

static void reset();
static void setReg(char* reg, uint8_t hexByte);
static void initCpu();

#endif
