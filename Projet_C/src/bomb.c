#include <SDL/SDL_image.h>
#include <SDL/SDL.h>
#include <assert.h>
#include <time.h>
#include <player.h>
#include <sprite.h>
#include <window.h>
#include <misc.h>
#include <constant.h>
#include <unistd.h>
#include <game.h>
#include <bomb.h>



struct bomb {
	int x, y;
	int timer;
	struct bomb *next_bomb;
};

struct bomb* bomb_init(void) {
	struct bomb* bomb = malloc(sizeof(*bomb));
	if (!bomb)
		error("Memory error");

	bomb->x =-1;
	bomb->y =-1;
	bomb->timer = SDL_GetTicks();
	bomb->next_bomb = NULL;

	return bomb;
}


struct bomb *new_bomb(struct player *player,struct bomb *bomb){

	if (bomb==NULL){
		bomb->next_bomb=NULL;
		bomb->x=player_get_x(player);
		bomb->y=player_get_y(player);
		bomb->timer=SDL_GetTicks();
	}
	else {
		while(bomb->next_bomb!=NULL){
			bomb=bomb->next_bomb;}

			struct bomb* new_bomb=malloc(sizeof(*new_bomb));
			bomb->next_bomb=new_bomb;
			new_bomb->x=player_get_x(player);
			new_bomb->y=player_get_y(player);
			new_bomb->timer=SDL_GetTicks();
			new_bomb->next_bomb=NULL;



	}

	return bomb;

}



/*void s_sleep(int s){
	time_t  start_time=0;
	time_t current_time=0;
	start_time=time(NULL);
	while (current_time-start_time+1<=s){
		current_time=time(NULL);
	}
}*/
void put_a_bomb(struct game* game){
	struct bomb *bomb=game_get_bomb(game);
	struct map *map=game_get_current_map(game);
	int time;



	time = SDL_GetTicks();



	while (bomb!=NULL){
		int execution_speed =time-(bomb->timer);

		if ((0<execution_speed) && (execution_speed< 1000)){
			map_set_cell_type(map,bomb->x, bomb->y, CELL_BOMB4);

		}

		if ((1000<execution_speed) && (execution_speed< 2000)){
			map_set_cell_type(map,bomb->x, bomb->y, CELL_BOMB3);


		}

		if ((2000<execution_speed) && (execution_speed< 3000)){
			map_set_cell_type(map,bomb->x, bomb->y, CELL_BOMB2);


		}

		if ((3000<execution_speed) && (execution_speed< 4000)){
			map_set_cell_type(map,bomb->x, bomb->y, CELL_BOMB1);
		}

		if ((4000<execution_speed) && (execution_speed< 4500)){
			map_set_cell_type(map,bomb->x, bomb->y, CELL_BOMB5);
			 if (map_get_cell_type(map, bomb->x, bomb->y-1)==CELL_SCENERY){
				 map_set_cell_type(map,bomb->x, bomb->y-1, CELL_BOMB5);
			 }
			map_set_cell_type(map,bomb->x, bomb->y+1, CELL_BOMB5);
			map_set_cell_type(map,bomb->x-1, bomb->y, CELL_BOMB5);
			map_set_cell_type(map,bomb->x+1, bomb->y, CELL_BOMB5);
		}
		if (4500<execution_speed){
					map_set_cell_type(map,bomb->x, bomb->y, CELL_EMPTY);
					map_set_cell_type(map,bomb->x, bomb->y-1, CELL_EMPTY);
					map_set_cell_type(map,bomb->x, bomb->y+1, CELL_EMPTY);
					map_set_cell_type(map,bomb->x-1, bomb->y, CELL_EMPTY);
					map_set_cell_type(map,bomb->x+1, bomb->y, CELL_EMPTY);
				}

		bomb=bomb->next_bomb;

	}

}



/*void wait(int seconds)
{
    clock_t endwait;
    endwait = clock () + seconds * CLOCKS_PER_SEC ;
    while (clock() < endwait) {}
    }*/
/*
void put_a_bomb(struct player* player, struct map* map,int i){
  int x= player->x;
  int y= player->y;

  if (i==1){
	  map_set_cell_type(map,x+1, y, CELL_BOMB);
	  window_display_image(sprite_get_bomb(BOMB_1), x+1, y);
  }



  if (i==2){
	  map_set_cell_type(map,x+1, y, CELL_BOX);
	  window_display_image(sprite_get_box(), x+1, y);
  }



  if(i==3){
	  map_set_cell_type(map,x+1, y, CELL_BOMB);
	  window_display_image(sprite_get_bomb(BOMB_3), x+1, y);
  }



  if (i==4){
	  map_set_cell_type(map,x+1, y, CELL_BOX);
	  window_display_image(sprite_get_box(), x+1, y);
  }
}
*/

