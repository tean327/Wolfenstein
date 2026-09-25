#include "Ennemy.h"

Ennemy* CreateEnnemy(int pPv, int posX, int posY, int height, int width)
{
    Ennemy* ennemy = (Ennemy*)malloc(sizeof(Ennemy));
    if(!ennemy)
    {
        return NULL;
    }

    ennemy->position = (Vector2*)malloc(sizeof(Vector2));
    ennemy->size = (Vector2*)malloc(sizeof(Vector2));

    if(!ennemy->position || !ennemy->size)
        return NULL;
    
    ennemy->pv = pPv;

    ennemy->position->X = posX;
    ennemy->position->Y = posY;
    ennemy->size->X = posX + ENNEMY_SIZE;
    ennemy->size->Y = posY + ENNEMY_SIZE;

    GLfloat vertices[18] = {
        ConvertToOpenGLX(ennemy->position->X, width), ConvertToOpenGLY(ennemy->position->Y, height), 0.0f,
        ConvertToOpenGLX(ennemy->position->X, width), ConvertToOpenGLY(ennemy->size->Y, height), 0.0f, 
        ConvertToOpenGLX(ennemy->size->X, width), ConvertToOpenGLY(ennemy->size->Y, height), 0.0f,
        ConvertToOpenGLX(ennemy->position->X, width), ConvertToOpenGLY(ennemy->position->Y, height), 0.0f, 
        ConvertToOpenGLX(ennemy->size->X, width), ConvertToOpenGLY(ennemy->position->Y, height), 0.0f,
        ConvertToOpenGLX(ennemy->size->X, width), ConvertToOpenGLY(ennemy->size->Y, height), 0.0f
    };

    GLfloat color[18] = {
        1.0f, 0.0f, 0.0f,
        1.0f, 0.0f, 0.0f,
        1.0f, 0.0f, 0.0f,
        1.0f, 0.0f, 0.0f,
        1.0f, 0.0f, 0.0f,
        1.0f, 0.0f, 0.0f
    };

    //Copy of vertices into the ennemy->vertices array
    //We can't assign it directly because ennemy->vertices has already been created
    for(int i = 0; i < 18; i++)
    {
        ennemy->vertices[i] = color[i];
        ennemy->UV[i] = color[i];
    }

    return ennemy;
}