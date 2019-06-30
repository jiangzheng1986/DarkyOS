#ifndef FLOPPY_H
#define FLOPPY_H

#define FDC_DOR                    0x3F2
#define FDC_STATUS                 0x3F4
#define FDC_DATA                   0x3F5
#define FDC_DIR                    0x3F7
#define FDC_DCR                    0x3F7

#define FDC_DOR_DRIVER_SELECT0     0x01
#define FDC_DOR_DRIVER_SELECT1     0x02
#define FDC_DOR_RESET              0x04
#define FDC_DOR_DMA_INTERRUPT      0x08
#define FDC_DOR_MOTOR_ENABLE0      0x10
#define FDC_DOR_MOTOR_ENABLE1      0x20
#define FDC_DOR_MOTOR_ENABLE2      0x40
#define FDC_DOR_MOTOR_ENABLE3      0x80

#define FDC_STATUS_DRIVER_A_BUSY   0x01
#define FDC_STATUS_DRIVER_B_BUSY   0x02
#define FDC_STATUS_DRIVER_C_BUSY   0x04
#define FDC_STATUS_DRIVER_D_BUSY   0x08
#define FDC_STATUS_CONTROLLER_BUSY 0x10
#define FDC_STATUS_NOT_DMA         0x20
#define FDC_STATUS_DIR_FDD_TO_CPU  0x40
#define FDC_STATUS_DATA_READY      0x80

#define FDC_DCR_500KBPS            0x00
#define FDC_DCR_300KBPS            0x01
#define FDC_DCR_250KBPS            0x02

#define FDC_CMD_SPECIFY            0x03
#define FDC_CMD_RECALIBRATE        0x07
#define FDC_CMD_SEEK               0x0F
#define FDC_CMD_READ_MT_MF_SK      0xE6
#define FDC_CMD_WRITE_MT_MF        0xC5
#define FDC_CMD_SENSEI             0x08

#define DMA_READ                   0x46
#define DMA_WRITE                  0x4A

typedef struct
{
    unsigned int dwSize;
    unsigned int dwSector;
    unsigned int dwHead;
    unsigned int dwTrack;
    unsigned int dwStretch;
    unsigned char byGap;
    unsigned char byRate;
    unsigned char bySpec1;
}FloppySturct;

void FDC_Init();

void FDC_ReadSector(unsigned int dwSectorNumber, unsigned char *pBuffer);

void FloppyTest();

#endif

