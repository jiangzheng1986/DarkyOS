
#include "Kernel.h"

FloppySturct g_stFloppyType = 
{
    2880,       //dwSize
    18,         //dwSector
    2,          //dwHead
    80,         //dwTrack
    0,          //dwStretch
    0x1B,       //byGap
    0x00,       //byRate
    0xCF,       //bySpec1
};

unsigned int g_dwCurrentFloppyDriver = 0;

BOOL FDC_OutputData(char data)
{
    int i;
    for (i = 0; i < 10000; i++)
    {
        unsigned char status = io_in8(FDC_STATUS);

        if ((status & FDC_STATUS_DATA_READY) &&
            !(status & FDC_STATUS_DIR_FDD_TO_CPU))
        {
            io_out8(FDC_DATA, data);
            return TRUE;
        }
    }

    return FALSE;
}

void FDC_SetupParameters()
{
    FDC_OutputData(FDC_CMD_SPECIFY);

    unsigned char HUT = 15;   //15 * 32ms = 480ms
    unsigned char SRT = 12;   //12 * 2ms = 24ms

    FDC_OutputData((SRT << 4) | HUT);

    unsigned char NDMA = 0;   //DMA Mode
    unsigned char HLT = 3;    //3 * 4ms = 12ms

    FDC_OutputData((HLT << 1) | NDMA);
}

void FDC_Init()
{
    int i;

    io_out8(FDC_DOR, FDC_DOR_DMA_INTERRUPT);

    for (i = 0; i < 100; i++)
    {
        io_nop();
    }

    io_out8(FDC_DOR, FDC_DOR_DMA_INTERRUPT | FDC_DOR_RESET);

    io_out8(FDC_DCR, FDC_DCR_500KBPS);

    FDC_SetupParameters();
}

void FDC_PrintStatus()
{
    char buffer[1000];

    unsigned char status = io_in8(FDC_STATUS);

    sprintf(buffer, "Status: %02x\n", status);

    PrintString(buffer);
}

void FDC_Recalibrate()
{
    //软盘重新校正

    FDC_OutputData(FDC_CMD_RECALIBRATE);

    FDC_OutputData(g_dwCurrentFloppyDriver);
}

void FDC_Seek(unsigned int dwSeekTrack)
{
    //寻道

    FDC_OutputData(FDC_CMD_SEEK);

    FDC_OutputData(g_dwCurrentFloppyDriver);

    FDC_OutputData(dwSeekTrack);
}

void FDC_SetDMA(unsigned char *pBuffer, unsigned char byCommand)
{
    unsigned int dwCount;
    unsigned long ulAddress = (unsigned long)pBuffer;

    io_cli();

    io_out8(10, 4 | 2);       //Mask DMA 2

    unsigned char byDMA_Command = 0;
    
    if (byCommand == FDC_CMD_READ_MT_MF_SK)
    {
        byDMA_Command = DMA_READ;
    }
    else
    {
        byDMA_Command = DMA_WRITE;
    }

    io_out8(12, byDMA_Command);

    io_out8(11, byDMA_Command);

    //send address

    io_out8(4, (unsigned char)ulAddress);

    ulAddress >>= 8;

    io_out8(4, (unsigned char)ulAddress);

    ulAddress >>= 8;

    io_out8(0x81, (unsigned char)ulAddress);

    //send count

    dwCount = 512 - 1;

    io_out8(5, (unsigned char)dwCount);
    
    dwCount >>= 8;

    io_out8(5, (unsigned char)dwCount);

    //activate DMA 2

    io_out8(10, 0 | 2);

    io_sti();
}

void FDC_ReadSector_NotBlocked(unsigned int dwSectorNumber, unsigned char *pBuffer)
{
    unsigned int dwHead;
    unsigned int dwTrack;
    unsigned int dwBlock;
    unsigned int dwSector;
    unsigned int dwSeekTrack;

    if (pBuffer == NULL)
    {
        return;
    }

    int nTotalSectors = g_stFloppyType.dwHead *
        g_stFloppyType.dwTrack * g_stFloppyType.dwSector;

    if (dwSectorNumber >= nTotalSectors)
    {
        return;
    }

    dwSector = dwSectorNumber % g_stFloppyType.dwSector + 1;
    dwBlock = dwSectorNumber / g_stFloppyType.dwSector;
    dwTrack = dwBlock / g_stFloppyType.dwHead;
    dwHead = dwBlock % g_stFloppyType.dwHead;
    dwSeekTrack = dwTrack << g_stFloppyType.dwStretch;

    FDC_Recalibrate();

    FDC_Seek(dwSeekTrack);

    FDC_SetDMA(pBuffer, FDC_CMD_READ_MT_MF_SK);

    FDC_OutputData(FDC_CMD_READ_MT_MF_SK);

    FDC_OutputData(g_dwCurrentFloppyDriver);

    FDC_OutputData(dwTrack);

    FDC_OutputData(dwHead);

    FDC_OutputData(dwSector);   //start sector

    FDC_OutputData(2);          //sector size = 512

    FDC_OutputData(g_stFloppyType.dwSector);

    FDC_OutputData(g_stFloppyType.byGap);

    FDC_OutputData(0xFF);       //sector size
}

void FDC_ReadSector(unsigned int dwSectorNumber, unsigned char *pBuffer)
{
    unsigned int dwMagicNumber = 0x98999899;
    unsigned int *pBuffer1 = (unsigned int *)pBuffer;
    pBuffer1[127] = dwMagicNumber;

    FDC_ReadSector_NotBlocked(dwSectorNumber, pBuffer);

    //while (TRUE)
    {
        if (pBuffer1[127] != dwMagicNumber)
        {
            //break;
        }
    }
}

void FDC_ReadSectors(unsigned int dwStartSectorNumber, unsigned int dwSectors, unsigned char *pBuffer)
{
    unsigned int i;
    for (i = 0; i < dwSectors; i++)
    {
        FDC_ReadSector(dwStartSectorNumber + i, pBuffer + i * 512);
    }
}

char g_szBuffer_Floppy[2048];

void FDC_DumpBuffer(unsigned char *pBuffer)
{
    DumpData(g_szBuffer_Floppy, pBuffer, 512);
    PrintString(g_szBuffer_Floppy);
    PrintString("\n");
}

unsigned char g_szFloppyBuffer[512];

void FloppyTest()
{
    FDC_Init();

    FDC_ReadSector(0, g_szFloppyBuffer);

    FDC_ReadSector(1, g_szFloppyBuffer);

    FDC_DumpBuffer(g_szFloppyBuffer);
}

void FloppyTest2()
{
    FDC_DumpBuffer(g_szFloppyBuffer);
}