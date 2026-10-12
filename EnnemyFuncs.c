#include"EnnemyFuncs.h"


void KillEnnemy(ListEnnemy** node)
{
    (*node)->previous = (*node)->next;
    FreeEnnemy(&(*node)->VALUE);
    free(*node);
}