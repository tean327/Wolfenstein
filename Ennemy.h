#ifndef ENNEMY_H_
#define ENNEMY_H_

#include"Librairies/include/glad/glad.h"
#include"mathFuncs.h"
#include<stdlib.h>

#define ENNEMY_SIZE 50

typedef struct 
{
    int pv;
    Vector2* position;
    Vector2* size;
    GLfloat vertices[6*3];
    GLfloat UV[6*3];
    unsigned int VBOvert;
    unsigned int VBO_UV;
} Ennemy;

Ennemy* CreateEnnemy(int pPv, int posX, int posY, int height, int width);

#endif