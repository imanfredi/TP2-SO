#include <stdint.h>

uint32_t GetUint();
uint32_t GetUniform(uint32_t max);
uint8_t memcheck(void *start, uint8_t value, uint32_t size);

int test_processes(int argc, char*argv[]);
int test_mm(int argc, char*argv[]);
int test_prio(int argc, char*argv[]);

//int test_sync(int argc, char*argv[]);
//void test_no_sync();