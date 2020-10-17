include Makefile.inc
MM=-D FREE_LIST_MEMORY_MANAGER
KERNEL=kernel.bin
SOURCES=$(wildcard *.c)
SOURCES_ASM=$(wildcard asm/*.asm)
SOURCES_IDT=$(wildcard IDT/*.c)
SOURCES_IDT_ASM=$(wildcard IDT/*.asm)
SOURCES_DRIVERS=$(wildcard drivers/*.c)
SOURCES_DRIVERS_ASM=$(wildcard drivers/*.asm)
SOURCES_SYSCALL_UTILS=$(wildcard IDT/sysCallUtils/*.c)
SOURCES_SYSCALL_UTILS_ASM=$(wildcard IDT/sysCallUtils/*.asm)

OBJECTS=$(SOURCES:.c=.o)
OBJECTS_ASM=$(SOURCES_ASM:.asm=.o)
OBJECTS_IDT=$(SOURCES_IDT:.c=.o)
OBJECTS_IDT_ASM=$(SOURCES_IDT_ASM:.asm=.o)
OBJECTS_DRIVERS=$(SOURCES_DRIVERS:.c=.o)
OBJECTS_DRIVERS_ASM=$(SOURCES_DRIVERS_ASM:.asm=.o)
OBJECTS_SYSCALL_UTILS=$(SOURCES_SYSCALL_UTILS:.c=.o)
OBJECTS_SYSCALL_UTILS_ASM=$(SOURCES_SYSCALL_UTILS_ASM:.asm=.o)

LOADERSRC=loader.asm

LOADEROBJECT=$(LOADERSRC:.asm=.o)
STATICLIBS=

all: $(KERNEL)

$(KERNEL): $(LOADEROBJECT) $(OBJECTS) $(STATICLIBS) $(OBJECTS_ASM) $(OBJECTS_IDT) $(OBJECTS_IDT_ASM) $(OBJECTS_DRIVERS) $(OBJECTS_DRIVERS_ASM) $(OBJECTS_SYSCALL_UTILS) $(OBJECTS_SYSCALL_UTILS_ASM)
	$(LD) $(LDFLAGS) -T kernel.ld -o $@ $^

%.o: %.c
	$(GCC) $(GCCFLAGS) $(MM) -I./include -I./IDT/include -I./drivers/include -I./IDT/sysCallUtils/include -c $< -o $@

%.o : %.asm
	$(ASM) $(ASMFLAGS) $< -o $@

$(LOADEROBJECT):
	$(ASM) $(ASMFLAGS) $(LOADERSRC) -o $(LOADEROBJECT)

clean:
	rm -rf asm/*.o *.o *.bin ./drivers/*.o ./IDT/*.o ./IDT/sysCallUtils/*.o

.PHONY: all clean
