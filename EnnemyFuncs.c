#include"EnnemyFuncs.h"

void KillEnnemy(ListEnnemy** node, GLFWwindow *window2D, GLFWwindow *window3D)
{
    (*node)->previous->next = (*node)->next;
    if((*node)->next)
        (*node)->next->previous = (*node)->previous;
    FreeEnnemy(&(*node)->VALUE, window2D, window3D);
    free(*node);
}

void Draw2DEnnemies(ListEnnemy *ennemyHead)
{
    ListEnnemy *crrnt = ennemyHead->next;
    while(crrnt)
    {
        glBindTexture(GL_TEXTURE_2D, crrnt->VALUE->texture->textureID);
        glUseProgram(crrnt->VALUE->program);
        glBindVertexArray(crrnt->VALUE->VAO);
        glDrawArrays(GL_TRIANGLES, 0, 6);
        glBindVertexArray(0);

        crrnt = crrnt->next;
    }
}