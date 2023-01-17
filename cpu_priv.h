struct CPU {
  uint8_t regs[8];
  uint8_t PC;
  bool hasBeenInitialized;
  bool waitingOnMemory;
};

void reset();
void setReg();
void init();
