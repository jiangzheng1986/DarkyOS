#ifndef IMAGE_H
#define IMAGE_H

typedef struct
{
    char *pFileContent;
    int nImageWidth;
    int nImageHeight;
    unsigned char *pImageData;
}Image;

void LoadImage(Image *pImage, char *pszFilename);

void ImageDraw(Image *pImage, int nX, int nY);

void ImageFree(Image *pImage);

#endif
