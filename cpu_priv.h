struct CPU {
  uint8_t regs[8];
  uint8_t PC;
  bool hasBeenInitialized;
  bool waitingOnMemory;
};

void reset(CPU cpu);
void setReg(char *reg, uint8_t hexByte, CPU cpu);
void init(CPU cpu);
