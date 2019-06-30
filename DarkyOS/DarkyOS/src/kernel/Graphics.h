#ifndef GRAPHICS_H
#define GRAPHICS_H

#define RGB(r, g, b)  ((r) | ((g) << 8) | ((b) << 16))

typedef struct
{
    char cyls, leds, vmode, reserve;
    short scrnx, scrny;
    char *vram;
}BootInfo;

BootInfo *GetBootInfo();

int GetScreenWidth();

int GetScreenHeight();

void InitGraphics();

void SwapBuffer();

void ClearScreen(unsigned int dwColor);

void FillRectangle(unsigned int dwColor, int nX, int nY, int nWidth, int nHeight);

void DrawString(unsigned int dwColor, int nX, int nY, int nWidth, int nHeight, char *pszString);

void DrawImage(int nX, int nY, int nWidth, int nHeight, unsigned char *pImageData);

#endif
