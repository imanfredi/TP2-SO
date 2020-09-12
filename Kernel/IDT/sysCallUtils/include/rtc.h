#ifndef _RTC_H
#define _RTC_H
#include <stdint.h>

/*Las direcciones son en hexa*/
#define SECONDS 0x00
#define MINUTES 0x02
#define HOURS 0x04
#define BASE 10
int buildTime(uint8_t * totalHour);
int getTime(int value);

#endif