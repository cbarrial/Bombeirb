#include <assert.h>
#include <time.h>
#include <SDL/SDL.h>
#include <game.h>
#include <misc.h>
#include <window.h>
#include <sprite.h>
#include <bomb.h>
#include <player.h>

struct game {
	struct map** maps;       // the game's map
	short levels;            // nb maps of the game
	short current;
	struct player* player;
	struct bomb* bomb;
};

struct game* game_new(void) {
	sprite_load(); // load sprites into process memory

	struct game* game = malloc(sizeof(*game));
	game->maps = malloc(sizeof(struct game));
	game->current = 0;
	game->maps[0] = map_get_default();
	game->maps[1] = map_get_map(1);
	game->maps[2] = map_get_map(2);
	game->levels = 2;
	game->bomb=bomb_init();


	game->player = player_init(1);
	player_from_map(game->player, game->maps[0]); // get x,y of the player on the first map

	return game;
}

void game_free(struct game* game) {
	assert(game);

	player_free(game->player);
	for (int i = 0; i < game->levels; i++)
		map_free(game->maps[i]);
}

short game_set_current(struct game* game){
	assert(game);
	return game->current=player_get_current_level(game_get_player(game));
}

struct map* game_get_current_map(struct game* game) {
	assert(game);
	return game->maps[game->current];
}


struct player* game_get_player(struct game* game) {
	assert(game);
	return game->player;
}

struct bomb* game_get_bomb(struct game* game) {
	assert(game);
	return game->bomb;
}

void game_banner_display(struct game* game) {
	assert(game);

	struct map* map = game_get_current_map(game);

	int y = (map_get_height(map)) * SIZE_BLOC;
	for (int i = 0; i < map_get_width(map); i++)
		window_display_image(sprite_get_banner_line(), i * SIZE_BLOC, y);

	int white_bloc = ((map_get_width(map) * SIZE_BLOC) - 6 * SIZE_BLOC) / 4;
	int x = white_bloc;
	y = (map_get_height(map) * SIZE_BLOC) + LINE_HEIGHT;
	window_display_image(sprite_get_banner_life(), x, y);

	x = white_bloc + SIZE_BLOC;
	window_display_image(sprite_get_number(2), x, y);

	x = 2 * white_bloc + 2 * SIZE_BLOC;
	window_display_image(sprite_get_banner_bomb(), x, y);

	x = 2 * white_bloc + 3 * SIZE_BLOC;
	window_display_image(
			sprite_get_number(player_get_nb_bomb(game_get_player(game))), x, y);

	x = 3 * white_bloc + 4 * SIZE_BLOC;
	window_display_image(sprite_get_banner_range(), x, y);

	x = 3 * white_bloc + 5 * SIZE_BLOC;
	window_display_image(sprite_get_number(1), x, y);
}

void game_display(struct game* game) {
	assert(game);

	window_clear();

	game_banner_display(game);
	map_display(game_get_current_map(game));
	player_display(game->player);

	window_refresh();
}


static short input_keyboard(struct game* game) {
	SDL_Event event;
	struct player* player = game_get_player(game);
	struct bomb *bomb=game_get_bomb(game);
	struct map* map = game_get_current_map(game);

while (SDL_PollEvent(&event)) {
		switch (event.type) {
		case SDL_QUIT:
			return 1;
		case SDL_KEYDOWN:
			switch (event.key.keysym.sym) {
			case SDLK_ESCAPE:
				return 1;
			case SDLK_UP:
				player_set_current_way(player, NORTH);
				player_move(player, map);
				break;
			case SDLK_DOWN:
				player_set_current_way(player, SOUTH);
				player_move(player, map);
				break;
			case SDLK_RIGHT:
				player_set_current_way(player, EAST);
				player_move(player, map);
				break;
			case SDLK_LEFT:
				player_set_current_way(player, WEST);
				player_move(player, map);
				break;
			case SDLK_SPACE:
				new_bomb(player, bomb);
				break;
			default:
				break;
			}

			break;
		}
	}

	return 0;
}

int game_update(struct game* game) {
	if (input_keyboard(game))
		return 1; // exit game
	game_set_current(game);
	put_a_bomb(game);
	return 0;
}
