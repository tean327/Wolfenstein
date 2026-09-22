#ifndef FILELOADER_H_
#define FILELOAEDER_H_
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "Librairies/include/glad/glad.h"

typedef struct
{
    unsigned char r, g, b;
} Pixel;

typedef struct
{
    unsigned char header[54];
    int dataPos;
    int imageSize;
    int width;
    int height;
    int maxValue;
    unsigned char *data;
    GLuint textureID;
} BMP;

BMP *LoadBMP(char *pFilename);
#endif