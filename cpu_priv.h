struct CPU {
  uint8_t regs[8];
  uint8_t PC;
  bool hasBeenInitialized;
  bool waitingOnMemory;
};

void reset(struct CPU cpu);
void setReg(char* reg, uint8_t hexByte, struct CPU cpu);
void init(struct CPU cpu);
