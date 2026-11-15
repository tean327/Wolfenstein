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

void ShootPlayer(Player* p, Ennemy* e)
{
    Vector2 centerOfEnnemy = (Vector2){ e->position->X + ENNEMY_SIZE/2, e->position->Y + ENNEMY_SIZE/2 };
    if(VectorNorm(centerOfEnnemy.X, centerOfEnnemy.Y, p->position.X, p->position.Y)
     <= VectorNorm(centerOfEnnemy.X, centerOfEnnemy.Y, centerOfEnnemy.X + e->distanceDeTir, centerOfEnnemy.Y + e->distanceDeTir)
        && e->timeSinceLastShoot >= 2/*Si ca fait plus de 2 sec qu'il a pas tiré il peut*/)
    {
        printf("%p SHOOT THE PLAYER\n", e);
        p->pv--;
        e->timeSinceLastShoot = 0.;
    }
}