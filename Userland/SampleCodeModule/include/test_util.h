#include <stdint.h>

uint32_t GetUint();
uint32_t GetUniform(uint32_t max);
uint8_t memcheck(void *start, uint8_t value, uint32_t size);

int test_processes();
int test_mm();
int test_prio();

int test_sync();
int test_no_sync();