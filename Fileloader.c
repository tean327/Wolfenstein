#include "FileLoader.h"

PPM *LoadPPM(char *pFilename)
{
    PPM *ppm = (PPM *)malloc(sizeof(PPM));
    if (!ppm)
    {
        printf("Could not allocate memory for ppm\n");
        return NULL;
    }

    FILE *file = fopen(pFilename, "r");
    if (!file)
    {
        printf("Failed to open file %s\n", pFilename);
        return NULL;
    }

    // Vérifie si le fichier commence par P6 ou P3
    int ppmValue = CheckPPM(file);
    if (ppmValue == -1)
    {
        fclose(file);
        printf("Not a ppm file\n");
        return NULL;
    }

    int c;

    while ((c = fgetc(file)) == '#')
        while (fgetc(file) != '\n')
            ;

    ungetc(c, file);

    if (fscanf(file, "%d %d", &ppm->width, &ppm->height) != 2)
    {
        fclose(file);
        return NULL;
    }

    ppm->maxValue;
    fscanf(file, "%d", &ppm->maxValue);
    fgetc(file);

    ppm->data = malloc(ppm->height * sizeof(Pixel *));
    if (!ppm->data)
    {
        fclose(file);
        printf("Could not allocate memory for the data\n");
        return NULL;
    }

    for (int i = 0; i < ppm->height; i++)
    {
        ppm->data[i] = malloc(ppm->width * sizeof(Pixel));
        if (!ppm->data[i])
        {
            printf("Could not allocate memory for a Pixel\n");
            return NULL;
        }
    }

    if (ppmValue == 0)
    {
        ppm->textureID = loadP3ppm(file, ppm);
        if (ppm->textureID < 0)
            return NULL;
    }
    else
    {
        ppm->textureID = loadP6ppm(file, ppm);
        if (ppm->textureID < 0)
            return NULL;
    }

    return ppm;
}

GLuint loadP3ppm(FILE *file, PPM *ppm)
{
    for (int i = 0; i < ppm->height; i++)
    {
        for (int j = 0; j < ppm->width; j++)
        {
            int rgb[3];
            if (fscanf(file, "%d %d %d ", &rgb[0], &rgb[1], &rgb[2]) != 3)
            {
                fclose(file);
                printf("ERROR IN FILE\n");
                exit(1);
            }
            ppm->data[i][j].r = (unsigned char)rgb[0];
            ppm->data[i][j].g = (unsigned char)rgb[1];
            ppm->data[i][j].b = (unsigned char)rgb[2];
        }
    }

    fclose(file);

    GLuint textureID;
    glGenTextures(1, &textureID);

    glBindTexture(GL_TEXTURE_2D, textureID);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, ppm->width, ppm->height, 0, GL_BGR, GL_UNSIGNED_BYTE, ppm->data);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glGenerateMipmap(GL_TEXTURE_2D);

    return textureID;
}

GLuint loadP6ppm(FILE *file, PPM *ppm)
{
    printf("PPM6\n");

    for (int i = 0; i < ppm->height; i++)
    {
        for (int j = 0; j < ppm->width; j++)
        {
            unsigned char rgb[3];
            if (fread(rgb, sizeof(unsigned char), 3, file) != 3)
            {
                fclose(file);
                printf("ERROR IN FILE\n");
                exit(1);
            }
            ppm->data[i][j].r = rgb[0];
            ppm->data[i][j].g = rgb[1];
            ppm->data[i][j].b = rgb[2];
        }
    }
    fclose(file);

    GLuint textureID;
    glGenTextures(1, &textureID);

    glBindTexture(GL_TEXTURE_2D, textureID);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, ppm->width, ppm->height, 0, GL_BGR, GL_UNSIGNED_BYTE, ppm->data);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glGenerateMipmap(GL_TEXTURE_2D);

    return textureID;
}

int CheckPPM(FILE *file)
{
    char buffer[3];
    fgets(buffer, 3, file);
    fgetc(file);
    if (strcmp(buffer, "P3") == 0)
        return 0;
    if (strcmp(buffer, "P6") == 0)
        return 1;
    return -1;
}