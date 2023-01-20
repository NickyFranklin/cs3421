cs3421_emul: parser.o clock.o cpu.o memory.o
	gcc -g -o -no-pie cs3421_emul.c $^

parser.o: clock.h cpu.h memory.h clock_priv.h cpu_priv.h memory_priv.h
cpu.o: cpu.h cpu_priv.h
memory.o: memory.h memory_priv.h
clock.o: clock.h clock_priv.h

%.o: %.c
	gcc -g -c -no-pie $<

clean:
	rm *.o cs3421_emul
