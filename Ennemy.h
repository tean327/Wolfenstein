#ifndef ENNEMY_H_
#define ENNEMY_H_

#include "Librairies/include/glad/glad.h"
#include "mathFuncs.h"
#include <stdlib.h>

#define ENNEMY_SIZE 50

typedef struct
{
    int pv;
    Vector2 *position;
    Vector2 *size;
    GLfloat vertices[6 * 3];
    GLfloat UV[6 * 3];
    unsigned int VBOvert;
    unsigned int VBO_UV;
    unsigned int VAO;

    unsigned int VBOvert3D;
    unsigned int VBO_UV3D;
    unsigned int VAO3D;
    GLuint program;
    GLfloat vertices3D[NUMBER_OF_RAYS * 6 * 3];
    GLfloat UV3D[NUMBER_OF_RAYS * 6 * 3];
} Ennemy;

Ennemy *CreateEnnemy(int pPv, int posX, int posY, GLuint pProgram, int height, int width);
void Generate3DVAOVOBSEnemy(Ennemy *ennemy);
void FreeEnnemy(Ennemy **ennemy);

int Draw3DEnnemies(Ennemy *Ennemy, Ray **rays, int playerPosX, int playerPosY, float playerAngle);
#endif