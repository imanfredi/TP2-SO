#include <stdint.h>
#include <selfLib.h>
#include <stdarg.h>
#include <utils.h>
#include <stringFunctions.h>

int strcmp(const uint8_t *s1, const uint8_t *s2)
{
    int i = 0;
    while (s1[i] != 0 && s2[i] != 0 && s1[i] == s2[i])
    {
        i++;
    }
    if (s1[i] == s2[i])
    {
        return 0;
    }
    return s1[i] < s2[i] ? -1 : 1;
}

int strncmp(const uint8_t *s1, const uint8_t *s2, int n)
{
    int i = 0;
    while (s1[i] != 0 && s2[i] != 0 && s1[i] == s2[i] && i < n)
    {
        i++;
    }
    if (s1[i] == s2[i])
    {
        return 0;
    }
    return s1[i] < s2[i] ? -1 : 1;
}

uint8_t *strcpy(uint8_t dest[], const uint8_t source[])
{
    int i = 0;
    while (source[i] != '\0')
    {
        dest[i] = source[i];
        i++;
    }

    return dest;
}

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

    //Calculate characters for each digit
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
    
      return p;
}

int printf(const char *format, ...){
    va_list vl;
    uint8_t buffer[300] = {0}; //habria que poner lo maximo de media pantalla permitido
    uint8_t num[30];
    int i = 0, j = 0;
    uint8_t *s;

    va_start(vl, format);
    while (format[i])
    {

        if (format[i] == '%')
        {
            if(format[i+1]){

                switch (format[i+1])
                {

                case 'c':{
                
                    buffer[j] = (uint8_t)va_arg(vl, int);
                    i++;
                    j++;
                    break;
                
                }
                case 'd':{
                    itoa(va_arg(vl, int), num, 10);
                    strcpy(buffer + j, num);
                    j += strlen(num);
                    i++;
                    break;
                }
                case 's':{
                             
                    s = va_arg(vl, uint8_t *);
                    strcpy(buffer + j, s);
                    j += strlen(s);
                    i++;
                    break;
                }
                default:
                    buffer[j++]='%';
                }
            }
            else{
                buffer[j++]='%';
            }
        }
        else
        {
            buffer[j++] = format[i];
        }

        i++;
    }
    buffer[j++] = '\0';
    
    printString(buffer);
   
    va_end(vl);
    return j;
}

void itoa(int value, uint8_t *str, int base)
{
    int i = 0;
    if (value < 0)
    {
        str[i++] = '-';
        value = -value;
    }
    i += uintToBase(value, str + i, base);
    str[++i] = '\0';
}

uint64_t stringHexaToNumber(uint8_t *string)
{
    uint64_t number = 0;
    uint8_t aux;
    uint64_t len = strlen(string);

    uint64_t i = 0;
    uint8_t error = 0;
    while(i<len && !error){
        aux = string[len-i-1];
        if(aux >= '0' && aux <= '9'){
            aux -= '0';
        }
        else if((aux >= 'A' && aux <= 'F') || (aux>='a' && aux<='f')){
            aux-= 'A' + 10;
        }
        else{
            aux = -1;
            error = 1;
        }
        if(!error){
            number += pow(16,i)*aux;
        }
        i++;
    }
    return number;
}

void *memcpy(void *destination, const void *source, uint64_t length)
{
    /*
	* memcpy does not support overlapping buffers, so always do it
	* forwards. (Don't change this without adjusting memmove.)
	*
	* For speedy copying, optimize the common case where both pointers
	* and the length are word-aligned, and copy word-at-a-time instead
	* of byte-at-a-time. Otherwise, copy by bytes.
	*
	* The alignment logic below should be portable. We rely on
	* the compiler to be reasonably intelligent about optimizing
	* the divides and modulos out. Fortunately, it is.
	*/
    uint64_t i;

    if ((uint64_t)destination % sizeof(uint32_t) == 0 &&
        (uint64_t)source % sizeof(uint32_t) == 0 &&
        length % sizeof(uint32_t) == 0)
    {
        uint32_t *d = (uint32_t *)destination;
        const uint32_t *s = (const uint32_t *)source;

        for (i = 0; i < length / sizeof(uint32_t); i++)
            d[i] = s[i];
    }
    else
    {
        uint8_t *d = (uint8_t *)destination;
        const uint8_t *s = (const uint8_t *)source;

        for (i = 0; i < length; i++)
            d[i] = s[i];
    }

    return destination;
}

void ftoa(double value, uint8_t *str, uint8_t precision) {
      int i = 0;
      if (value < 0) {
            str[i++] = '-';
            value *= -1;
      }
      i += uintToBase((int)value, str + i, 10);
      str[i++] = '.';
      str[i] = 0;
      double decPart = (value - (int)value);
      decPart *= pow(10, precision);
      int intPart = (float)decPart;
      i += uintToBaseWithLength(intPart, str + i, 10, precision + 1);
}