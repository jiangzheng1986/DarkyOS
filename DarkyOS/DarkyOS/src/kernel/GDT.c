
#include "Kernel.h"

void SetSegmentDescriptor(SegmentDescriptor *sd, unsigned int limit, int base, int ar)
{
    //base = base_low(16) + base_mid(8) + base_high(8) = 32bit
    //limit = limit_low(16) + limit_high(4) = 20bit
    //ar = access_right(8) + limit_high(4) = 12bit

    if (limit > 0xfffff)
    {
        ar |= 0x8000;		/* G_bit = 1 */
        limit /= 0x1000;
    }

    sd->limit_low = limit & 0xffff;
    sd->base_low = base & 0xffff;
    sd->base_mid = (base >> 16) & 0xff;
    sd->access_right = ar & 0xff;
    sd->limit_high = ((limit >> 16) & 0x0f) | ((ar >> 8) & 0xf0);
    sd->base_high = (base >> 24) & 0xff;
}

void InitGDT(void)
{	
	SegmentDescriptor *gdt = (SegmentDescriptor *) ADR_GDT;
	
	int i;
	
	//GDT≥ı ºªØ
	for ( i = 0; i < 8192; i++ )
	{
        SetSegmentDescriptor(gdt + i, 0, 0, 0);
	}
    SetSegmentDescriptor(gdt + 1, 0xffffffff, 0x00000000, AR_DATA32_RW);
    SetSegmentDescriptor(gdt + 2, LIMIT_BOTPAK, ADR_BOTPAK, AR_CODE32_ER);
	load_gdtr(LIMIT_GDT, ADR_GDT);		//0xffff = (8192 * 8 - 1)
}
