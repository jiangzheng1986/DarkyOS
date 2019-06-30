#include "Kernel.h"

extern char ascii[4096];

unsigned char *g_pBackBuffer = NULL;

BootInfo *GetBootInfo()
{
    return (BootInfo *)ADR_BOOTINFO;
}

unsigned char *GetVideoMemory()
{
    return (unsigned char *)(GetBootInfo()->vram);
}

int GetScreenWidth()
{
    return GetBootInfo()->scrnx;
}

int GetScreenHeight()
{
    return GetBootInfo()->scrny;
}

void ColorIndexToRGB(unsigned char byColorIndex, unsigned char *pr, unsigned char *pg, unsigned char *pb)
{
    int nColorIndex = byColorIndex;
    int nB = nColorIndex / 36;
    int nTemp1 = (nColorIndex % 36);
    int nG = nTemp1 / 6;
    int nR = nTemp1 % 6;
    *pr = nR * 51;
    *pg = nG * 51;
    *pb = nB * 51;
}

unsigned char RGBToColorIndex_Slow(unsigned int dwColor)
{
    unsigned char *pColor = (unsigned char *)&dwColor;
    int nR = pColor[0];
    int nG = pColor[1];
    int nB = pColor[2];
    nR = nR / 51;
    nG = nG / 51;
    nB = nB / 51;
    return nB * 36 + nG * 6 + nR;
}

unsigned char RGBToColorIndex_Opt(unsigned int dwColor)
{
    unsigned char *pColor = (unsigned char *)&dwColor;
    int nR = pColor[0];
    int nG = pColor[1];
    int nB = pColor[2];
    nR = ((nR << 2) + nR) >> 8;
    nG = ((nG << 2) + nG) >> 8;
    nB = ((nB << 2) + nB) >> 8;
    return (nB << 5) + (nB << 2) +
        (nG << 2) + (nG << 1) +
        nR;
}

unsigned char RGBToColorIndex_Dither_Slow(unsigned int dwColor, int nX, int nY)
{
    unsigned char *pColor = (unsigned char *)&dwColor;
    int nR = pColor[0];
    int nG = pColor[1];
    int nB = pColor[2];
    static int arrTable[4] = { 3, 1, 0, 2 };
    int nIndex;
    nX &= 1;
    nY &= 1;
    nIndex = arrTable[nX + (nY * 2)];
    nR = (nR * 21) / 256;
    nG = (nG * 21) / 256;
    nB = (nB * 21) / 256;
    nR = (nR + nIndex) / 4;
    nG = (nG + nIndex) / 4;
    nB = (nB + nIndex) / 4;
    return nB * 36 + nG * 6 + nR;
}

unsigned char RGBToColorIndex_Dither_Opt(unsigned int dwColor, int nX, int nY)
{
    unsigned char *pColor = (unsigned char *)&dwColor;
    int nR = pColor[0];
    int nG = pColor[1];
    int nB = pColor[2];
    static int arrTable[4] = { 3, 1, 0, 2 };
    int nIndex;
    nX &= 1;
    nY &= 1;
    nIndex = arrTable[nX + (nY << 1)];
    nR = ((nR << 4) + (nR << 2) + nR) >> 8;
    nG = ((nG << 4) + (nG << 2) + nG) >> 8;
    nB = ((nB << 4) + (nB << 2) + nB) >> 8;
    nR = (nR + nIndex) >> 2;
    nG = (nG + nIndex) >> 2;
    nB = (nB + nIndex) >> 2;
    return (nB << 5) + (nB << 2) +
        (nG << 2) + (nG << 1) +
        nR;
}

void InitBackBuffer()
{
    int nScreenWidth = GetScreenWidth();
    int nScreenHeight = GetScreenHeight();

    g_pBackBuffer = (unsigned char *)MemoryAlloc4K(nScreenWidth * nScreenHeight * sizeof(unsigned char));
}

void SetPalette(int start, int end, unsigned char *rgb)
{
    int i, eflags;

    eflags = io_load_eflags();

    io_cli();

    io_out8(0x03c8, start);

    for (i = start; i <= end; i++)
    {
        io_out8(0x03c9, rgb[0] / 4);
        io_out8(0x03c9, rgb[1] / 4);
        io_out8(0x03c9, rgb[2] / 4);
        rgb += 3;
    }

    io_store_eflags(eflags);
}

void InitPallete()
{
    unsigned char table_rgb[256 * 3];
    unsigned char r = 0;
    unsigned char g = 0;
    unsigned char b = 0;

    int i;
    for (i = 0; i < 255; i++)
    {
        ColorIndexToRGB(i, &r, &g, &b);

        table_rgb[i * 3 + 0] = r;
        table_rgb[i * 3 + 1] = g;
        table_rgb[i * 3 + 2] = b;
    }

    SetPalette(0, 255, table_rgb);
}

void InitGraphics()
{
    InitBackBuffer();

    InitPallete();
}

void SwapBuffer()
{
    //后面可以用rep movsd优化

    unsigned char *pVideoMemory = GetVideoMemory();
    int nScreenWidth = GetScreenWidth();
    int nScreenHeight = GetScreenHeight();

    unsigned char *pVideoMemory1 = pVideoMemory;
    unsigned char *pBackBuffer1 = g_pBackBuffer;

    int i1;
    for (i1 = 0; i1 < nScreenHeight; i1++)
    {
        int i2;
        for (i2 = 0; i2 < nScreenWidth; i2++)
        {
            (*pVideoMemory1) = (*pBackBuffer1);
            pVideoMemory1++;
            pBackBuffer1++;
        }
    }
}

void ClearScreen(unsigned int dwColor)
{
    //后面可以用rep stosd优化

    unsigned char byColorIndex = RGBToColorIndex_Opt(dwColor);

    int nScreenWidth = GetScreenWidth();
    int nScreenHeight = GetScreenHeight();

    unsigned char *pBackBuffer1 = g_pBackBuffer;

    int i1;
    for (i1 = 0; i1 < nScreenHeight; i1++)
    {
        int i2;
        for (i2 = 0; i2 < nScreenWidth; i2++)
        {
            (*pBackBuffer1) = byColorIndex;
            pBackBuffer1++;
        }
    }
}

void FillRectangle(unsigned int dwColor, int nX, int nY, int nWidth, int nHeight)
{
    unsigned char byColorIndex = RGBToColorIndex_Opt(dwColor);

    int nScreenWidth = GetScreenWidth();
    int nScreenHeight = GetScreenHeight();

    unsigned char *pBackBuffer1 = g_pBackBuffer;

    int nX1 = MAX(nX, 0);
    int nY1 = MAX(nY, 0);
    
    int nX2 = nX + nWidth;
    int nY2 = nY + nHeight;
    nX2 = MIN(nX2, nScreenWidth);
    nY2 = MIN(nY2, nScreenHeight);

    int i1;
    for (i1 = nY1; i1 < nY2; i1++)
    {
        unsigned char *pBase = pBackBuffer1 + nScreenWidth * i1;

        int i2;
        for (i2 = nX1; i2 < nX2; i2++)
        {
            (*(pBase + i2)) = byColorIndex;
        }
    }
}

void InnerDrawChar(unsigned char byColorIndex, int nX, int nY, char chChar)
{
    char *pFont = ascii + chChar * 16;

    int nScreenWidth = GetScreenWidth();

    unsigned char *pBackBuffer1 = g_pBackBuffer;

    int i;
    char d;
    unsigned char *p;
    int x = nX;
    int y = nY;
    unsigned char c = byColorIndex;
    for (i = 0; i < 16; i++)
    {
        p = pBackBuffer1 + (y + i) * nScreenWidth + x;
        d = pFont[i];
        if ((d & 0x80) != 0) { p[0] = c; }
        if ((d & 0x40) != 0) { p[1] = c; }
        if ((d & 0x20) != 0) { p[2] = c; }
        if ((d & 0x10) != 0) { p[3] = c; }
        if ((d & 0x08) != 0) { p[4] = c; }
        if ((d & 0x04) != 0) { p[5] = c; }
        if ((d & 0x02) != 0) { p[6] = c; }
        if ((d & 0x01) != 0) { p[7] = c; }
    }
}

void DrawString(unsigned int dwColor, int nX, int nY, int nWidth, int nHeight, char *pszString)
{
    unsigned char byColorIndex = RGBToColorIndex_Opt(dwColor);

    char *pszString1 = pszString;

    int nSavedX = nX;
    int nSavedY = nY;
    int nCharIndexInLine = 0;

    while (1)
    {
        char chChar = (*pszString1);

        if (chChar == 0)
        {
            break;
        }

        if (nX + 8 >= nSavedX + nWidth)
        {
            nY += 16;
            nX = nSavedX;
        }

        if (nY + 16 >= nSavedY + nHeight)
        {
            break;
        }

        if (chChar == '\r')
        {
        }
        else if (chChar == '\n')
        {
            nY += 16;
            nX = nSavedX;
            nCharIndexInLine = 0;
        }
        else if (chChar == '\t')
        {
            nX += 8 * 4;
            nCharIndexInLine = (nCharIndexInLine + 7) & ~7;
            nX = nSavedX + 8 * nCharIndexInLine;
        }
        else
        {
            InnerDrawChar(byColorIndex, nX, nY, chChar);

            nX += 8;
            nCharIndexInLine++;
        }

        pszString1++;
    }
}

void DrawImage(int nX, int nY, int nWidth, int nHeight, unsigned char *pImageData)
{
    int nScreenWidth = GetScreenWidth();
    //int nScreenHeight = GetScreenHeight();

    unsigned char *pBackBuffer1 = g_pBackBuffer;

    unsigned int *pImageData1 = (unsigned int *)pImageData;

    int i1;
    for (i1 = 0; i1 < nHeight; i1++)
    {
        int nY1 = nY + i1;
        unsigned char *pBase = pBackBuffer1 + nScreenWidth * nY1;
        int nBase = i1 * nWidth;

        int i2;
        for (i2 = 0; i2 < nWidth; i2++)
        {
            int nX1 = nX + i2;

            unsigned int dwColor = pImageData1[nBase + i2];

            unsigned char byColorIndex = RGBToColorIndex_Dither_Opt(dwColor, nX1, nY1);

            *(pBase + nX1) = byColorIndex;
        }
    }
}
