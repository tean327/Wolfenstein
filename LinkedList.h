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

typedef struct ListIntNode
{
    int VALUE;
    struct ListIntNode *next;
} ListInt;

typedef struct ListWallNode
{
    Wall *VALUE;
    struct ListWallNode *next;
} ListWall;

#pragma endregion

// Int list functions
void AddAtEndOfListInt(int value, ListInt *head);
int IsInsideListInt(ListInt *head, int value);
ListInt *GetListIntNodeFromValue(ListInt *head, int value);
void FreeListIntMemory(ListInt *head);

void AddAtEndOfListWall(Wall *value, ListWall *head);
int IsInsideListWall(ListWall *head, Wall *value);
void FreeListWallMemory(ListWall *head);
#endif