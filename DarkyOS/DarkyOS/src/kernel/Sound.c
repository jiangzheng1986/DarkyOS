#include "Kernel.h"

void Beep(int bEnable, int nFrequency)
{
    int i;
    if (!bEnable)
    {
        i = io_in8(0x61);
        io_out8(0x61, i & 0x0d);
    }
    else
    {
        i = io_in8(0x61);
        io_out8(0x61, i & 0x0d);

        i = 1193180 / nFrequency;
        io_out8(0x43, 0xb6);
        io_out8(0x42, i & 0xff);
        io_out8(0x42, i >> 8);
        i = io_in8(0x61);
        io_out8(0x61, (i | 0x03) & 0x0f);
    }
}

int nToneTable[] = {
    262, 294, 330, 349, 392, 440, 494,
    523, 587, 659, 698, 784, 880, 988,
    1046, 1175, 1318, 1397, 1568, 1760, 1967,
};

int dummy2 = 0;

void WaitForAWhile(int repeat)
{
    int i = 0;
    int j = 0;
    int count = 5 * repeat;
    for (j = 0; j < count; j++)
    {
        for (i = 0; i < 100000000; i++)
        {
            dummy2++;
        }
    }
}

void PlayTone(int nTone1, int nTone2, int repeat)
{
    if (nTone1 >= 1 && nTone1 <= 3 &&
        nTone2 >= 1 && nTone2 <= 7)
    {
        int nToneIndex = (nTone1 - 1) * 7 + (nTone2 - 1);
        int nFrequency = nToneTable[nToneIndex];

        Beep(1, nFrequency);
    }
    else
    {
        Beep(0, 0);
    }

    WaitForAWhile(repeat);
}

void PlayTest()
{
    PlayTone(2, 1, 1);
    PlayTone(2, 2, 1);
    PlayTone(2, 3, 1);
    PlayTone(2, 1, 1);
    PlayTone(0, 0, 1);

    PlayTone(2, 1, 1);
    PlayTone(2, 2, 1);
    PlayTone(2, 3, 1);
    PlayTone(2, 1, 1);
    PlayTone(0, 0, 1);

    PlayTone(2, 3, 1);
    PlayTone(2, 4, 1);
    PlayTone(2, 5, 2);
    PlayTone(0, 0, 1);

    PlayTone(2, 3, 1);
    PlayTone(2, 4, 1);
    PlayTone(2, 5, 2);
    PlayTone(0, 0, 1);

    PlayTone(2, 5, 1);
    PlayTone(2, 6, 1);
    PlayTone(2, 5, 1);
    PlayTone(2, 4, 1);
    PlayTone(2, 3, 2);
    PlayTone(2, 1, 2);
    PlayTone(0, 0, 1);

    PlayTone(2, 5, 1);
    PlayTone(2, 6, 1);
    PlayTone(2, 5, 1);
    PlayTone(2, 4, 1);
    PlayTone(2, 3, 2);
    PlayTone(2, 1, 2);
    PlayTone(0, 0, 1);

    PlayTone(2, 2, 2);
    PlayTone(1, 5, 2);
    PlayTone(2, 1, 2);
    PlayTone(0, 0, 1);

    PlayTone(2, 2, 2);
    PlayTone(1, 5, 2);
    PlayTone(2, 1, 2);
    PlayTone(0, 0, 1);
}
