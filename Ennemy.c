#include "Ennemy.h"
#include <stdio.h>

Ennemy *CreateEnnemy(int pPv, int posX, int posY, GLuint pProgram, int height, int width)
{
    Ennemy *ennemy = (Ennemy *)malloc(sizeof(Ennemy));
    if (!ennemy)
    {
        return NULL;
    }

    ennemy->position = (Vector2 *)malloc(sizeof(Vector2));
    ennemy->size = (Vector2 *)malloc(sizeof(Vector2));

    if (!ennemy->position || !ennemy->size)
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
        ConvertToOpenGLX(ennemy->size->X, width), ConvertToOpenGLY(ennemy->size->Y, height), 0.0f};

    GLfloat color[18] = {
        1.0f, 0.0f, 0.0f,
        1.0f, 0.0f, 0.0f,
        1.0f, 0.0f, 0.0f,
        1.0f, 0.0f, 0.0f,
        1.0f, 0.0f, 0.0f,
        1.0f, 0.0f, 0.0f};

    // Copy of vertices into the ennemy->vertices array
    // We can't assign it directly because ennemy->vertices has already been created
    for (int i = 0; i < 18; i++)
    {
        ennemy->vertices[i] = vertices[i];
        ennemy->UV[i] = color[i];
    }
    ennemy->program = pProgram;

    glUseProgram(pProgram);
    glGenVertexArrays(1, &ennemy->VAO);
    glBindVertexArray(ennemy->VAO);

    glGenBuffers(1, &ennemy->VBOvert);
    glBindBuffer(GL_ARRAY_BUFFER, ennemy->VBOvert);
    glBufferData(GL_ARRAY_BUFFER, 18 * sizeof(GLfloat), ennemy->vertices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void *)0);
    glEnableVertexAttribArray(0);

    glGenBuffers(1, &ennemy->VBO_UV);
    glBindBuffer(GL_ARRAY_BUFFER, ennemy->VBO_UV);
    glBufferData(GL_ARRAY_BUFFER, 18 * sizeof(GLfloat), ennemy->UV, GL_STATIC_DRAW);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, (void *)0);

    glEnableVertexAttribArray(1);

    return ennemy;
}

void Generate3DVAOVOBSEnemy(Ennemy *ennemy)
{
    glUseProgram(ennemy->program);
    glGenVertexArrays(1, &ennemy->VAO3D);
    glBindVertexArray(ennemy->VAO3D);

    glGenBuffers(1, &ennemy->VBOvert3D);
    glBindBuffer(GL_ARRAY_BUFFER, ennemy->VBOvert3D);
    glBufferData(GL_ARRAY_BUFFER, 18 * sizeof(GLfloat), ennemy->vertices3D, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void *)0);
    glEnableVertexAttribArray(0);

    glGenBuffers(1, &ennemy->VBO_UV3D);
    glBindBuffer(GL_ARRAY_BUFFER, ennemy->VBO_UV3D);
    glBufferData(GL_ARRAY_BUFFER, 18 * sizeof(GLfloat), ennemy->UV3D, GL_STATIC_DRAW);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, (void *)0);

    glEnableVertexAttribArray(1);
}

void FreeEnnemy(Ennemy **ennemy)
{
    glDeleteVertexArrays(1, &(*ennemy)->VAO);
    glDeleteBuffers(1, &(*ennemy)->VBOvert);
    glDeleteBuffers(1, &(*ennemy)->VBO_UV);

    printf("Freedat\n");
    free((*ennemy)->position);
    free((*ennemy)->size);
    free(*ennemy);
    *ennemy = NULL;
}

int Draw3DEnnemies(Ennemy *ennemy, Ray **rays, int playerPosX, int playerPosY, float playerAngle)
{
    if (!ennemy)
        return 0;

    int vertexcount = 0;

    float projPlaneDist = (WIDTH / 2.0f) / tanf(FOV / 2.0f);
    float zoom = 0.02f;

    float spriteX = (ennemy->position->X + ennemy->size->X) / 2.0f;
    float spriteY = (ennemy->position->Y + ennemy->size->Y) / 2.0f;

    float dx = spriteX - playerPosX;
    float dy = spriteY - playerPosY;
    float dist = sqrtf(dx * dx + dy * dy);

    float spriteAngle = atan2f(-dy, dx);
    float relAngle = spriteAngle - playerAngle;

    while (relAngle > (float)PI)
        relAngle -= 2.0f * (float)PI;
    while (relAngle < -(float)PI)
        relAngle += 2.0f * (float)PI;

    if (fabsf(relAngle) > (FOV / 2.0f))
        return 0;

    float perpDist = dist * cosf(relAngle);
    if (perpDist < 20.0f)
        perpDist = 20.0f;

    float spriteScreenX = WIDTH * (relAngle + FOV / 2.0f) / FOV;
    float spriteHeight = projPlaneDist / (perpDist * zoom);
    float spriteWidth = spriteHeight;

    float xLeft = spriteScreenX - spriteWidth / 2.0f;
    float xRight = spriteScreenX + spriteWidth / 2.0f;
    float yTop = HEIGHT / 2.0f - spriteHeight / 2.0f;
    float yBottom = yTop + spriteHeight;

    int colStart = (int)floorf(xLeft * NUMBER_OF_RAYS / WIDTH);
    int colEnd = (int)ceilf(xRight * NUMBER_OF_RAYS / WIDTH);
    if (colStart < 0)
        colStart = 0;
    if (colEnd >= NUMBER_OF_RAYS)
        colEnd = NUMBER_OF_RAYS - 1;

    for (int col = colStart; col <= colEnd; col++)
    {
        if (perpDist >= rays[col]->norme)
            continue;

        float colXLeft = fmaxf(xLeft, (float)WIDTH * col / NUMBER_OF_RAYS);
        float colXRight = fminf(xRight, (float)WIDTH * (col + 1) / NUMBER_OF_RAYS);
        if (colXRight <= colXLeft)
            continue;

        int j = vertexcount * 3;

        float xS[6] = {colXLeft, colXLeft, colXRight, colXLeft, colXRight, colXRight};
        float yS[6] = {yTop, yBottom, yBottom, yTop, yTop, yBottom};

        for (int v = 0; v < 6; v++)
        {
            ennemy->vertices3D[j + v * 3 + 0] = ConvertToOpenGLX(xS[v], WIDTH);
            ennemy->vertices3D[j + v * 3 + 1] = ConvertToOpenGLY(yS[v], HEIGHT);
            ennemy->vertices3D[j + v * 3 + 2] = 0.0f;

            /* couleur unie rouge en attendant la texture */
            ennemy->UV3D[j + v * 3 + 0] = 1.0f;
            ennemy->UV3D[j + v * 3 + 1] = 0.0f;
            ennemy->UV3D[j + v * 3 + 2] = 0.0f;
        }

        vertexcount += 6;
    }

    if (vertexcount > 0)
    {
        glBindBuffer(GL_ARRAY_BUFFER, ennemy->VBOvert3D);
        glBufferData(GL_ARRAY_BUFFER, vertexcount * 3 * sizeof(GLfloat),
                     ennemy->vertices3D, GL_DYNAMIC_DRAW);

        glBindBuffer(GL_ARRAY_BUFFER, ennemy->VBO_UV3D);
        glBufferData(GL_ARRAY_BUFFER, vertexcount * 3 * sizeof(GLfloat),
                     ennemy->UV3D, GL_DYNAMIC_DRAW);
    }

    return vertexcount;
}