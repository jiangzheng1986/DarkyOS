#ifndef IDT_H
#define IDT_H

typedef struct
{
    short offset_low, selector;
    char dw_count, access_right;
    short offset_high;
}GateDescriptor;

void InitIDT(void);

void asm_inthandler20();
void asm_inthandler21();
void asm_inthandler27();
void asm_inthandler2c();

#endif

