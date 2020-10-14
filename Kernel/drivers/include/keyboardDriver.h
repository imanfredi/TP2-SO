#ifndef _KEYBOARD_H
#define _KEYBOARD_H
#include <lib.h>
#include <stdint.h>
#define KEYS 59
#define BUFFER_KEYBOARD 30
#define PRESSED 1
#define RELEASED 2
#define ERRROR -1

#define L_SHFT_PRESSED 0x2A
#define L_SHFT_RELEASED 0xAA
#define R_SHFT_PRESSED 0x36
#define R_SHFT_RELEASED 0xB6
#define L_CTRL_PRESSED 0x1D
#define L_CTRL_RELEASED 0x9D
#define REGISTERS 17
#define BLQ_MAYUS 0x3A
#define ENTER 0x1C
#define BACK_SPACE 0x0E
#define REGISTERS 17
#define EOF -1
#define CLEAR_SCREEN 252

uint8_t hasKey();
uint8_t getKey();
void activateKeyBoard(uint64_t * stackPointer);
char getCharacter();
uint64_t getRegisters();
int initKeyboard();

#endif