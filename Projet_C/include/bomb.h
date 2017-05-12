#ifndef BOMB_H_
#define BOMB_H_

#include <player.h>
#include <map.h>
#include <game.h>

struct bomb;

struct bomb* bomb_init();

struct bomb *new_bomb(struct player *player,struct bomb *bomb);

void put_a_bomb(struct game* game);

#endif /* BOMB_H_ */
