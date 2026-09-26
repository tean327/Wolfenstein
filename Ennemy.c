#include "Ennemy.h"

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