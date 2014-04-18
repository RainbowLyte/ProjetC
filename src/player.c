#include <SDL/SDL_image.h>
#include <assert.h>
#include <math.h>

#include <player.h>
#include <sprite.h>
#include <window.h>
#include <misc.h>
#include <constant.h>
#include <wiimote.h>

struct player {
	int id; // # of player
	int x, y;
	int x_sprite, y_sprite;
	way_t current_way;
	int way[4];
	int nb_bomb;
	int nb_life;
	int nb_range;
	int invicibility;
	int moving;
	int velocity;
	int anim;
	int rumble;
	int key;
};

struct player* player_init(int id, int bomb_number, int life_number, int range_number) {
	struct player* player = malloc(sizeof(*player));
	if (!player)
		error("Memory error");

	player->current_way = SOUTH;
	player->way[NORTH] = 0;
	player->way[SOUTH] = 0;
	player->way[EAST] = 0;
	player->way[WEST] = 0;
	player->nb_bomb = bomb_number;
	player->nb_life = life_number;
	player->nb_range = range_number;
	player->invicibility = 0;
	player->moving = 0;
	player->velocity = 4;
	player->x_sprite = 0;
	player->y_sprite = 0;
	player->anim = 0;
	player->rumble = 0;
	player->id = id;
	player->key = 0;

	return player;
}
 
void player_free(struct player* player) {
	assert(player);
	free(player);
}

void player_reset_way_mov(struct player* player) {
	assert(player);
	for(int i = 0; i < 4; i++)
		player->way[i] = 0;
	player->moving = 0;
}

int player_get_x(struct player* player) {
	assert(player != NULL);
	return player->x;
}

int player_get_y(struct player* player) {
	assert(player != NULL);
	return player->y;
}

void player_set_current_way(struct player* player, way_t way) {
	assert(player);
	player->current_way = way;
}

void player_set_way(struct player* player, way_t way) {
	assert(player);
	player->way[way] = 1;
}

void player_unset_way(struct player* player, way_t way) {
	assert(player);
	player->way[way] = 0;
}

void player_inc_moving(struct player* player) {
	assert(player);
	player->moving++;
}

void player_dec_moving(struct player* player) {
	assert(player);
	if(player->moving > 0)
		player->moving--;
}

int player_get_moving(struct player* player) {
	assert(player);
	return(player->moving);
}

void player_inc_velocity(struct player* player) {
	assert(player);
	if(player->velocity < 40)
		player->velocity++;
}

void player_dec_velocity(struct player* player) {
	assert(player);
	if(player->velocity > 1)
		player->velocity--;
}
int player_get_nb_bomb(struct player* player) { // get nb_bomb
	assert(player);
	return player->nb_bomb;
}

void player_inc_nb_bomb(struct player* player) { // nb_bomb++
	assert(player);
	if(player_get_nb_bomb(player) < 9)
		player->nb_bomb += 1;
}

void player_dec_nb_bomb(struct player* player) { // nb_bomb--
	assert(player);
	if(player_get_nb_bomb(player) > 0)
		player->nb_bomb -= 1;
}

int player_get_nb_life(struct player* player) { // get nb_life
	assert(player);
	return player->nb_life;
}

void player_inc_nb_life(struct player* player) { // nb_life++
	assert(player);
	if(player_get_nb_life(player) < 9)
		player->nb_life += 1;
}

void player_dec_nb_life(struct player* player) { // nb_life-- TODO gameover if nb_life <= 0
	assert(player);
	if(player_get_nb_life(player) > 0)
		player->nb_life -= 1;
#ifdef USE_WIIMOTE
		wiimote_set_rumble(player->id, 1);
		player->rumble = DEFAULT_GAME_FPS; // 1s of rumble
#endif
}

void player_hit(struct player* player, int invicibility_time) { // invicibility_time in frame
	assert(player);
	if(!player->invicibility) {
		player_dec_nb_life(player);
	player_set_invicibility(player, invicibility_time);
	}
}

void player_set_invicibility(struct player* player, int invicibility_time) {
	assert(player);
	player->invicibility = invicibility_time;
}

int player_get_nb_range(struct player* player) { // get nb_range
	assert(player);
	return player->nb_range;
}

void player_inc_nb_range(struct player* player) { // nb_range++
	assert(player);
	if(player_get_nb_range(player) < 9)
		player->nb_range += 1;
}

void player_dec_nb_range(struct player* player) { // nb_range--
	assert(player);
	if(player_get_nb_range(player) > 1)
		player->nb_range -= 1;
}

int player_get_key(struct player* player) {
	assert(player);
	return(player->key);
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
	player->x_sprite = 0;
	player->y_sprite = 0;
}

static int player_move_aux(struct player* player, struct map* map, int x, int y, struct game* game) {
	int cellType = 0;
//	struct bomb* bomb;
//	struct list* bList;

	if (!map_is_inside(map, x, y))
		return 0;

	switch (map_get_cell_type(map, x, y)) {
	case CELL_SCENERY:
		return 0; // stop the function and the player movements
		break;

	case CELL_CASE: 
		switch(player->current_way)
		{
			case NORTH:
			if(y < 1 || player->y != y + 1)
				return 0;
			else {
				cellType = map_get_cell_type(map, x, y - 1);
				if( cellType != CELL_EMPTY || y < 1)
					return 0;
				map_set_cell_type(map, x, y, CELL_EMPTY);
				map_set_cell_type(map, x, y - 1, CELL_CASE);
			}
			break;

			case SOUTH:
				if(y >= map_get_height(map) - 1 || player->y != y - 1)
					return 0;
			else {
				cellType = map_get_cell_type(map, x, y + 1);
				if( cellType != CELL_EMPTY)
					return 0;
				map_set_cell_type(map, x, y, CELL_EMPTY);
				map_set_cell_type(map, x, y + 1, CELL_CASE);
			}
			break;

			case EAST:
			if(x >= map_get_width(map) - 1 || player->x != x - 1)
				return 0;
			else {
				cellType = map_get_cell_type(map, x + 1, y);
				if( cellType != CELL_EMPTY)
					return 0;
				map_set_cell_type(map, x, y, CELL_EMPTY);
				map_set_cell_type(map, x + 1, y, CELL_CASE);
			}
			break;

			case WEST:
			if(x < 1 || player->x != x + 1)
				return 0;
			else {
				cellType = map_get_cell_type(map, x - 1, y);
				if( cellType != CELL_EMPTY)
					return 0;
				map_set_cell_type(map, x, y, CELL_EMPTY);
				map_set_cell_type(map, x - 1, y, CELL_CASE);
			}
			break;
		}
		break;
		case CELL_BOMB:
			return 0;
			break;
		case CELL_GOAL: // todo : goal
			break;
		default:
			break;
	}

	// Player has moved
	return 1;
}

int player_move(struct player* player, struct map* map, struct game* game) {
	int x = player->x;
	int y = player->y;
	int move = 0;

//printf("%d%d%d%d\n", player->way[0],  player->way[1],  player->way[2],  player->way[3]);

	if(player->way[NORTH] && (player->moving == 1 || (player->x_sprite <= -15 && player_move_aux(player, map, x - 1, y - 1, game)) || (player->x_sprite >= 15 && player_move_aux(player, map, x + 1, y - 1, game))  || player_move_aux(player, map, x, y - 1, game)  || player->y_sprite > 0)) {
		player->current_way = NORTH;
		if (player_move_aux(player, map, x, y - 1, game) || player->y_sprite > 0) {
			if(player->y_sprite > 0 && (player->y_sprite - player->velocity) < 0)
				player->y_sprite = 0;
			else
				player->y_sprite -= player->velocity;
			if(player->x_sprite > 0) {
				player->x_sprite -= player->velocity;
				if(player->x_sprite < 0)
					player->x_sprite = 0;
			}
			else if(player->x_sprite < 0) {
				player->x_sprite += player->velocity;
				if(player->x_sprite > 0)
					player->x_sprite = 0;
			}
		}
		else if(player->x_sprite <= -15 && player_move_aux(player, map, x - 1, y - 1, game)) {
			player->x_sprite -= player->velocity;
			player->current_way = WEST;
		}
		else if(player->x_sprite >= 15 && player_move_aux(player, map, x + 1, y - 1, game)) {
			player->x_sprite += player->velocity;
			player->current_way = EAST;
		}
	}
	else if(player->way[SOUTH] && (player->moving == 1 || (player->x_sprite <= -15 && player_move_aux(player, map, x - 1, y + 1, game))|| (player->x_sprite >= 15 && player_move_aux(player, map, x + 1, y + 1, game)) || (player_move_aux(player, map, x, y + 1, game) || player->y_sprite < 0) )) {
		player->current_way = SOUTH;
		if (player_move_aux(player, map, x, y + 1, game) || player->y_sprite < 0) {
			if(player->y_sprite < 0 && (player->y_sprite + player->velocity) > 0)
				player->y_sprite = 0;
			else
				player->y_sprite += player->velocity;
			if(player->x_sprite > 0) {
				player->x_sprite -= player->velocity;
				if(player->x_sprite < 0)
					player->x_sprite = 0;
			}
			else if(player->x_sprite < 0) {
				player->x_sprite += player->velocity;
				if(player->x_sprite > 0)
					player->x_sprite = 0;
			}
		}
		else if(player->x_sprite <= -15 && player_move_aux(player, map, x - 1, y + 1, game)) {
			player->x_sprite -= player->velocity;
			player->current_way = WEST;
		}
		else if(player->x_sprite >= 15 && player_move_aux(player, map, x + 1, y + 1, game)) {
			player->x_sprite += player->velocity;
			player->current_way = EAST;
		}
	}
	else if(player->way[WEST] && (player->moving == 1 || (player->y_sprite <= -15 && player_move_aux(player, map, x - 1, y - 1, game)) || (player->y_sprite >= 15 && player_move_aux(player, map, x - 1, y + 1, game)) || player_move_aux(player, map, x - 1, y, game)  || player->x_sprite > 0)) {
		player->current_way = WEST;
		if (player_move_aux(player, map, x - 1, y, game) || player->x_sprite > 0) {
			if(player->x_sprite > 0 && (player->x_sprite - player->velocity) < 0)
				player->x_sprite = 0;
			else
				player->x_sprite -= player->velocity;
			if(player->y_sprite > 0) {
				player->y_sprite -= player->velocity;
				if(player->y_sprite < 0)
					player->y_sprite = 0;
			}
			else if(player->y_sprite < 0) {
				player->y_sprite += player->velocity;
				if(player->y_sprite > 0)
					player->y_sprite = 0;
			}
		}
		else if(player->y_sprite <= -15 && player_move_aux(player, map, x - 1, y - 1, game)) {
			player->y_sprite -= player->velocity;
			player->current_way = NORTH;
		}
		else if(player->y_sprite >= 15 && player_move_aux(player, map, x - 1, y + 1, game)) {
			player->y_sprite += player->velocity;
			player->current_way = SOUTH;
		}
	}
	else if(player->way[EAST] && (player->moving == 1 || (player->y_sprite <= -15 && player_move_aux(player, map, x + 1, y - 1, game)) || (player->y_sprite >= 15 && player_move_aux(player, map, x + 1, y + 1, game)) || player_move_aux(player, map, x + 1, y, game) || player->x_sprite < 0)) {
		player->current_way = EAST;
		if (player_move_aux(player, map, x + 1, y, game) || player->x_sprite < 0) {
			if(player->x_sprite < 0 && (player->x_sprite + player->velocity) > 0)
				player->x_sprite = 0;
			else
				player->x_sprite += player->velocity;
			if(player->y_sprite > 0) {
				player->y_sprite -= player->velocity;
				if(player->y_sprite < 0)
					player->y_sprite = 0;
			}
			else if(player->y_sprite < 0) {
				player->y_sprite += player->velocity;
				if(player->y_sprite > 0)
					player->y_sprite = 0;
			}
		}
		else if(player->y_sprite <= -15 && player_move_aux(player, map, x + 1, y - 1, game)) {
			player->y_sprite -= player->velocity;
			player->current_way = NORTH;
		}
		else if(player->y_sprite >= 15 && player_move_aux(player, map, x + 1, y + 1, game)) {
			player->y_sprite += player->velocity;
			player->current_way = SOUTH;
		}
	}

	if(player->y_sprite < -20) {
		player->y_sprite += 40;
		player->y--;
		move = 1;
		player->current_way = NORTH;
	}
	if(player->y_sprite > 20) {
		player->y_sprite -= 40;
		player->y++;
		move = 1;
		player->current_way = SOUTH;
	}
	if(player->x_sprite < -20) {
		player->x_sprite += 40;
		player->x--;
		move = 1;
		player->current_way = WEST;
	}
	//printf("x_spire: %d\n", player->x_sprite);
	if(player->x_sprite > 20) {
		player->x_sprite -= 40;
		player->x++;
		move = 1;
		player->current_way = EAST;
		//printf("ok\n");
	}

	unsigned char type = map_get_cell_compose_type(map, player->x, player->y);
	if(type != CELL_EMPTY) {
		switch(type & 15) {
		case CELL_BONUS:
			switch(type >> 4) {
			case BONUS_RANGE_INC:
				player_inc_nb_range(player);
				break;
			case BONUS_RANGE_DEC:
				player_dec_nb_range(player);
				break;
			case BONUS_BOMB_INC:
				player_inc_nb_bomb(player);
				break;
			case BONUS_BOMB_DEC:
				player_dec_nb_bomb(player);
				break;
			case BONUS_LIFE:
				player_inc_nb_life(player);
				break;
			}
			map_set_cell_type(map, player->x, player->y, CELL_EMPTY);
			break;
		case CELL_DOOR:
			if(type >> 7)
				level_change_map(game, player, map, (type & 112) >> 4);
			else if(type >> 7 == 0 && player->key > 0) {
				player->key--;
				level_change_map(game, player, map, (type & 112) >> 4);
			}
			//printf("door, type: %d, type>>4: %d, map: %d\n", type, type >>4, (type & 112)>>4);
			break;
		case CELL_KEY:
			player->key++;
			map_set_cell_type(map, player->x, player->y, CELL_EMPTY);
			break;
		}
	}

/*	if (move) {
		if(map_get_cell_type(map, x, y) == CELL_PLAYER)
			map_set_cell_type(map, x, y, CELL_EMPTY);
		map_set_cell_type(map, player->x, player->y, CELL_PLAYER);
	} */
	//printf("mov: %d, x: %d, y: %d, x_sprite: %d, y_sprite: %d\n", player->moving, player->x, player->y, player->x_sprite, player->y_sprite);
	return move;
}

void player_display(struct player* player, struct game* game) {
	assert(player);
	int anim;

	if(player->invicibility > 0) {
		if(((player->invicibility)/2)%2)
			SDL_SetAlpha(sprite_get_players(), SDL_SRCALPHA, 128);
		else
			SDL_SetAlpha(sprite_get_players(), SDL_SRCALPHA, 192);
	}
	else
		SDL_SetAlpha(sprite_get_players(), SDL_SRCALPHA, 255);

	if(player->moving) {
		anim = (((game_get_frame(game) - player->anim)*(player->velocity)/12)+1)%8;
	}
	else {
		anim = 0;
	}

	window_display_sprite(	sprite_get_players(),
							sprite_get_rect_player_anim(anim, player->current_way),
							player->x * SIZE_BLOC + player->x_sprite,
							player->y * SIZE_BLOC + player->y_sprite - 20
							);
}

void player_update(struct player* player) {
	assert(player);
	if(player->invicibility > 0)
		player->invicibility--;
	if(player->rumble > 0) {
		player->rumble--;
#ifdef USE_WIIMOTE
		if(player->rumble <= 0)
			wiimote_set_rumble(player->id, 0);
#endif
	}
}
