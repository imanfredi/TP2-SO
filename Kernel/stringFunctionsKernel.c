#include <stdint.h>
#include <stringFunctionsKernel.h>


int strlen(const uint8_t *s)
{

    int i = 0;

    while (s[i] != 0)
        i++;
   
    return i;

}

uint32_t uintToBase(uint64_t value, uint8_t *buffer, uint32_t base)
{
    uint8_t *p = buffer;
    uint8_t *p1, *p2;
    uint32_t digits = 0;

    //Calculate uint8_tacters for each digit
    do
    {
        uint32_t remainder = value % base;
        *p++ = (remainder < 10) ? remainder + '0' : remainder + 'A' - 10;
        digits++;
    } while (value /= base);

    // Terminate string in buffer.
    *p = 0;

    //Reverse string in buffer.
    p1 = buffer;
    p2 = p - 1;
    while (p1 < p2)
    {
        uint8_t tmp = *p1;
        *p1 = *p2;
        *p2 = tmp;
        p1++;
        p2--;
    }

    return digits;
}

uint32_t uintToBaseWithLength(uint64_t value, uint8_t *buffer, uint32_t base, uint8_t size) {
      uint8_t *p = buffer + size - 1;
      *p-- = 0;
      uint32_t digits = 0;
      //Calculate characters for each digit
      do {
            uint64_t remainder = value % base;
            *p-- = (remainder < 10) ? remainder + '0' : remainder + 'A' - 10;
            digits++;
      } while (value /= base);

      while (buffer <= p) {
            *p-- = '0';
      }

      return digits;
}
