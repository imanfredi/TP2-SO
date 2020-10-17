MM=FREE_LIST_MEMORY_MANAGER

all:  bootloader kernel userland image

bootloader:
	cd Bootloader; make all

kernel:
	cd Kernel; make all MM=-D$(MM)

userland:
	cd Userland; make all

image: kernel bootloader userland
	cd Image; make all

clean:
	cd Bootloader; make clean
	cd Image; make clean
	cd Kernel; make clean
	cd Userland; make clean

CPP_SOURCES=$(shell find . -name '*.c')
CPP_OUT=$(CPP_SOURCES:.c=.cpp)

cpp: $(CPP_OUT)

%.cpp: %.c
	cppcheck --quiet --enable=all --force --inconclusive  $< 2>> out.cppOut


cleanCpp:
		rm out.cppOut


.PHONY: bootloader image collections kernel userland all clean
