#include "Kernel.h"

unsigned int g_dwTimerCounter = 0;

void InitTimer(void)
{
    io_out8(PIT_CTRL, 0x34);
    io_out8(PIT_CNT0, 0x9c);
    io_out8(PIT_CNT0, 0x2e);		//0x2e9c=11932,100Hz
}

void inthandler20(int *esp)
{
    io_out8(PIC0_OCW2, 0x60);		//֪ͨPIC0,IRQ-0�Ѿ��������

    g_dwTimerCounter++;
}
