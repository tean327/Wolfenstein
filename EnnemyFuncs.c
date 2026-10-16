#include"EnnemyFuncs.h"

void KillEnnemy(ListEnnemy** node)
{
    (*node)->previous->next = (*node)->next;
    if((*node)->next)
        (*node)->next->previous = (*node)->previous;
    FreeEnnemy(&(*node)->VALUE);
    free(*node);
}

void Draw2DEnnemies(ListEnnemy *ennemyHead)
{
    ListEnnemy *crrnt = ennemyHead->next;
    while(crrnt)
    {
        glUseProgram(crrnt->VALUE->program);
        glBindVertexArray(crrnt->VALUE->VAO);
        glDrawArrays(GL_TRIANGLES, 0, 6);
        glBindVertexArray(0);

        crrnt = crrnt->next;
    }
}