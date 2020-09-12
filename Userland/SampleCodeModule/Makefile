# include ../Makefile.inc

# MODULE=0000-sampleCodeModule.bin
# SOURCES=$(wildcard *.c)
# SOURCES_ASM=$(wildcard asm/*.asm)
# SOURCES_APPLICATIONS=$(wildcard applications/*.c)

# OBJECTS=$(SOURCES:.c=.o)
# OBJECTS_ASM=$(SOURCES_ASM:.asm=.o)
# OBJECTS_APPLICATIONS=$(SOURCES_APPLICATIONS:.c=.o)

# all: $(MODULE)

# $(MODULE): $(OBJECTS) $(OBJECTS_ASM) $(OBJECTS_APPLICATIONS)
# 	$(LD) $(LDFLAGS) -T sampleCodeModule.ld -o ../$@ $^

# %.o: %.c
# 	$(GCC) $(GCCFLAGS) -I./include -c $< -o $@

# %.o:%.asm
# 	$(ASM) $(ASMFLAGS) $< -o $@


# clean:
# 	rm -rf *.o asm/*.o applications/*.o

# .PHONY: all clean print

include ../Makefile.inc

MODULE=0000-sampleCodeModule.bin
SOURCES=$(wildcard [^_]*.c)
SOURCES_ASM=$(wildcard asm/*.asm)
SOURCES_APPLICATIONS=$(wildcard applications/*.c)

OBJECTS=$(SOURCES:.c=.o)
OBJECTS_ASM=$(SOURCES_ASM:.asm=.o)
OBJECTS_APPLICATIONS=$(SOURCES_APPLICATIONS:.c=.o)

all: $(MODULE)

$(MODULE): $(SOURCES) $(SOURCES_APPLICATIONS) $(OBJECTS_ASM)
	$(GCC) $(GCCFLAGS) -I./include -T sampleCodeModule.ld _loader.c $^ -o ../$(MODULE)


%.o:%.asm
	$(ASM) $(ASMFLAGS) $< -o $@

clean:
	rm -rf *.o asm/*.o applications/*.o

.PHONY: all clean print