#include <assert.h>
#include <time.h>
#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include <player.h>
#include <sprite.h>
#include <window.h>
#include <misc.h>
#include <constant.h>
#include <unistd.h>

struct player {
	int x, y;
	enum direction current_direction;
	int nb_bombs;
	int current_level;
};


struct player* player_init(int bomb_number) {
	struct player* player = malloc(sizeof(*player));
	if (!player)
		error("Memory error");

	player->current_direction = SOUTH;
	player->nb_bombs = bomb_number;
	player->current_level=0;

	return player;
}

void player_free(struct player* player) {
	assert(player);
	free(player);
}

int player_get_x(struct player* player) {
	assert(player != NULL);
	return player->x;
}

int player_get_y(struct player* player) {
	assert(player != NULL);
	return player->y;
}

void player_inc_current_level(struct player* player) {
	assert(player);
	player->current_level +=1;
}

void player_set_current_way(struct player* player, enum direction way) {
	assert(player);
	player->current_direction = way;
}

int player_get_nb_bomb(struct player* player) {
	assert(player);
	return player->nb_bombs;
}

int player_get_current_level(struct player* player) {
	assert(player);
	return player->current_level;
}

void player_inc_nb_bomb(struct player* player) {
	assert(player);
	player->nb_bombs += 1;
}

void player_dec_nb_bomb(struct player* player) {
	assert(player);
	player->nb_bombs -= 1;
}

void player_from_map(struct player* player, struct map* map) {
	assert(player);
	assert(map);

	int i, j;
	for (i = 0; i < map_get_width(map); i++) {
	  for (j = 0; j < map_get_height(map); j++) {	
	    if (map_get_cell_type(map, i, j) == CELL_PLAYER) {
	      player->x = i;
	      player->y = j;
	    }
	  }
	}
}

static int player_move_aux(struct player* player, struct map* map, int x, int y) {

	if (!map_is_inside(map, x, y))
		return 0;

	switch (map_get_cell_type(map, x, y)) {
	case CELL_SCENERY:
		return 0;
		break;

	case CELL_BOX:
		return 0;
		break;

	case CELL_BONUS:
		break;

	case CELL_MONSTER:
		break;

	case CELL_PLAYER:
		break;

	case CELL_DOOR:
		player_inc_current_level(player);
		break;

	default:
		break;
	}

	// Player has moved
	return 1;
}

/*int player_move(struct player* player, struct map* map) {
	int x = player->x;
	int y = player->y;
	int move = 0;

	switch (player->current_direction) {
	case NORTH:
	  if (map_get_cell_type(map, x, y-1)==CELL_SCENERY){ //Don't move if scenery_type
	    move=0;
	  }
	  else if (map_get_cell_type(map, x, y-1)==CELL_BOX) {//Case of box
	    if ((map_get_cell_type(map, x, y-2)==CELL_EMPTY)&&(y-1>0)){//Move if following cell empty and not out of map
	      player->y--;
	      move=1;
	      map_set_cell_type(map,x,y-2,CELL_BOX);
	      window_display_image(sprite_get_box(), x, y-2);
	    }}
	  else if (y-1<0){ //Deal with map size
	    move=0;
	  }
	  else if (player->y!=0) {
		  if (player_move_aux(player,map,x,y-1)){
		  	player->y--;
	       	move = 1;
		}
	  }
		break;

	case SOUTH:
	   if (map_get_cell_type(map, x, y+1)==CELL_SCENERY){//Don't move if scenery_type
	    move=0;
	   }
	   else if (map_get_cell_type(map, x, y+1)==CELL_BOX) {//Case of box
	     if ((map_get_cell_type(map, x, y+2)==CELL_EMPTY)&&(y+1<map_get_height(map)-1)){//Move if following cell empty and not out of map
	      player->y++;
	      move=1;
	      map_set_cell_type(map,x,y+2,CELL_BOX);
	      window_display_image(sprite_get_box(), x, y+2);
	    }}
	   else if (y+1>map_get_height(map)-1){//Deal with map size
	    move=0;
	  }
	   else if (player->y!=map_get_height(map)-1) {
	   		  if (player_move_aux(player,map,x,y+1)){
	   		  	player->y++;
	   	       	move = 1;
	   		}
	   	  }
		break;

	case WEST:
	  if (map_get_cell_type(map, x-1,y)==CELL_SCENERY){//Don't move if scenery_type
	    move=0;
	   }
	  else  if (map_get_cell_type(map, x-1, y)==CELL_BOX) {//Case of box
	    if ((map_get_cell_type(map, x-2, y)==CELL_EMPTY)&&(x-1>0)){//Move if following cell empty and not out of map	      
	      player->x--;
	      move=1;
	      map_set_cell_type(map,x-2,y,CELL_BOX);
	      window_display_image(sprite_get_box(), x-2, y);
	    }}
	  else if (x-1<0){//Deal with map size
	    move=0;
	  }
	  else if (player->x!=0) {
	  		  if (player_move_aux(player,map,x,x-1)){
	  		  	player->x--;
	  	       	move = 1;
	  		}
	  	  }
		break;

	case EAST:
	  if (map_get_cell_type(map, x+1, y)==CELL_SCENERY){//Don't move if scenery_type
	    move=0;
	   }
	  else if (map_get_cell_type(map, x+1, y)==CELL_BOX) {//Case of box
	    if ((map_get_cell_type(map, x+2, y)==CELL_EMPTY)&&(x+1<map_get_width(map)-1)){//Move if following cell empty and not out of map	     
	      player->x++;
	      move=1;
	      map_set_cell_type(map,x+2,y,CELL_BOX);
	      window_display_image(sprite_get_box(), x+2, y);
	    }}
	  else if (x+1>map_get_width(map)-1){//Deal with map size
	    move=0;
	  }
	  else if (player->x!=map_get_width(map)-1) {
	  	   		  if (player_move_aux(player,map,x,x+1)){
	  	   		  	player->x++;
	  	   	       	move = 1;
	  	   		}
	  	   	  }
		break;
	}

	if (move) {
		map_set_cell_type(map, x, y, CELL_EMPTY);
		map_set_cell_type(map, player->x, player->y, CELL_PLAYER);
	}
	return move;
}*/

int player_move(struct player* player, struct map* map) {
	int x = player->x;
	int y = player->y;
	int move = 0;

	switch (player->current_direction) {
	case NORTH:
		if (map_get_cell_type(map , player->x , player->y - 1) == CELL_BOX ){
			if (map_get_cell_type(map , player->x , player->y -2) == CELL_EMPTY){
				if (player->y - 1 != 0){
					map_set_cell_type(map , player->x , player->y - 2 , CELL_BOX ) ;
					map_set_cell_type(map , player->x , player->y - 1 , CELL_EMPTY ) ;
				}
			}
		}
		if (player->y!=0){
			if (player_move_aux(player, map, x, y - 1)){
				player->y--;
				move = 1;
			}
		}
		break;

	case SOUTH:
		if (map_get_cell_type(map , player->x , player->y + 1 ) == CELL_BOX ){
					if (map_get_cell_type(map , player->x , player->y + 2) == CELL_EMPTY){
						if (player->y + 1 != map_get_height(map)-1 ){
							map_set_cell_type(map , player->x , player->y + 2  , CELL_BOX ) ;
							map_set_cell_type(map , player->x , player->y + 1  , CELL_EMPTY ) ;
						}
					}
		}
		if (player->y!=map_get_height(map)-1){
			if (player_move_aux(player, map, x, y + 1)) {
				player->y++;
				move = 1;
			}
		}
		break;

	case WEST:
		if (map_get_cell_type(map , player->x - 1, player->y ) == CELL_BOX ){
							if (map_get_cell_type(map , player->x - 2 , player->y) == CELL_EMPTY){
								if (player->x - 1 != 0){
									map_set_cell_type(map , player->x - 2, player->y , CELL_BOX ) ;
									map_set_cell_type(map , player->x - 1 , player->y  , CELL_EMPTY ) ;
								}
							}
		}
		if (player->x!=0){
			if (player_move_aux(player, map, x - 1, y)) {
				player->x--;
				move = 1;
			}
		}
		break;

	case EAST:
		if (map_get_cell_type(map , player->x + 1, player->y ) == CELL_BOX ){
							if (map_get_cell_type(map , player->x + 2 , player->y) == CELL_EMPTY){
								if (player->x + 1 != 0){
									map_set_cell_type(map , player->x + 2, player->y , CELL_BOX ) ;
									map_set_cell_type(map , player->x + 1 , player->y  , CELL_EMPTY ) ;
								}
							}
		}
		if (player->x!=map_get_width(map)-1){
			if (player_move_aux(player, map, x + 1, y)) {
				player->x++;
				move = 1;
			}
		}
		break;
	}



	if (move) {
		map_set_cell_type(map, x, y, CELL_EMPTY);
		map_set_cell_type(map, player->x, player->y, CELL_PLAYER);
	}
	return move;
}



void player_display(struct player* player) {
	assert(player);
	window_display_image(sprite_get_player(player->current_direction),
			player->x * SIZE_BLOC, player->y * SIZE_BLOC);
}
