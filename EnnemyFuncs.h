#ifndef ENNEMY_FUNCS_H_
#define ENNEMY_FUNCS_H_

#include"Ennemy.h"
#include"Player.h"
#include"Ray.h"

void KillEnnemy(ListEnnemy** node, GLFWwindow *window2D, GLFWwindow *window3D);
void Draw2DEnnemies(ListEnnemy *ennemyHead);
void ShootPlayer(Player* p, Ennemy* e);

#endif
