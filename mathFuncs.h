#ifndef MATH_FUNCTIONS_H_
#define MATH_FUNCTIONS_H_
#include <math.h>
#include "LinkedList.h"

float ConvertToOpenGLX(float pCoord, int screenWidth)
{
    return (pCoord - screenWidth / 2) / (screenWidth / 2);
}

float ConvertToOpenGLY(float pCoord, int screenHeight)
{
    return (pCoord - screenHeight / 2) / (screenHeight / 2);
}

void MultiplyMatrices(float result[], float a[], float b[])
{
    for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            float sum = 0.0f;
            for (int k = 0; k < 4; k++)
            {
                sum += (a[i * 4 + k] * b[k * 4 + j]);
            }
            result[i * 4 + j] = sum;
        }
    }
}

void MultiplyMatriceToVector(float result[], float a[], Vector2 *b)
{
    for (int i = 0; i < 16; i++)
        result[i] = 0.0f;
    result[0] = 1.0f;
    result[5] = 1.0f;
    result[10] = 1.0f;
    result[15] = 1.0f;

    result[12] = b->X;
    result[13] = b->Y;
    result[14] = 0.0f;
}

float CalculRayDistance(float xA, float yA, float xB, float yB, float rayAngle, float playerAngle)
{
    float normalRayDistance = sqrt(pow(xB - xA, 2) + pow(yB - yA, 2));
    return normalRayDistance * cos(rayAngle - playerAngle);
}

#endif