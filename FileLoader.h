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
    int width;
    int height;
    int maxValue;
    Pixel **data;
    GLuint textureID;
} PPM;

PPM *LoadPPM(char *pFilename);
GLuint loadP3ppm(FILE *file, PPM *ppm);
GLuint loadP6ppm(FILE *file, PPM *ppm);
int CheckPPM(FILE *file);
#endif