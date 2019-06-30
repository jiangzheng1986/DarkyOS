
#include "Kernel.h"

void SetGateDescriptor(GateDescriptor *gd, int offset, int selector, int ar)
{
    gd->offset_low = offset & 0xffff;
    gd->selector = selector;
    gd->dw_count = (ar >> 8) & 0xff;
    gd->access_right = ar & 0xff;
    gd->offset_high = (offset >> 16) & 0xffff;
}

void InitIDT(void)
{	
	GateDescriptor *idt = (GateDescriptor *) ADR_IDT;
	
	int i;
	
	//IDT≥ı ºªØ
	for (i = 0; i < 256; i++)
	{
        SetGateDescriptor(idt + i, 0, 0, 0);
	}
	load_idtr(LIMIT_IDT, ADR_IDT);		//0x07ff = (256 * 8 - 1)
	
    SetGateDescriptor(idt + 0x20, (int) asm_inthandler20, 2 * 8, AR_INTGATE32);
    SetGateDescriptor(idt + 0x21, (int) asm_inthandler21, 2 * 8, AR_INTGATE32);
    SetGateDescriptor(idt + 0x27, (int) asm_inthandler27, 2 * 8, AR_INTGATE32);
    SetGateDescriptor(idt + 0x2c, (int) asm_inthandler2c, 2 * 8, AR_INTGATE32);
}
