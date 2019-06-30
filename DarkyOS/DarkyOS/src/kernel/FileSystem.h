#ifndef FILE_SYSTEM_H
#define FILE_SYSTEM_H

typedef struct
{
    unsigned char szName[8];
    unsigned char szExt[3];
    unsigned char byType;
    char szReserved[10];
    unsigned short wTime;
    unsigned short wDate;
    unsigned short wClustno;
    unsigned int dwSize;
}FileInfo;

void InitFileSystem();

int GetFileCount();

void GetFileInfo(int i, char *pszFilename, unsigned int *pdwSize);

char *ReadFile(char *pszFilename);

#endif

