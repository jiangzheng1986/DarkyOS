#include "Kernel.h"

void LoadImage(Image *pImage, char *pszFilename)
{
    char *pFileContent = ReadFile(pszFilename);
    
    pImage->pFileContent = pFileContent;
    pImage->nImageWidth = *(int *)(pFileContent + 0);
    pImage->nImageHeight = *(int *)(pFileContent + 4);
    pImage->pImageData = (unsigned char *)(pFileContent + 12);
}

void ImageDraw(Image *pImage, int nX, int nY)
{
    if (pImage)
    {
        DrawImage(nX, nY, pImage->nImageWidth, pImage->nImageHeight, pImage->pImageData);
    }
}

void ImageFree(Image *pImage)
{
    //MemoryFree4K(pImage->pFileContent);
}
