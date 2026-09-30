#ifndef PLAYER_H_
#define PLAYER_H_

#include <stdio.h>
#include <stdlib.h>
#include "mathFuncs.h"
#include "Ennemy.h"

#define PLAYER_SIZE 20
#define PLAYER_ORIGINAL_POS_X 620
#define PLAYER_ORIGINAL_POS_Y 410

typedef struct
{
    Vector2 position;
    float angle;
    int speed;
} Player;

Player *CreatePlayer(int speed);
void Shoot(Player *player, Ennemy **ennemy);
#endif
