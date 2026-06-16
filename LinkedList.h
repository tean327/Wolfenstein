#ifndef DATASTRUCTURES_H_
#define DATASTRUCTURES_H_
#include <stdlib.h>

#pragma region List
typedef struct nodeVector2
{
    double X;
    double Y;
} Vector2;

typedef struct nodeWall
{
    Vector2 *position;
    Vector2 *size;
} Wall;

typedef struct nodeRay
{
    Vector2 *pointA;
    Vector2 *pointB;
    float norme;
    float rayAngle;
} Ray;

typedef struct ListWallNode
{
    Wall *VALUE;
    struct ListWallNode *next;
} ListWall;

#pragma endregion

void AddAtEndOfListWall(Wall *value, ListWall *head);
int IsInsideListWall(ListWall *head, Wall *value);
void FreeListWallMemory(ListWall *head);
#endif