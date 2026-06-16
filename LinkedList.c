#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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

typedef struct ListWallNode
{
    Wall *VALUE;
    struct ListWallNode *next;
} ListWall;

void AddAtEndOfListWall(Wall *value, ListWall *head)
{
    ListWall *newNode = (ListWall *)malloc(sizeof(ListWall));
    if (newNode == NULL)
    {
        printf("Unable to allocate memory for new node\n");
        exit(1);
    }
    newNode->VALUE = value;
    newNode->next = NULL;

    if (head->VALUE == NULL)
    {
        head->VALUE = value;
        free(newNode);
        return;
    }
    if (head->next == NULL)
    {
        head->next = newNode;
    }
    else
    {
        ListWall *current = head;
        while (current->next != NULL)
        {
            current = current->next;
        }
        current->next = newNode;
        printf("");
        return;
    }
}

/// @brief This function is used to only check if a value is inside a list
/// @param head the head of the list
/// @param value the value you want to check if it's inside
/// @return Returns 1 if the value is inside else 0 if it's not
int IsInsideListWall(ListWall *head, Wall *value)
{
    ListWall *searchNode = head;
    while (searchNode->next != NULL)
    {
        if ((*searchNode).VALUE->position == (*value).position && (*searchNode).VALUE->size == (*value).size)
        {
            return 1;
        }
        searchNode = searchNode->next;
    }
    return 0;
}

void FreeListWallMemory(ListWall *head)
{
    ListWall *tmp;
    while (head != NULL)
    {
        tmp = head;
        head = head->next;
        free(tmp);
    }
}