#ifndef MOUSE_H
#define MOUSE_H

typedef struct
{
    unsigned char buf[3], phase;
    int x, y, btn;
}MouseDecoder;

extern int g_bLeftMouseDown;
extern int g_bRightMouseDown;
extern int g_bCenterMouseDown;

extern int g_nMouseX;
extern int g_nMouseY;

void InitMouse();

#endif
