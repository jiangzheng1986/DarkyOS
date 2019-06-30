#include "Kernel.h"

unsigned char g_byKeyCode = 0;

char g_arrKeyTable[0x54] =
{
    0, 0, '1', '2', '3', '4', '5', '6', '7', '8', '9', '0', '-', '=', 0, 0,
    'Q', 'W', 'E', 'R', 'T', 'Y', 'U', 'I', 'O', 'P', '[', ']', 0, 0, 'A', 'S',
    'D', 'F', 'G', 'H', 'J', 'K', 'L', ';', '\'', '`', 0, '\\', 'Z', 'X', 'C', 'V',
    'B', 'N', 'M', ',', '.', '/', 0, '*', 0, ' ', 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, '7', '8', '9', '-', '4', '5', '6', '+', '1',
    '2', '3', '0', '.',
};

void WaitKBCSendReady()
{
    for (;;)
    {
        if ((io_in8(PORT_KEYSTA) & KEYSTA_SEND_NOTREADY) == 0)
        {
            break;
        }
    }
}

void InitKeyboard()
{
    WaitKBCSendReady();
    io_out8(PORT_KEYCMD, KEYCMD_WRITE_MODE);
    
    WaitKBCSendReady();
    io_out8(PORT_KEYDAT, KBC_MODE);
}

void inthandler21(int *esp)
{
    unsigned char byData;
    io_out8(PIC0_OCW2, 0x61);		//通知PIC0,IRQ-1已经受理完成

    byData = io_in8(PORT_KEYDAT);

    g_byKeyCode = byData;
}

char KeyCodeToChar(unsigned char byKeyCode)
{
    if (byKeyCode >= 128)
    {
        return KeyCodeToChar(byKeyCode - 128);
    }
    else if (byKeyCode < 0x54)
    {
        return g_arrKeyTable[byKeyCode];
    }
    else
    {
        return 0;
    }
}
