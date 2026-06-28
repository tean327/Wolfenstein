#ifndef MATH_FUNCTIONS_H_
#define MATH_FUNCTIONS_H_
#include <math.h>
#include "LinkedList.h"

/// @brief Convert the x of a coordante from the screen plan to the openGL plan
/// @return the converted coordonate x
float ConvertToOpenGLX(float pCoord, int screenWidth)
{
    return (pCoord - screenWidth / 2) / (screenWidth / 2);
}

/// @brief Convert the y of a coordante from the screen plan to the openGL pla
/// @return the converted coordonate y
float ConvertToOpenGLY(float pCoord, int screenHeight)
{
    return (pCoord - screenHeight / 2) / (screenHeight / 2);
}

/// @brief Multiply two matrices
/// @param result Where you store the result
/// @param a
/// @param b
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

/// @brief Used to create a translation matrice to the vector
/// @param result where you want to store the translation matrice
/// @param vector the vector where you want to translate to
void MultiplyMatriceToVector(float result[], Vector2 *vector)
{
    for (int i = 0; i < 16; i++)
        result[i] = 0.0f;
    result[0] = 1.0f;
    result[5] = 1.0f;
    result[10] = 1.0f;
    result[15] = 1.0f;

    result[12] = vector->X;
    result[13] = vector->Y;
    result[14] = 0.0f;
}

/// @brief Return the distance from the ray to the wall, also take care of the different angle, if not the wall will not be a square
/// @return
float CalculRayDistance(float xA, float yA, float xB, float yB, float rayAngle, float playerAngle)
{
    float normalRayDistance = sqrt(pow(xB - xA, 2) + pow(yB - yA, 2));
    return normalRayDistance * cos(rayAngle - playerAngle);
}

#endif