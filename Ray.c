#include"Ray.h"

void freeRays(Ray *rays[], int arrSize)
{
    for (int i = 0; i < arrSize; i++)
    {
        free(rays[i]->pointA);
        free(rays[i]->pointB);
        free(rays[i]);
    }
}