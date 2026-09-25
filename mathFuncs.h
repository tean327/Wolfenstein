#ifndef MATH_FUNCTIONS_H_
#define MATH_FUNCTIONS_H_
#include <math.h>
#include "LinkedList.h"

#define PI 3.141592653589793

/// @brief Convert the x of a coordante from the screen plan to the openGL plan
/// @return the converted coordonate x
float ConvertToOpenGLX(float pCoord, int screenWidth);

/// @brief Convert the y of a coordante from the screen plan to the openGL pla
/// @return the converted coordonate y
float ConvertToOpenGLY(float pCoord, int screenHeight);

/// @brief Convert the x of a coordante from the screen plan to the openGL plan
/// @return the converted coordonate x
float ConvertOpenGLXToPixel(float pCoord, int screenWidth);

/// @brief Convert the y of a coordante from the screen plan to the openGL pla
/// @return the converted coordonate y
float ConvertOpenGLYToPixel(float pCoord, int screenHeight);

/// @brief Multiply two matrices
/// @param result Where you store the result
/// @param a
/// @param b
void MultiplyMatrices(float result[], float a[], float b[]);

/// @brief Used to create a translation matrice to the vector
/// @param result where you want to store the translation matrice
/// @param vector the vector where you want to translate to
void MultiplyMatriceToVector(float result[], Vector2 *vector);

/// @brief Return the distance from the ray to the wall, also take care of the different angle, if not the wall will not be a square
/// @return
float CalculRayDistance(float xA, float yA, float xB, float yB, float rayAngle, float playerAngle);
#endif