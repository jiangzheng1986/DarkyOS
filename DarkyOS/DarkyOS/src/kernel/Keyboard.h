#ifndef KEYBOARD_H
#define KEYBOARD_H

extern unsigned char g_byKeyCode;

void InitKeyboard();

char KeyCodeToChar(unsigned char byKeyCode);

#endif
