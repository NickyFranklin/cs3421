struct CPU {
  uint8_t regs[8];
  uint8_t PC;
  Bool hasBeenInitialized;
  Bool waitingOnMemory;
};

void reset();
void setReg(char *reg, uint8_t hexByte);
void dump();
