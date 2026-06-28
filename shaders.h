#ifndef SHADERS_H
#define SHADERS_H
#include <stdio.h>
#include <string.h>

const char *ReadFragment()
{
    FILE *file = fopen("shader/fragment.frag", "r");
    char buffer[200];
    char vertex[200];
    int i = 0;
    while (fgets(buffer, sizeof(buffer), file))
    {
        fread(vertex, 1, sizeof(buffer), file);
    }
    fclose(file);
    const char *a = vertex;
    return a;
}

const char *ReadVertex()
{
    FILE *file = fopen("shader/vertex.vert", "rb");
    char buffer[200];
    char vertex[200];
    int i = 0;
    while (fgets(buffer, sizeof(buffer), file))
    {
        fread(vertex, 1, sizeof(buffer), file);
    }
    fclose(file);

    const char *a = vertex;
    return a;
}
#endif
