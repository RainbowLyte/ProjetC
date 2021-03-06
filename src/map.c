#include <SDL/SDL_image.h>
#include <assert.h> 
#include <stdio.h>
#include <time.h>

#include <map.h>
#include <constant.h>
#include <misc.h>
#include <sprite.h>
#include <window.h>
#include <list.h>
#include <monster.h>

struct map {
	int width;
	int height;
	char* grid;
	struct list* monstersList;
	struct list* bombsList;
};

#define CELL(i,j) (i +  map->width * j)

struct map* map_new(int width, int height)
{
	assert(width > 0 && height > 0);

	struct map* map = malloc(sizeof *map);
	if (map == NULL )
		error("map_new : malloc map failed");

	map->width = width;
	map->height = height;

	map->monstersList = list_new();
	map->bombsList = list_new();

	map->grid = malloc(height * width);
	if (map->grid == NULL) {
		error("map_new : malloc grid failed");
	}

	// Grid cleaning
	int i, j;
	for (i = 0; i < width; i++)
		for (j = 0; j < height; j++)
			map->grid[CELL(i,j)] = CELL_EMPTY;

	return map;
}

struct list* map_load_monsters(struct map* map, struct game* game) {
	int i, j;
	map->monstersList = list_delete(map->monstersList);
	for (j = 0; j < map->height; j++) {
		for (i = 0; i < map->width; i++) {
			switch(map->grid[CELL(i,j)]) {
			case CELL_MONSTER_NORMAL:
				monster_init(map, i, j, MONSTER_NORMAL);
				break;
			case CELL_MONSTER_ALIEN1:
				monster_init(map, i, j, MONSTER_ALIEN1);
				break;
			case CELL_MONSTER_ALIEN2:
				monster_init(map, i, j, MONSTER_ALIEN2);
				break;
			case CELL_MONSTER_ALIEN3:
				monster_init(map, i, j, MONSTER_ALIEN3);
				break;
			case CELL_MONSTER_ALIEN4:
				monster_init(map, i, j, MONSTER_ALIEN4);
				break;
			default:
				break;
			}
		}
	}
	return map->monstersList;
}

int map_is_inside(struct map* map, int x, int y)
{
	assert(map);

	if(x < 0 || y < 0 || x >= map->width || y >= map->height)
		return 0;

	return 1;
}

void map_free(struct map* map)
{
	if (map == NULL )
		return;
	if(map->bombsList != NULL) {
		struct list* bList = map->bombsList;
		while(bList != NULL) {
			if(list_get_data(bList) != NULL)
				free(list_get_data(bList));

			bList = list_cut_head(bList);
		}
	}
	if(map->monstersList != NULL) {
		struct list* mList = map->monstersList;
		while(mList != NULL) {
			if(list_get_data(mList) != NULL)
				free(list_get_data(mList));

			mList = list_cut_head(mList);
		}
	}

	free(map->grid);
	free(map);
}

int map_get_width(struct map* map)
{
	assert(map != NULL);
	return map->width;
}

int map_get_height(struct map* map)
{
	assert(map);
	return map->height;
}

enum cell_type map_get_cell_type(struct map* map, int x, int y)
{
	assert(map && map_is_inside(map, x, y));
	return map->grid[CELL(x,y)] & 15;
}

unsigned char map_get_cell_compose_type(struct map* map, int x, int y)
{
	assert(map && map_is_inside(map, x, y));
	return map->grid[CELL(x,y)];
}

void map_set_cell_type(struct map* map, int x, int y, enum cell_type type)
{
	assert(map && map_is_inside(map, x, y));
	map->grid[CELL(x,y)] = type;
}

struct list* map_get_monsters(struct map* map)
{
	assert(map);
	return map->monstersList;
}

struct list* map_get_bombs(struct map* map)
{
	assert(map);
	return map->bombsList;
}

void map_set_monsters(struct map* map, struct list* mList)
{
	assert(map);
	map->monstersList = mList;
}

void map_set_bombs(struct map* map, struct list* bList)
{
	assert(map);
	map->bombsList = bList;
}

void map_case_destroyed(struct game* game, struct map* map, int x, int y)
{
	assert(map && map_is_inside(map, x, y));
	
	if(map_get_cell_compose_type(map, x, y) >> 4 == 0) {

		int r = rand_ab(0, 99);

		if(game_get_nb_player(game) == 1) {
			if(0 <= r && r < 40)
				map_set_cell_type(map, x, y, CELL_EMPTY);
			else if( 40 <= r && r < 55 )
				map_set_cell_type(map, x, y, CELL_BONUS_RANGE_INC);
			else if( 55 <= r && r < 60 )
				map_set_cell_type(map, x, y, CELL_BONUS_RANGE_DEC);
			else if( 60 <= r && r < 75)
				map_set_cell_type(map, x, y, CELL_BONUS_BOMB_INC);
			else if( 75 <= r && r < 80 )
				map_set_cell_type(map, x, y, CELL_BONUS_BOMB_DEC);
			else if( 80 <= r && r < 90 )
				map_set_cell_type(map, x, y, CELL_BONUS_ROLLER);
			else if( 90 <= r && r < 95 ) {
					map_set_cell_type(map, x, y, CELL_EMPTY);
					monster_init(map,x, y, MONSTER_NORMAL);
			}
			else if( 95 <= r && r < 100 )
				map_set_cell_type(map, x, y, CELL_BONUS_LIFE);
		}
		else {
			if(0 <= r && r < 55)
				map_set_cell_type(map, x, y, CELL_EMPTY);
			else if( 55 <= r && r < 65 )
				map_set_cell_type(map, x, y, CELL_BONUS_ROLLER);
			else if( 65 <= r && r < 70 )
				map_set_cell_type(map, x, y, CELL_BONUS_LIFE);
			else if( 70 <= r && r < 80 )
				map_set_cell_type(map, x, y, CELL_BONUS_BOMB_INC);
			else if( 80 <= r && r < 90 ) {
					map_set_cell_type(map, x, y, CELL_BONUS_RANGE_INC);
			}
			else if( 90 <= r && r < 100 )
				map_set_cell_type(map, x, y, CELL_BONUS_RANGE_INC);
		}
	}
	else {
		switch(map_get_cell_compose_type(map, x, y) >> 4) {
		case BONUS_BOMB_INC:
			map_set_cell_type(map, x, y, CELL_BONUS_BOMB_INC);
			break;
		case BONUS_BOMB_DEC:
			map_set_cell_type(map, x, y, CELL_BONUS_BOMB_DEC);
			break;
		case BONUS_RANGE_INC:
			map_set_cell_type(map, x, y, CELL_BONUS_RANGE_INC);
			break;
		case BONUS_RANGE_DEC:
			map_set_cell_type(map, x, y, CELL_BONUS_RANGE_DEC);
			break;
		case BONUS_ROLLER:
			map_set_cell_type(map, x, y, CELL_BONUS_ROLLER);
			break;
		case BONUS_LIFE:
			map_set_cell_type(map, x, y, CELL_BONUS_LIFE);
			break;
		case BONUS_MONSTER:
			monster_init(map, x, y, MONSTER_NORMAL);
			map_set_cell_type(map, x, y, CELL_EMPTY);
			break;
		case BONUS_ALIEN1:
			monster_init(map, x, y, MONSTER_ALIEN1);
			map_set_cell_type(map, x, y, CELL_EMPTY);
			break;
		case BONUS_ALIEN2:
			monster_init(map, x, y, MONSTER_ALIEN2);
			map_set_cell_type(map, x, y, CELL_EMPTY);
			break;
		case BONUS_ALIEN3:
			monster_init(map, x, y, MONSTER_ALIEN3);
			map_set_cell_type(map, x, y, CELL_EMPTY);
			break;
		case BONUS_ALIEN4:
			monster_init(map, x, y, MONSTER_ALIEN4);
			map_set_cell_type(map, x, y, CELL_EMPTY);
			break;
		default:
			break;
		}
	}
}

void display_bonus(struct map* map, int x, int y, unsigned char type)
{
	// bonus is encoded with the 4 most significant bits
	switch (type >> 4) {
	case BONUS_RANGE_INC:
		window_display_image(sprite_get_bonus(BONUS_RANGE_INC), x, y);
		break;

	case BONUS_RANGE_DEC:
		window_display_image(sprite_get_bonus(BONUS_RANGE_DEC), x, y);
		break;

	case BONUS_BOMB_INC:
		window_display_image(sprite_get_bonus(BONUS_BOMB_INC), x, y);
		break;

	case BONUS_BOMB_DEC:
		window_display_image(sprite_get_bonus(BONUS_BOMB_DEC), x, y);
		break;

	case BONUS_ROLLER:
		window_display_image(sprite_get_bonus(BONUS_ROLLER), x, y);
		break;

	case BONUS_LIFE:
		window_display_image(sprite_get_banner_life(), x, y);
		break;
	}
}

void display_scenery(struct map* map, int x, int  y, unsigned char type)
{
	switch (type >> 4) { // sub-types are encoded with the 4 most significant bits
	case SCENERY_STONE:
		window_display_image(sprite_get_stone(), x, y);
		break;

	case SCENERY_TREE:
		window_display_image(sprite_get_tree(), x, y);
		break;

	case SCENERY_WALL_B:
		window_display_image(sprite_get_wall_b(), x, y);
		break;

	case SCENERY_WALL_G:
		window_display_image(sprite_get_wall_g(), x, y);
		break;

	case SCENERY_WALL_H:
		window_display_image(sprite_get_wall_h(), x, y);
		break;

	case SCENERY_WALL_D:
		window_display_image(sprite_get_wall_d(), x, y);
		break;

	case SCENERY_ANGLE_BG:
		window_display_image(sprite_get_angle_bg(), x, y);
		break;

	case SCENERY_ANGLE_BD:
		window_display_image(sprite_get_angle_bd(), x, y);
		break;

	case SCENERY_ANGLE_HG:
		window_display_image(sprite_get_angle_hg(), x, y);
		break;

	case SCENERY_ANGLE_HD:
		window_display_image(sprite_get_angle_hd(), x, y);
		break;

	case SCENERY_BLOC:
		window_display_image(sprite_get_bloc(), x, y);
		break;
	}
}

void display_door(struct map* map, int x, int  y, unsigned char type) {
	switch (type >> 7) { // sub-type is encoded with the most significant bit
	case CLOSED_DOOR:
		window_display_image(sprite_get_closed_door(), x, y);
		break;

	case OPENED_DOOR:
		window_display_image(sprite_get_door(), x, y);
		break;
	}
}

void display_goal(struct map* map, int x, int  y, unsigned char type) {
	switch (type) {
	case CELL_FLAG:
		window_display_image(sprite_get_flag(), x, y);
		break;

	case CELL_PRINCESS:
		window_display_image(sprite_get_princess(), x, y);
		break;
	}
}

void map_display(struct map* map, int offset_x, int offset_y)
{
	assert(map != NULL);
	assert(map->height > 0 && map->width > 0);

	int x, y;
	for (int i = 0; i < map->width; i++) {
		for (int j = 0; j < map->height; j++) {
			x = i * SIZE_BLOC + offset_x;
			y = j * SIZE_BLOC + offset_y;

			char type = map->grid[CELL(i,j)];

			switch (type & 15) { // type is encoded with 4 bits, 15 (1111) is a mask to keep the four less significant bits
			case CELL_SCENERY:
				display_scenery(map, x, y, type);
				break;
			case CELL_CASE:
				window_display_image(sprite_get_box(), x, y);
				break;
			case CELL_BONUS:
				display_bonus(map, x, y, type);
				break;
			case CELL_KEY:
				window_display_image(sprite_get_key(), x, y);
				break;
			case CELL_DOOR:
				display_door(map, x, y, type);
				break;
			case CELL_GOAL:
				display_goal(map, x, y, type);
				break;
			}
		}
	}
}

void map_insert_monster(struct map* map, int x, int y, s_type type, void* data)
{
	map->monstersList = list_insert_back(map->monstersList, x, y, type, data);
}

void map_insert_bomb(struct map* map, int x, int y, s_type type, void* data)
{
	map->bombsList = list_insert_back(map->bombsList, x, y, type, data);
}
