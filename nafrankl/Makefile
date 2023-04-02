cs3421_emul: parser.o clock.o cpu.o memory.o instructionMemory.o cache.o
	gcc -g -o cs3421_emul $^

parser.o: clock.h cpu.h memory.h clock_priv.h cpu_priv.h memory_priv.h cache.h cache_priv.h
cpu.o: cpu.h cpu_priv.h
memory.o: memory.h memory_priv.h
clock.o: clock.h clock_priv.h
instmem.o: instruct_mem.h priv_instruct_mem.h
cache.o: cache.h cache_priv.h

%.o: %.c
	gcc -g -c $<

clean:
	rm *.o cs3421_emul
