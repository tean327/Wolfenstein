#include "UtilFuncs.h"


int KeyPressed(int key, GLFWwindow* window, GLFWwindow* window3D)
{
    return glfwGetKey(window, key) || glfwGetKey(window3D, key);
}