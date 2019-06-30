#include "Kernel.h"

char g_szBuffer[10000];

void PrintSystemInfo()
{
    SystemTime stLocalTime = { 0 };
    char chKeyInput = ' ';
    char szKeyInput[3];

    GetLocalTime(&stLocalTime);

    chKeyInput = KeyCodeToChar(g_byKeyCode);
    if (chKeyInput == 0)
    {
        chKeyInput = ' ';
    }

    szKeyInput[0] = chKeyInput;
    szKeyInput[1] = g_byKeyCode >= 128 ? '0' : '1';
    szKeyInput[2] = 0;

    sprintf(g_szBuffer, "DateTime: 20%02d-%02d-%02d %02d:%02d:%02d KeyCode:%d KeyInput:\"%s\" TimerCounter:%d L:%d R:%d C:%d\r\n",
        stLocalTime.byYear, stLocalTime.byMonth, stLocalTime.byDay,
        stLocalTime.byHour, stLocalTime.byMinute, stLocalTime.bySecond,
        g_byKeyCode, szKeyInput,
        g_dwTimerCounter,
        g_bLeftMouseDown, g_bRightMouseDown, g_bCenterMouseDown);

    PrintString(g_szBuffer);
}

void PrintRootDirectory()
{
    char szFilename[1000];
    unsigned int dwSize = 0;
    char szFileInfo[1000];

    int i;

    int nCount = GetFileCount();

    for (i = 0; i < nCount; i++)
    {
        GetFileInfo(i, szFilename, &dwSize);

        FillBlanks(szFilename, 16);

        sprintf(szFileInfo, "%s%d\n", szFilename, dwSize);

        PrintString(szFileInfo);
    }
}

void HariMain(void)
{
    int nSpan = 5;
    int nScreenWidth = 0;
    int nScreenHeight = 0;  
    char *pFileContent1 = NULL;
    Image Image1;

    SystemInitialize();

    nScreenWidth = GetScreenWidth();
    nScreenHeight = GetScreenHeight();

    pFileContent1 = ReadFile("Test.txt");

    LoadImage(&Image1, "Dog.tex");

    //PlayTest();

    FloppyTest();

    while(1)
    { 
        ClearScreen(RGB(128, 128, 255));

        ImageDraw(&Image1, 100, 50);

        ClearString();

        FloppyTest2();

        //PrintSystemInfo();

        //PrintRootDirectory();

        //PrintString(pFileContent1);

        DrawString(RGB(255, 0, 0), nSpan, nSpan, nScreenWidth - nSpan * 2, nScreenHeight - nSpan * 2, g_szBuffer_PrintString);

        FillRectangle(RGB(0, 255, 0), g_nMouseX, g_nMouseY, 16, 16);
        
        SwapBuffer();
    }
}
