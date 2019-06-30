#include "Kernel.h"

MouseDecoder g_MouseDec;

int g_bLeftMouseDown = 0;
int g_bRightMouseDown = 0;
int g_bCenterMouseDown = 0;

int g_nMouseX = 0;
int g_nMouseY = 0;

void InitMouse()
{
    WaitKBCSendReady();
    io_out8(PORT_KEYCMD, KEYCMD_SENDTO_MOUSE);

    WaitKBCSendReady();
    io_out8(PORT_KEYDAT, MOUSECMD_ENABLE);

    g_MouseDec.x = 0;
    g_MouseDec.y = 0;
    g_MouseDec.btn = 0;
    g_MouseDec.phase = 0;
    g_MouseDec.buf[0] = 0;
    g_MouseDec.buf[1] = 0;
    g_MouseDec.buf[2] = 0;

    int nScreenWidth = GetScreenWidth();
    int nScreenHeight = GetScreenHeight();

    g_nMouseX = nScreenWidth / 2 - 8;
    g_nMouseY = nScreenHeight / 2 - 8;
}

int MouseDecode(unsigned char byData)
{
    MouseDecoder *pMouseDec = &g_MouseDec;

    if (pMouseDec->phase == 0)
    {
        if (byData == 0xfa)
        {
            pMouseDec->phase = 1;
        }
        return 0;
    }
    else if (pMouseDec->phase == 1)
    {
        if ((byData & 0xc8) == 0x08)
        {
            pMouseDec->buf[0] = byData;
            pMouseDec->phase = 2;
        }
        return 0;
    }
    else if (pMouseDec->phase == 2)
    {
        pMouseDec->buf[1] = byData;
        pMouseDec->phase = 3;
        return 0;
    }
    else if (pMouseDec->phase == 3)
    {
        pMouseDec->buf[2] = byData;
        pMouseDec->phase = 1;

        pMouseDec->btn = pMouseDec->buf[0] & 0x07;
        pMouseDec->x = pMouseDec->buf[1];
        pMouseDec->y = pMouseDec->buf[2];

        if ((pMouseDec->buf[0] & 0x10) != 0)
        {
            pMouseDec->x |= 0xffffff00;
        }

        if ((pMouseDec->buf[0] & 0x20) != 0)
        {
            pMouseDec->y |= 0xffffff00;
        }

        pMouseDec->y = -pMouseDec->y;

        return 1;
    }
    return -1;
}

void inthandler2c(int *esp)
{
    unsigned char byData;
    io_out8(PIC1_OCW2, 0x64);		//通知PIC1,IRQ-12已经受理完成
    io_out8(PIC0_OCW2, 0x62);		//通知PIC0,IRQ-2已经受理完成

    byData = io_in8(PORT_KEYDAT);

    if (MouseDecode(byData))
    {
        int nScreenWidth = GetScreenWidth();

        int nScreenHeight = GetScreenHeight();

        MouseDecoder *pMouseDec = &g_MouseDec;

        g_bLeftMouseDown = 0;
        g_bRightMouseDown = 0;
        g_bCenterMouseDown = 0;

        if ((pMouseDec->btn & 0x01) != 0)
        {
            g_bLeftMouseDown = 1;
        }

        if ((pMouseDec->btn & 0x02) != 0)
        {
            g_bRightMouseDown = 1;
        }

        if ((pMouseDec->btn & 0x04) != 0)
        {
            g_bCenterMouseDown = 1;
        }

        g_nMouseX += pMouseDec->x;
        g_nMouseY += pMouseDec->y;
        if (g_nMouseX < 0)
        {
            g_nMouseX = 0;
        }
        if (g_nMouseY < 0)
        {
            g_nMouseY = 0;
        }
        if (g_nMouseX > nScreenWidth - 1)
        {
            g_nMouseX = nScreenWidth - 1;
        }
        if (g_nMouseY > nScreenHeight - 1)
        {
            g_nMouseY = nScreenHeight - 1;
        }
    }
}
