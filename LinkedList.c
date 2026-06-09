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
    int posX;
    int posY;
    int sizeX;
    int sizeY;
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

void AddAtEndOfListInt(int value, ListInt *head)
{
    ListInt *newNode = (ListInt *)malloc(sizeof(ListInt));
    if (newNode == NULL)
    {
        printf("Unable to allocate memory for new node\n");
        // Exit will end the program with the code 1
        exit(1);
    }
    newNode->VALUE = value;
    newNode->next = NULL;

    if (head->next == NULL)
    {
        head->next = newNode;
    }
    else
    {
        ListInt *current = head;
        while (current->next != NULL)
        {
            current = current->next;
        }
        current->next = newNode;
        return;
    }
}

/// @brief This function is used to only check if a value is inside a list
/// @param head the head of the list
/// @param value the value you want to check if it's inside
/// @return Returns 1 if the value is inside else 0 if it's not
int IsInsideListInt(ListInt *head, int value)
{
    ListInt *searchNode = head;
    while (searchNode->next != NULL)
    {
        if (searchNode->VALUE == value)
        {
            return 1;
        }
        searchNode = searchNode->next;
    }
    return 0;
}

/// @brief This function is used to get the node from a list based on a parameter value it can also be used to check if a value is inside a list but it's quite to big for only that
/// @param head the head of the list
/// @param value the value where you want to get the node based of
/// @return Returns the node if the value is inside else null if it's not
ListInt *GetListIntNodeFromValue(ListInt *head, int value)
{
    ListInt *searchNode = head;
    while (searchNode->next != NULL)
    {
        if (searchNode->VALUE == value)
        {
            return searchNode;
        }
    }
    return NULL;
}

void FreeListIntMemory(ListInt *head)
{
    ListInt *tmp;
    while (head != NULL)
    {
        tmp = head;
        head = head->next;
        free(tmp);
    }
}

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