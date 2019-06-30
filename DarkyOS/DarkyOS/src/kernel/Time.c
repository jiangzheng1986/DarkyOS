#include "Kernel.h"

unsigned char GetCmosData_BCD(unsigned char byAddress)
{
    io_out8(0x70, byAddress);

    unsigned char byData_BCD = io_in8(0x71);

    return byData_BCD;
}

unsigned char BCD_To_Byte(unsigned char byBCD)
{
    unsigned char byHigh = (byBCD >> 4);
    unsigned char byLow = (byBCD & 0x0F);
    return byHigh * 10 + byLow;
}

unsigned char GetCmosData(unsigned char byAddress)
{
    unsigned char byData_BCD = GetCmosData_BCD(byAddress);

    unsigned char byData = BCD_To_Byte(byData_BCD);

    return byData;
}

void GetLocalTime(SystemTime *pLocalTime)
{
    if (pLocalTime)
    {
        pLocalTime->byYear = GetCmosData(9);
        pLocalTime->byMonth = GetCmosData(8);
        pLocalTime->byDay = GetCmosData(7);
        pLocalTime->byHour = GetCmosData(4);
        pLocalTime->byMinute = GetCmosData(2);
        pLocalTime->bySecond = GetCmosData(0);
    }
}
