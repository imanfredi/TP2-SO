#include <rtc.h>
static uint8_t BCDtoDecimal(uint8_t number);
static uint32_t uintToBase(uint64_t value, char *buffer, uint32_t base);
static uint8_t getHour(uint8_t hour);

int buildTime(uint8_t * totalHour)
    {
        uint8_t hours, mins, seconds;

        hours = getTime(HOURS);
        uintToBase(getHour(BCDtoDecimal(hours)), (char *)totalHour, BASE);
        totalHour[2] = ':';

        mins = getTime(MINUTES);
        uintToBase(BCDtoDecimal(mins), (char *)totalHour + 3, BASE);
        totalHour[5] = ':';

        seconds = getTime(SECONDS);
        uintToBase(BCDtoDecimal(seconds), (char *)totalHour + 6, BASE);
        return 1;
 }

static   uint8_t getHour(uint8_t hour)
{
        switch (hour)
        {
        case 0:
            return 21;
        case 1:
            return 22;
        case 2:
            return 23;
        default:
            return hour - 3;
        }
}

static uint8_t BCDtoDecimal(uint8_t number)
{

        uint8_t unit = number & 0x0F; // & 0000 FFFF
        uint8_t ten = number & 0xF0;  // & FFFF 0000

        ten >>= 4;

        return ten * 10 + unit;
    }
    static uint32_t uintToBase(uint64_t value, char *buffer, uint32_t base)
    {
        uint64_t aux=value;
        char *p = buffer;
        char *p1, *p2;
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
            char tmp = *p1;
            *p1 = *p2;
            *p2 = tmp;
            p1++;
            p2--;
        }
        
        if (aux< base)
        {
            buffer[1] = buffer[0];
            buffer[0] = '0';
            buffer[2] = 0;
        }
        return digits;
    }
