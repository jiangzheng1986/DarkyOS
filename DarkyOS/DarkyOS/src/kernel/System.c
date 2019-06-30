#include "Kernel.h"

void SystemInitialize()
{
    InitGDT();

    InitIDT();

    InitMemoryManager();

    InitPIC();

    InitTimer();

    InitGraphics();

    InitKeyboard();

    InitMouse();

    InitFileSystem();

    io_out8(PIC0_IMR, 0xf8);
    io_out8(PIC1_IMR, 0xef);

    io_sti();
}

void DumpData(char *pszString, unsigned char *pbyData, int nCount)
{
    int i;
    int j = 0;
    char szBuffer[100];

    for (i = 0; i < nCount; i++)
    {
        unsigned char byByte = pbyData[i];

        sprintf(szBuffer, "%02X ", byByte);
        pszString[j] = szBuffer[0];
        pszString[j + 1] = szBuffer[1];
        pszString[j + 2] = szBuffer[2];
        j += 3;
    }

    pszString[j] = 0;
}

char g_szBuffer_PrintString[10000] = { 0 };
int g_nCharIndex_PrintString = 0;

void PrintString(char *pszString)
{
    int nLength = strlen(pszString);
    strcpy(g_szBuffer_PrintString + g_nCharIndex_PrintString, pszString);
    g_nCharIndex_PrintString += nLength;
}

void ClearString() 
{
    g_szBuffer_PrintString[0] = 0;
    g_nCharIndex_PrintString = 0;
}

int StringCompare(char *pszString1, char *pszString2)
{
    int i = 0;
    int nCompare = 0;
    while (1)
    {
        unsigned char byChar1 = pszString1[i];
        unsigned char byChar2 = pszString2[i];

        nCompare = byChar1 - byChar2;

        if (nCompare != 0)
        {
            break;
        }

        if (byChar1 == 0)
        {
            break;
        }

        if (byChar2 == 0)
        {
            break;
        }

        i++;
    }

    if (nCompare > 0)
    {
        return 1;
    }
    else if (nCompare < 0)
    {
        return -1;
    }
    else
    {
        return 0;
    }
}

char CharToUpper(char chChar)
{
    if (chChar >= 'a' && chChar <= 'z')
    {
        return chChar + ('A' - 'a');
    }
    else
    {
        return chChar;
    }
}

int StringCompareIgnoreCase(char *pszString1, char *pszString2)
{
    int i = 0;
    int nCompare = 0;
    while (1)
    {
        unsigned char byChar1 = CharToUpper(pszString1[i]);
        unsigned char byChar2 = CharToUpper(pszString2[i]);

        nCompare = byChar1 - byChar2;

        if (nCompare != 0)
        {
            break;
        }

        if (byChar1 == 0)
        {
            break;
        }

        if (byChar2 == 0)
        {
            break;
        }

        i++;
    }

    if (nCompare > 0)
    {
        return 1;
    }
    else if (nCompare < 0)
    {
        return -1;
    }
    else
    {
        return 0;
    }
}

void FillBlanks(char *pszString, int nTotalLength)
{
    int nLength = strlen(pszString);

    int i;
    for (i = nLength; i < nTotalLength; i++)
    {
        pszString[i] = ' ';
    }

    pszString[i] = 0;
}
