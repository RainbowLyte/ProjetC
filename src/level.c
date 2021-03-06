#include <file.h>
#include <assert.h>
#include <stdlib.h>
#include <stddef.h> // NULL (macos)
#include <level.h>
#include <game.h> 
#include <map.h>
#include <list.h>

struct level {
	struct map** maps; // array of the level's maps
	short nb_maps; // nb maps of the level
	short cur_map; // the current map
};

struct level* level_get_level(struct game* game, int n_lvl, int n_map) {
	

	struct level* level = malloc(sizeof(*level));
	int map_count = 0;
	int nb_players = game_get_nb_player(game);

	if(nb_players >= 2) {
		level->nb_maps = sprite_get_nb_map_multi();
		level->cur_map = n_map;
		level->maps = malloc(sizeof(*level->maps) * level->nb_maps);
		for(int i = 0; i < level->nb_maps; i++)
			level->maps[i] = file_load_map_multi(i, nb_players);
	}
	else {
		for(int i = 0; i < 8; i++) {
			if(file_map_exist(n_lvl, i))
				map_count++;
			else
				i = 8;
		}

		if(map_count == 0)
			return NULL;

		level->nb_maps = map_count;
		level->cur_map = n_map;

		level->maps = malloc(sizeof(*level->maps) * level->nb_maps);

		for(int i = 0; i < level->nb_maps; i++) {
			level->maps[i] = file_load_map(n_lvl, i);
		}

		map_load_monsters(level->maps[0], game);
	} // end else

	window_resize(map_get_width(level_get_curr_map(level)) * SIZE_BLOC, map_get_height(level_get_curr_map(level)) * SIZE_BLOC + BANNER_HEIGHT + LINE_HEIGHT);

	return level;
}

void level_change_level(struct game* game, struct player* player, struct map* map) {
	assert(game);
	assert(player);
	assert(map);

	struct level* level = game_get_curr_level(game);
	struct list* bList = map_get_bombs(map);

	while(bList != NULL) {
		bList = list_cut_head(bList);
		player_inc_nb_bomb(player);
	}

	level_free(level);
	level = game_next_lvl(game);

	if(level == NULL) {
		printf("Level null, maybe there is no more level ?\n");
		return;
	}
	else {
		//printf("Next level\n");
	}
	players_from_map(game, level->maps[level->cur_map]);
	window_resize(map_get_width(level_get_curr_map(level)) * SIZE_BLOC, map_get_height(level_get_curr_map(level)) * SIZE_BLOC + BANNER_HEIGHT + LINE_HEIGHT);
}

void level_change_map(struct game* game, struct player* player, struct map* map, unsigned char num) {
	struct level* level = game_get_curr_level(game);
	struct list* bList = map_get_bombs(map);

	assert(game);
	assert(player);
	assert(map);

	if(num < level->nb_maps) {

		while(bList != NULL) {
			map_set_cell_type(map, list_get_x(bList), list_get_y(bList), CELL_EMPTY);
			bList = list_cut_head(bList);
			player_inc_nb_bomb(player);
		}
		map_set_bombs(map, NULL);
		player_reset_way_mov(player);
		level->cur_map = num;
		//printf("Next map\n");
		int x = player_get_x(player);
		int y = player_get_y(player);
		players_from_map(game, level->maps[level->cur_map]);
		if(player_get_x(player) == 0 && player_get_y(player) == 0) {
			player_set_x(player, x);
			player_set_x(player, y);
		}
		map_load_monsters(level->maps[level->cur_map], game);
		window_resize(map_get_width(level_get_curr_map(level)) * SIZE_BLOC, map_get_height(level_get_curr_map(level)) * SIZE_BLOC + BANNER_HEIGHT + LINE_HEIGHT);
	}
}

void level_set_cur_map_struct(struct level* level, int cur_map, struct map* map) {
	assert(level);
	assert(map);
	level->maps[cur_map] = map;
}

struct map* level_get_curr_map(struct level* level) {
	return level->maps[level->cur_map];
}

void level_set_cur_map(struct level* level, int i) {
	level->cur_map = i;
}

int level_get_curr_nb_map(struct level* level) {
	assert(level);
	return(level->cur_map);
}

struct map* level_get_map(struct level* level, int num) {
	assert(num <= level->nb_maps);
	return level->maps[num];
}

void level_free(struct level* level) {
	for (int i = 0; i < level->nb_maps; i++)
		map_free(level->maps[i]);
	free(level->maps);
	free(level);
}

void level_display(struct level* level) {
	map_display(level->maps[level->cur_map], 0, 0);
}
