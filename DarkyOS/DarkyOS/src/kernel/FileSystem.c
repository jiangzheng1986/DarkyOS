
#include "Kernel.h"

int *g_pFAT = NULL;

int *ReadFAT();

void InitFileSystem()
{
    g_pFAT = ReadFAT();
}

FileInfo *GetRootDirectory()
{
    FileInfo *pFileInfo = (FileInfo *)(ADR_DISKIMG + 0x002600);

    return pFileInfo;
}

void GetFileName(FileInfo *pFileInfo, char *pszName)
{
    int i;
    for (i = 0; i < 8; i++)
    {
        char chChar = pFileInfo->szName[i];
        if (chChar == ' ')
        {
            break;
        }

        pszName[i] = chChar;
    }

    pszName[i] = 0;
}

void GetFileExtension(FileInfo *pFileInfo, char *pszExtension)
{
    int i;
    for (i = 0; i < 8; i++)
    {
        char chChar = pFileInfo->szExt[i];
        if (chChar == ' ')
        {
            break;
        }

        pszExtension[i] = chChar;
    }

    pszExtension[i] = 0;
}

void GetFileFilename(FileInfo *pFileInfo, char *pszFullName)
{
    char szName[1000];
    char szExtension[1000];

    GetFileName(pFileInfo, szName);
    GetFileExtension(pFileInfo, szExtension);
    sprintf(pszFullName, "%s.%s", szName, szExtension);
}

FileInfo *FindFileInDirectory(char *pszFilename)
{
    char szFilename[1000];

    FileInfo *pFileInfo = GetRootDirectory();

    int i;

    for (i = 0; i < 224; i++)
    {
        if (pFileInfo->szName[0] == 0)
        {
            break;
        }

        GetFileFilename(pFileInfo, szFilename);

        if (StringCompareIgnoreCase(szFilename, pszFilename) == 0)
        {
            return pFileInfo;
        }

        pFileInfo++;
    }

    return NULL;
}

int GetFileCount()
{
    FileInfo *pFileInfo = GetRootDirectory();

    int i;

    int nFileCount = 0;

    for (i = 0; i < 224; i++)
    {
        if (pFileInfo->szName[0] == 0)
        {
            break;
        }

        nFileCount++;

        pFileInfo++;
    }

    return nFileCount;
}

void GetFileInfo(int i, char *pszFilename, unsigned int *pdwSize)
{
    FileInfo *pFileInfo = GetRootDirectory();

    int nFileCount = GetFileCount();

    FileInfo *pFileInfo1 = NULL;

    pszFilename[0] = 0;

    *pdwSize = 0;

    if (i < 0 || i >= nFileCount)
    {
        return;
    }

    pFileInfo1 = pFileInfo + i;

    GetFileFilename(pFileInfo1, pszFilename);

    *pdwSize = pFileInfo1->dwSize;
}

int *ReadFAT()
{
    int *pFAT = (int *)MemoryAlloc4K(4 * 2880);

    //4320

    unsigned char *pFAT_Raw = (unsigned char *)(ADR_DISKIMG + 0x000200);

    int i = 0;
    int j = 0;

    for (i = 0; i < 2880; i += 2)
    {
        unsigned char byByte1 = pFAT_Raw[j + 0];
        unsigned char byByte2 = pFAT_Raw[j + 1];
        unsigned char byByte3 = pFAT_Raw[j + 2];
        pFAT[i + 0] = (byByte1 | (byByte2 << 8)) & 0xfff;
        pFAT[i + 1] = ((byByte2 >> 4) | (byByte3 << 4)) & 0xfff;
        j += 3;
    }

    return pFAT;
}

char *ReadFile(char *pszFilename)
{
    FileInfo *pFileInfo = FindFileInDirectory(pszFilename);

    if (pFileInfo == NULL)
    {
        return NULL;
    }

    char *pImage = (char *)(0x003e00 + ADR_DISKIMG);

    unsigned int dwSize = pFileInfo->dwSize;

    char *pFileContent = (char *)MemoryAlloc4K(dwSize + 1);

    char *pFileContent1 = pFileContent;

    unsigned int dwLeft = dwSize;

    unsigned short wClustno = pFileInfo->wClustno;
    
    while (1)
    {
        if (dwLeft == 0)
        {
            break;
        }

        unsigned int dwToRead = MIN(dwLeft, 512);

        memcpy(pFileContent1, pImage + wClustno * 512, dwToRead);

        dwLeft -= dwToRead;
        pFileContent1 += dwToRead;
        wClustno = g_pFAT[wClustno];
    }

    pFileContent[dwSize] = 0;

    return pFileContent;
}
