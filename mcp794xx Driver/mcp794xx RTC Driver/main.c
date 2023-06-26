#include "mcp794xx_driver_basic.h"

uint8_t time = 0xff;

int main()
{
    printf("time: %x", time & (MCP794XX_12HR_24HR_FRMT_STAT_MASK | MCP794XX_TIME_AM_PM_MASK) );
    return 0;
}
