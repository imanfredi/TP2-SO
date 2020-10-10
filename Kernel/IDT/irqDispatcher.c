#include <keyboardDriver.h>
#include <stdint.h>
#include <time.h>

static void int_20();
static void int_21();

void irqDispatcher(uint64_t irq, uint64_t* rsp) {
    switch (irq) {
        case 0:
            int_20();
            break;
        case 1:
            int_21(rsp);
            break;
    }
    return;
}

static void int_20() {
    timer_handler();
}

static void int_21(uint64_t* stackPointer) {
    activateKeyBoard(stackPointer);
}
