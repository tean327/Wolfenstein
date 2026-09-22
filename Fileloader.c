#include "FileLoader.h"

BMP *LoadBMP(char *pFilename)
{
    BMP *ppm = (BMP *)malloc(sizeof(BMP));
    if (!ppm)
    {
        printf("Could not allocate memory for BMP\n");
        return NULL;
    }

    FILE *file = fopen(pFilename, "rb");
    if (!file)
    {
        printf("Image could not be opened\n");
        return NULL;
    }

    // On vérifie si la taille du header est bien 54
    if (fread(ppm->header, 1, 54, file) != 54)
    {
        printf("Not a correct BMP file\n");
        return NULL;
    }
    // Un fichier BMP commencera toujours par les lettres b et m donc on check
    if (ppm->header[0] != 'B' || ppm->header[1] != 'M')
    {
        printf("Not a correct BMP file\n");
        return NULL;
    }

    ppm->dataPos = *(int *)&(ppm->header[0x0A]);
    ppm->imageSize = *(int *)&(ppm->header[0x22]);
    ppm->width = *(int *)&(ppm->header[0x12]);
    ppm->height = *(int *)&(ppm->header[0x16]);

    if (ppm->imageSize == 0)
        ppm->imageSize = ppm->width * ppm->height * 3; // 3 : one byte for each Red, Green and Blue component
    if (ppm->dataPos == 0)
        ppm->dataPos = 54; // The BMP header is done that way

    ppm->data = malloc(sizeof(char) * ppm->imageSize);
    if (!ppm->data)
    {
        printf("Could not allocate memory for BMP data\n");
        return NULL;
    }

    // Read the actual data from the file into the buffer
    fread(ppm->data, 1, ppm->imageSize, file);

    // Everything is in memory now, the file can be closed
    fclose(file);

    GLuint textureID;
    glGenTextures(1, &textureID);

    glBindTexture(GL_TEXTURE_2D, textureID);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, ppm->width, ppm->height, 0, GL_BGRA, GL_UNSIGNED_BYTE, ppm->data);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

    ppm->textureID = textureID;
    return ppm;
}