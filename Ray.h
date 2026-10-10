#ifndef RAY_H_
#define RAY_H_

#include"mathFuncs.h"

typedef struct nodeRay
{
    Vector2 *pointA;
    Vector2 *pointB;
    float norme;
    float rayAngle;
} Ray;

#endif