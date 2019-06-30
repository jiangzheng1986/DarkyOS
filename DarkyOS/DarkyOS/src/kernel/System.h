#ifndef SYSTEM_H
#define SYSTEM_H

#define BOOL int
#define TRUE 1
#define FALSE 0

#define MIN(a, b) (((a) < (b)) ? (a) : (b))
#define MAX(a, b) (((a) > (b)) ? (a) : (b))

void SystemInitialize();

void DumpData(char *pszString, unsigned char *pbyData, int nCount);

extern char g_szBuffer_PrintString[];

void PrintString(char *pszString);

void ClearString();

int StringCompare(char *pszString1, char *pszString2);

char CharToUpper(char chChar);

int StringCompareIgnoreCase(char *pszString1, char *pszString2);

void FillBlanks(char *pszString, int nTotalLength);

#endif
