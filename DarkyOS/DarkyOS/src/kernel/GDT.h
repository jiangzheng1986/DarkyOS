#ifndef GDT_H
#define GDT_H

typedef struct
{
    short limit_low, base_low;
    char base_mid, access_right;
    char limit_high, base_high;
}SegmentDescriptor;

void InitGDT(void);

#endif

