#ifndef DATASTRUCTURES_H_
#define DATASTRUCTURES_H_
#include"Ennemy.h"
#include <stdlib.h>

#pragma region List

typedef struct nodeWall
{
    Vector2 *position;
    Vector2 *size;
} Wall;

typedef struct ListWallNode
{
    Wall *VALUE;
    struct ListWallNode *next;
} ListWall;

typedef struct ListEnnemyNode
{
    Ennemy* VALUE;
    struct ListEnnemyNode* previous;
    struct ListEnnemyNode* next;
} ListEnnemy;

#pragma endregion

void AddAtEndOfListWall(Wall *value, ListWall *head);
int IsInsideListWall(ListWall *head, Wall *value);
void FreeListWallMemory(ListWall *head);

void AddAtEndOfListEnnemy(Ennemy *value, ListEnnemy *head);
int IsInsideListEnnemy(ListEnnemy *head, Ennemy *value);
void FreeListEnnemyMemory(ListEnnemy *head);
#endif