#include <SDL/SDL_image.h>
#include <SDL/SDL_ttf.h>
#include <assert.h>
#include <dirent.h>

#include <sprite.h>
#include <misc.h>
#include <constant.h>

// Sprites general 
#define MAP_CASE        "sprite/wood_box.png"
#define MAP_KEY			"sprite/key.png"
#define MAP_DOOR		"sprite/door.png"
#define MAP_CLOSED_DOOR	"sprite/closed_door.png"
#define FLAG			"sprite/flag.png"
#define PRINCESS		"sprite/bomberwoman.png"
#define EMPTY			"sprite/empty.png"

// Scenery elements
#define MAP_STONE		"sprite/stone.png"
#define MAP_TREE        "sprite/tree.png"
#define MAP_WALL_B		"sprite/wall_b.png"
#define MAP_WALL_G		"sprite/wall_g.png"
#define MAP_WALL_H		"sprite/wall_h.png"
#define MAP_WALL_D		"sprite/wall_d.png"
#define MAP_ANGLE_BG	"sprite/angle_bg.png"
#define MAP_ANGLE_BD	"sprite/angle_bd.png"
#define MAP_ANGLE_HG	"sprite/angle_hg.png"
#define MAP_ANGLE_HD	"sprite/angle_hd.png"
#define MAP_BLOC		"sprite/bloc.png"

// Font
#define FONT 			"sprite/font.ttf"

// Sprites of Banner
#define BANNER_LINE		"sprite/banner_line.png"
#define BANNER_LIFE		"sprite/banner_life.png"
#define BANNER_BOMB		"sprite/bomb3.png"
#define BANNER_RANGE	"sprite/banner_range.png"

// Sprites of Bombs
#define BOMB			"sprite/bomb40.png"

// Sprites of menu
#define MENU_BG_GREY 		"sprite/m_bg_grey.png"
#define MENU_BG_MAIN		"sprite/m_bg_main.png"
#define MENU_BG_WIN			"sprite/m_bg_win.png"
#define MENU_BG_GAMEOVER	"sprite/m_bg_gameover.png"
#define MENU_B_NEWGAME		"sprite/m_b_newgame.png"
#define MENU_B_QUIT			"sprite/m_b_quit.png"
#define MENU_B_KEEP			"sprite/m_b_continuer.png"
#define MENU_B_MAIN			"sprite/m_b_main.png"
#define MENU_H_PAUSE		"sprite/m_h_pause.png"
#define MENU_S_STARS		"sprite/stars.png"
#define MENU_S_ALIEN2		"sprite/alien2_200.png"
#define MENU_S_ALIEN3		"sprite/alien3_200.png"
#define MENU_S_ALIEN4		"sprite/alien4_200.png"
#define MENU_S_CAGE			"sprite/cage.png"

// Sprites of Bonus
#define IMG_BONUS_BOMB_RANGE_INC 	"sprite/bonus_bomb_range_inc.png"
#define IMG_BONUS_BOMB_RANGE_DEC	"sprite/bonus_bomb_range_dec.png"
#define IMG_BONUS_BOMB_NB_INC		"sprite/bonus_bomb_nb_inc.png"
#define IMG_BONUS_BOMB_NB_DEC		"sprite/bonus_bomb_nb_dec.png"
#define IMG_BONUS_ROLLER			"sprite/roller.png"

// Sprites of Players
#define PLAYER_1			"sprite/bomberman1_40.png"
#define PLAYER_2			"sprite/bomberman2_40.png"
#define PLAYER_3			"sprite/bomberman3_40.png"
#define PLAYER_4			"sprite/bomberman4_40.png"

// Sprites of Monsters
#define MONSTER_LEFT		"sprite/monster_left.png"
#define MONSTER_UP			"sprite/monster_up.png"
#define MONSTER_RIGHT		"sprite/monster_right.png"
#define MONSTER_DOWN		"sprite/monster_down.png"
#define MONSTER_IMG_ALIEN1	"sprite/alien1_40.png"
#define MONSTER_IMG_ALIEN2	"sprite/alien2_40.png"
#define MONSTER_IMG_ALIEN3	"sprite/alien3_40.png"
#define MONSTER_IMG_ALIEN4	"sprite/alien4_40.png"

// Text & Menu
SDL_Surface* numbers[11];
SDL_Surface* numbers_w[11];
SDL_Surface* menu[NB_SURFACE_MENU];
SDL_Surface* map_multi[20];
int nb_map_multi = 0;
int max_width;

#define NB_TXT_SCORE 5
SDL_Surface* score[NB_TXT_SCORE];

#define NB_ANIM_STARS 4
#define SIZE_OF_STARS 10
SDL_Surface* menu_stars;
SDL_Rect menu_rect_stars[2 * NB_ANIM_STARS - 1];


// banner
SDL_Surface* banner_line;
SDL_Surface* banner_bomb;
SDL_Surface* banner_range;
SDL_Surface* banner_life;

// map
SDL_Surface* box;
SDL_Surface* goal;
SDL_Surface* key;
SDL_Surface* door;
SDL_Surface* closed_door;
SDL_Surface* stone;
SDL_Surface* tree;
SDL_Surface* flag;
SDL_Surface* princess;
SDL_Surface* empty;
SDL_Surface* wall_b;
SDL_Surface* wall_g;
SDL_Surface* wall_h;
SDL_Surface* wall_d;
SDL_Surface* angle_bg;
SDL_Surface* angle_bd;
SDL_Surface* angle_hg;
SDL_Surface* angle_hd;
SDL_Surface* bloc;

#define SIZE_OF_SPRITE 40

// bonus
#define NB_BONUS 5
SDL_Surface* bonus[NB_BONUS];

// monster
#define NB_MONSTER 5
SDL_Surface** monsters[NB_MONSTER];
SDL_Surface* monster_img_norm[4];
SDL_Surface* monster_img_alien1[4];
SDL_Surface* monster_img_alien2[4];
SDL_Surface* monster_img_alien3[4];
SDL_Surface* monster_img_alien4[4];

// player
// sprites 40x60
#define NB_ANIM_PLAYER 8
#define SIZE_TAB_ANIM_PLAYER 4
#define SIZE_OF_PLAYER_SPRITE 60

SDL_Surface* players[4];

SDL_Rect* player[4][SIZE_TAB_ANIM_PLAYER];
SDL_Rect player_rect_up[4][NB_ANIM_PLAYER];
SDL_Rect player_rect_down[4][NB_ANIM_PLAYER];
SDL_Rect player_rect_left[4][NB_ANIM_PLAYER];
SDL_Rect player_rect_right[4][NB_ANIM_PLAYER];

// bombs
#define SIZE_TAB_ANIM_BOMB 8
#define NB_ANIM_BOMBS 4

SDL_Surface* bombs;

SDL_Rect* bomb[SIZE_TAB_ANIM_BOMB];
SDL_Rect bomb_rect_wait[NB_ANIM_BOMBS];
SDL_Rect bomb_rect_center[2*NB_ANIM_BOMBS - 1];
SDL_Rect bomb_rect_v[2*NB_ANIM_BOMBS - 1];
SDL_Rect bomb_rect_up[2*NB_ANIM_BOMBS - 1];
SDL_Rect bomb_rect_down[2*NB_ANIM_BOMBS - 1];
SDL_Rect bomb_rect_left[2*NB_ANIM_BOMBS - 1];
SDL_Rect bomb_rect_right[2*NB_ANIM_BOMBS - 1];
SDL_Rect bomb_rect_h[2*NB_ANIM_BOMBS - 1];

void menu_load() {
	TTF_Font* police = TTF_OpenFont(FONT, 50);
	SDL_Color couleurBlanche = {255, 255, 255};
	SDL_Color couleurNoir = {0, 0, 0};

	menu[M_H_PAUSE] = TTF_RenderText_Blended(police, "PAUSE", couleurBlanche);
	menu[M_H_SAVE] = TTF_RenderText_Blended(police, "Save ?", couleurBlanche);

	TTF_CloseFont(police);
	police = TTF_OpenFont(FONT, 40);

	numbers[0] = TTF_RenderText_Blended(police, "0", couleurNoir);
	numbers[1] = TTF_RenderText_Blended(police, "1", couleurNoir);
	numbers[2] = TTF_RenderText_Blended(police, "2", couleurNoir);
	numbers[3] = TTF_RenderText_Blended(police, "3", couleurNoir);
	numbers[4] = TTF_RenderText_Blended(police, "4", couleurNoir);
	numbers[5] = TTF_RenderText_Blended(police, "5", couleurNoir);
	numbers[6] = TTF_RenderText_Blended(police, "6", couleurNoir);
	numbers[7] = TTF_RenderText_Blended(police, "7", couleurNoir);
	numbers[8] = TTF_RenderText_Blended(police, "8", couleurNoir);
	numbers[9] = TTF_RenderText_Blended(police, "9", couleurNoir);
	numbers[10] = TTF_RenderText_Blended(police, ":", couleurNoir);

	numbers_w[0] = TTF_RenderText_Blended(police, "0", couleurBlanche);
	numbers_w[1] = TTF_RenderText_Blended(police, "1", couleurBlanche);
	numbers_w[2] = TTF_RenderText_Blended(police, "2", couleurBlanche);
	numbers_w[3] = TTF_RenderText_Blended(police, "3", couleurBlanche);
	numbers_w[4] = TTF_RenderText_Blended(police, "4", couleurBlanche);
	numbers_w[5] = TTF_RenderText_Blended(police, "5", couleurBlanche);
	numbers_w[6] = TTF_RenderText_Blended(police, "6", couleurBlanche);
	numbers_w[7] = TTF_RenderText_Blended(police, "7", couleurBlanche);
	numbers_w[8] = TTF_RenderText_Blended(police, "8", couleurBlanche);
	numbers_w[9] = TTF_RenderText_Blended(police, "9", couleurBlanche);
	numbers_w[10] = TTF_RenderText_Blended(police, ":", couleurBlanche);

	score[0] = TTF_RenderText_Blended(police, "It's a Draw !", couleurBlanche);
	score[1] = TTF_RenderText_Blended(police, "Player 1 Wins !", couleurBlanche);
	score[2] = TTF_RenderText_Blended(police, "Player 2 Wins !", couleurBlanche);
	score[3] = TTF_RenderText_Blended(police, "Player 3 Wins !", couleurBlanche);
	score[4] = TTF_RenderText_Blended(police, "Player 4 Wins !", couleurBlanche);

	TTF_CloseFont(police);
	police = TTF_OpenFont(FONT, 22);

	menu[M_B_SINGLE] = TTF_RenderText_Blended(police, "Single Player", couleurBlanche);
	menu[M_B_NEWGAME] = TTF_RenderText_Blended(police, "New Game", couleurBlanche);
	menu[M_B_LOADGAME] = TTF_RenderText_Blended(police, "Load Game", couleurBlanche);

	menu[M_B_MULTI] = TTF_RenderText_Blended(police, "Multi  Player", couleurBlanche);
	menu[M_B_2PLAYER] = TTF_RenderText_Blended(police, "2 Players", couleurBlanche);
	menu[M_B_3PLAYER] = TTF_RenderText_Blended(police, "3 Players", couleurBlanche);
	menu[M_B_4PLAYER] = TTF_RenderText_Blended(police, "4 Players", couleurBlanche);

	menu[M_B_KEEP] = TTF_RenderText_Blended(police, "Continue", couleurBlanche);
	menu[M_B_MAINMENU] = TTF_RenderText_Blended(police, "Main Menu", couleurBlanche);
	menu[M_B_QUIT] = TTF_RenderText_Blended(police, "Quit", couleurBlanche);

	menu[M_B_YES] = TTF_RenderText_Blended(police, "Yes", couleurBlanche);
	menu[M_B_NO] = TTF_RenderText_Blended(police, "No", couleurBlanche);

	menu[M_B_CHANGEMAP] = TTF_RenderText_Blended(police, "Change Map", couleurBlanche);

	menu[M_S_SELECT] = TTF_RenderText_Blended(police, ">", couleurBlanche);
	menu[M_S_SELECT_BLACK] = TTF_RenderText_Blended(police, ">", couleurNoir);

	// Maps multiplayer
	DIR* dir = NULL;
	struct dirent* readfile = NULL;
	dir = opendir("./data/multi");
	
	if(!dir)
		printf("Error : unable to open data/multi\n");

	max_width = 0;
	while((readfile = readdir(dir)) != NULL) {
		if(map_is_valid_format2(readfile->d_name)) {
			map_multi[nb_map_multi] = TTF_RenderText_Blended(police, readfile->d_name, couleurNoir);
			if(map_multi[nb_map_multi]->w >= max_width)
				max_width = map_multi[nb_map_multi]->w;
			nb_map_multi++;
		}
	}
	if(closedir(dir) == -1)
		printf("Problème à la fermeture");

	
	TTF_CloseFont(police);

	// Backgrounds
	menu[M_BG_GREY] = load_image(MENU_BG_GREY);
	menu[M_BG_MAINMENU] = load_image(MENU_BG_MAIN);
	menu[M_BG_WIN] = load_image(MENU_BG_WIN);
	menu[M_BG_GAMEOVER] = load_image(MENU_BG_GAMEOVER);

	// Sprites
	menu[M_S_STARS] = load_image(MENU_S_STARS);
	menu[M_S_ALIEN2] = load_image(MENU_S_ALIEN2);
	menu[M_S_ALIEN3] = load_image(MENU_S_ALIEN3);
	menu[M_S_ALIEN4] = load_image(MENU_S_ALIEN4);
	menu[M_S_CAGE] = load_image(MENU_S_CAGE);

	for(int i = 0; i < NB_ANIM_STARS; i++) {
			menu_rect_stars[i].x = SIZE_OF_STARS * i;
			menu_rect_stars[i].y = 0;
			menu_rect_stars[i].w = SIZE_OF_STARS;
			menu_rect_stars[i].h = SIZE_OF_STARS;
		}
	for(int i = NB_ANIM_STARS; i < 2*NB_ANIM_STARS - 1; i++) {
		menu_rect_stars[i].x = menu_rect_stars[2*NB_ANIM_STARS-1-i].x;
		menu_rect_stars[i].y = 0;
		menu_rect_stars[i].w = SIZE_OF_STARS;
		menu_rect_stars[i].h = SIZE_OF_STARS;
	}

}

void menu_unload() {
	for (int i = 0; i < NB_SURFACE_MENU; i++)
		SDL_FreeSurface(menu[i]);

	for (int i = 0; i < 11; i++)
		SDL_FreeSurface(numbers[i]);

	for (int i = 0; i < 11; i++)
		SDL_FreeSurface(numbers_w[i]);

	SDL_FreeSurface(menu_stars);

	for(int i = 0; i < nb_map_multi; i++)
		SDL_FreeSurface(map_multi[i]);

	for(int i = 0; i < NB_TXT_SCORE; i++)
		SDL_FreeSurface(score[i]);
}

void banner_load() {
	// other banner sprites
	banner_line = load_image(BANNER_LINE);
	banner_bomb = load_image(BANNER_BOMB);
	banner_range = load_image(BANNER_RANGE);
	banner_life = load_image(BANNER_LIFE);
}

void banner_unload() {
	// other banner sprites
	SDL_FreeSurface(banner_line);
	SDL_FreeSurface(banner_bomb);
	SDL_FreeSurface(banner_range);
	SDL_FreeSurface(banner_life);
}

void map_load() {
	// Sprite loading
	tree = load_image(MAP_TREE);
	box = load_image(MAP_CASE);
	key = load_image(MAP_KEY);
	stone = load_image(MAP_STONE);
	door = load_image(MAP_DOOR);
	closed_door = load_image(MAP_CLOSED_DOOR);
	flag = load_image(FLAG);
	princess = load_image(PRINCESS);
	empty = load_image(EMPTY);
	wall_b = load_image(MAP_WALL_B);
	wall_g = load_image(MAP_WALL_G);
	wall_h = load_image(MAP_WALL_H);
	wall_d = load_image(MAP_WALL_D);
	angle_bg = load_image(MAP_ANGLE_BG);
	angle_bd = load_image(MAP_ANGLE_BD);
	angle_hg = load_image(MAP_ANGLE_HG);
	angle_hd = load_image(MAP_ANGLE_HD);
	bloc = load_image(MAP_BLOC);
}

void map_unload() {
	SDL_FreeSurface(tree);
	SDL_FreeSurface(box);
	SDL_FreeSurface(goal);
	SDL_FreeSurface(key);
	SDL_FreeSurface(stone);
	SDL_FreeSurface(door);
	SDL_FreeSurface(closed_door);
	SDL_FreeSurface(flag);
	SDL_FreeSurface(princess);
	SDL_FreeSurface(empty);

	SDL_FreeSurface(wall_b);
	SDL_FreeSurface(wall_g);
	SDL_FreeSurface(wall_h);
	SDL_FreeSurface(wall_d);

	SDL_FreeSurface(angle_bg);
	SDL_FreeSurface(angle_bd);
	SDL_FreeSurface(angle_hg);
	SDL_FreeSurface(angle_hd);

	SDL_FreeSurface(bloc);
}

void bonus_load() {
	bonus[BONUS_RANGE_INC-1] = load_image(IMG_BONUS_BOMB_RANGE_INC);
	bonus[BONUS_RANGE_DEC-1] = load_image(IMG_BONUS_BOMB_RANGE_DEC);
	bonus[BONUS_BOMB_INC-1] = load_image(IMG_BONUS_BOMB_NB_INC);
	bonus[BONUS_BOMB_DEC-1] = load_image(IMG_BONUS_BOMB_NB_DEC);
	bonus[BONUS_ROLLER-1] = load_image(IMG_BONUS_ROLLER);
}

void bonus_unload() {
	for (int i = 0; i < NB_BONUS; i++)
		SDL_FreeSurface(bonus[i]);
}

void player_load() {
	players[0] = load_image(PLAYER_1);
	players[1] = load_image(PLAYER_2);
	players[2] = load_image(PLAYER_3);
	players[3] = load_image(PLAYER_4);

	for(int j=0; j<4; j++)
	{
		player[j][SOUTH] =  player_rect_down[j];
		player[j][NORTH] = player_rect_up[j];
		player[j][EAST] = player_rect_right[j];
		player[j][WEST] = player_rect_left[j];

		for(int i = 0; i < NB_ANIM_PLAYER; i++) {
			player_rect_down[j][i].x = SIZE_OF_SPRITE * i;
			player_rect_down[j][i].y = 0;
			player_rect_down[j][i].w = SIZE_OF_SPRITE ;
			player_rect_down[j][i].h = SIZE_OF_PLAYER_SPRITE;
		}
		for(int i = 0; i < NB_ANIM_PLAYER; i++) {
			player_rect_up[j][i].x = SIZE_OF_SPRITE * i;
			player_rect_up[j][i].y = 3 * SIZE_OF_PLAYER_SPRITE;
			player_rect_up[j][i].w = SIZE_OF_SPRITE ;
			player_rect_up[j][i].h = SIZE_OF_PLAYER_SPRITE;
		}
		for(int i = 0; i < NB_ANIM_PLAYER; i++) {
			player_rect_right[j][i].x = SIZE_OF_SPRITE * i;
			player_rect_right[j][i].y = SIZE_OF_PLAYER_SPRITE;
			player_rect_right[j][i].w = SIZE_OF_SPRITE ;
			player_rect_right[j][i].h = SIZE_OF_PLAYER_SPRITE;
		}
		for(int i = 0; i < NB_ANIM_PLAYER; i++) {
			player_rect_left[j][i].x = SIZE_OF_SPRITE * i;
			player_rect_left[j][i].y = 2 * SIZE_OF_PLAYER_SPRITE;
			player_rect_left[j][i].w = SIZE_OF_SPRITE ;
			player_rect_left[j][i].h = SIZE_OF_PLAYER_SPRITE;
		}
	}

}

void player_unload() {
	SDL_FreeSurface(players[0]);
	SDL_FreeSurface(players[1]);
	SDL_FreeSurface(players[2]);
	SDL_FreeSurface(players[3]);
}

void monster_load() {

	monsters[MONSTER_NORMAL] = monster_img_norm;
	monsters[MONSTER_ALIEN1] = monster_img_alien1;
	monsters[MONSTER_ALIEN2] = monster_img_alien2;
	monsters[MONSTER_ALIEN3] = monster_img_alien3;
	monsters[MONSTER_ALIEN4] = monster_img_alien4;

	monster_img_norm[WEST] = load_image(MONSTER_LEFT);
	monster_img_norm[EAST] = load_image(MONSTER_RIGHT);
	monster_img_norm[NORTH] = load_image(MONSTER_UP);
	monster_img_norm[SOUTH] = load_image(MONSTER_DOWN);

	monster_img_alien1[NORTH] = load_image(MONSTER_IMG_ALIEN1);
	monster_img_alien1[WEST] = load_image(MONSTER_IMG_ALIEN1);
	monster_img_alien1[SOUTH] = load_image(MONSTER_IMG_ALIEN1);
	monster_img_alien1[EAST] = load_image(MONSTER_IMG_ALIEN1);

	monster_img_alien2[NORTH] = load_image(MONSTER_IMG_ALIEN2);
	monster_img_alien2[WEST] = load_image(MONSTER_IMG_ALIEN2);
	monster_img_alien2[SOUTH] = load_image(MONSTER_IMG_ALIEN2);
	monster_img_alien2[EAST] = load_image(MONSTER_IMG_ALIEN2);

	monster_img_alien3[NORTH] = load_image(MONSTER_IMG_ALIEN3);
	monster_img_alien3[WEST] = load_image(MONSTER_IMG_ALIEN3);
	monster_img_alien3[SOUTH] = load_image(MONSTER_IMG_ALIEN3);
	monster_img_alien3[EAST] = load_image(MONSTER_IMG_ALIEN3);

	monster_img_alien4[NORTH] = load_image(MONSTER_IMG_ALIEN4);
	monster_img_alien4[WEST] = load_image(MONSTER_IMG_ALIEN4);
	monster_img_alien4[SOUTH] = load_image(MONSTER_IMG_ALIEN4);
	monster_img_alien4[EAST] = load_image(MONSTER_IMG_ALIEN4);
}

void monster_unload() {
	for (int i = 0; i < 4; i++) {
		SDL_FreeSurface(monster_img_norm[i]);
		SDL_FreeSurface(monster_img_alien1[i]);
		SDL_FreeSurface(monster_img_alien2[i]);
		SDL_FreeSurface(monster_img_alien3[i]);
		SDL_FreeSurface(monster_img_alien4[i]);
	}

}

void bombs_load()
{
	bombs = load_image(BOMB);
	bomb[0] = bomb_rect_center;
	bomb[1] = bomb_rect_v;
	bomb[2] = bomb_rect_h;
	bomb[3] = bomb_rect_down;
	bomb[4] = bomb_rect_up;
	bomb[5] = bomb_rect_left;
	bomb[6] = bomb_rect_right;
	bomb[7] = bomb_rect_wait;

	for(int i = 0; i < NB_ANIM_BOMBS; i++) {
		bomb_rect_wait[i].x = 0;
		bomb_rect_wait[i].y = SIZE_OF_SPRITE * i;
		bomb_rect_wait[i].w = SIZE_OF_SPRITE;
		bomb_rect_wait[i].h = SIZE_OF_SPRITE;
	}
	for(int i = 0; i < NB_ANIM_BOMBS; i++) {
		bomb_rect_center[i].x = 7 * SIZE_OF_SPRITE;
		bomb_rect_center[i].y = SIZE_OF_SPRITE * i;
		bomb_rect_center[i].w = SIZE_OF_SPRITE;
		bomb_rect_center[i].h = SIZE_OF_SPRITE;
	}
	for(int i = NB_ANIM_BOMBS; i < 2*NB_ANIM_BOMBS - 1; i++) {
		bomb_rect_center[i].x = 7 * SIZE_OF_SPRITE;
		bomb_rect_center[i].y = bomb_rect_center[2*NB_ANIM_BOMBS-1-i].y;
		bomb_rect_center[i].w = SIZE_OF_SPRITE;
		bomb_rect_center[i].h = SIZE_OF_SPRITE;
	}

	for(int i = 0; i < NB_ANIM_BOMBS; i++) {
		bomb_rect_v[i].x = 5 * SIZE_OF_SPRITE;
		bomb_rect_v[i].y = SIZE_OF_SPRITE * i;
		bomb_rect_v[i].w = SIZE_OF_SPRITE;
		bomb_rect_v[i].h = SIZE_OF_SPRITE;
	}
	for(int i = NB_ANIM_BOMBS; i < 2*NB_ANIM_BOMBS - 1; i++) {
		bomb_rect_v[i].x = 5 * SIZE_OF_SPRITE;
		bomb_rect_v[i].y = bomb_rect_v[2*NB_ANIM_BOMBS-1-i].y;
		bomb_rect_v[i].w = SIZE_OF_SPRITE;
		bomb_rect_v[i].h = SIZE_OF_SPRITE;
	}

	for(int i = 0; i < NB_ANIM_BOMBS; i++) {
		bomb_rect_up[i].x = 1 * SIZE_OF_SPRITE;
		bomb_rect_up[i].y = SIZE_OF_SPRITE * i;
		bomb_rect_up[i].w = SIZE_OF_SPRITE;
		bomb_rect_up[i].h = SIZE_OF_SPRITE;
	}
	for(int i = NB_ANIM_BOMBS; i < 2*NB_ANIM_BOMBS - 1; i++) {
		bomb_rect_up[i].x = 1 * SIZE_OF_SPRITE;
		bomb_rect_up[i].y = bomb_rect_up[2*NB_ANIM_BOMBS-1-i].y;
		bomb_rect_up[i].w = SIZE_OF_SPRITE;
		bomb_rect_up[i].h = SIZE_OF_SPRITE;
	}

	for(int i = 0; i < NB_ANIM_BOMBS; i++) {
		bomb_rect_down[i].x = 2 * SIZE_OF_SPRITE;
		bomb_rect_down[i].y = SIZE_OF_SPRITE * i;
		bomb_rect_down[i].w = SIZE_OF_SPRITE;
		bomb_rect_down[i].h = SIZE_OF_SPRITE;
	}
	for(int i = NB_ANIM_BOMBS; i < 2*NB_ANIM_BOMBS - 1; i++) {
		bomb_rect_down[i].x = 2 * SIZE_OF_SPRITE;
		bomb_rect_down[i].y = bomb_rect_down[2*NB_ANIM_BOMBS-1-i].y;
		bomb_rect_down[i].w = SIZE_OF_SPRITE;
		bomb_rect_down[i].h = SIZE_OF_SPRITE;
	}

	for(int i = 0; i < NB_ANIM_BOMBS; i++) {
		bomb_rect_left[i].x = 3 * SIZE_OF_SPRITE;
		bomb_rect_left[i].y = SIZE_OF_SPRITE * i;
		bomb_rect_left[i].w = SIZE_OF_SPRITE;
		bomb_rect_left[i].h = SIZE_OF_SPRITE;
	}
	for(int i = NB_ANIM_BOMBS; i < 2*NB_ANIM_BOMBS - 1; i++) {
		bomb_rect_left[i].x = 3 * SIZE_OF_SPRITE;
		bomb_rect_left[i].y = bomb_rect_left[2*NB_ANIM_BOMBS-1-i].y;
		bomb_rect_left[i].w = SIZE_OF_SPRITE;
		bomb_rect_left[i].h = SIZE_OF_SPRITE;
	}

	for(int i = 0; i < NB_ANIM_BOMBS; i++) {
		bomb_rect_right[i].x = 4 * SIZE_OF_SPRITE;
		bomb_rect_right[i].y = SIZE_OF_SPRITE * i;
		bomb_rect_right[i].w = SIZE_OF_SPRITE;
		bomb_rect_right[i].h = SIZE_OF_SPRITE;
	}
	for(int i = NB_ANIM_BOMBS; i < 2*NB_ANIM_BOMBS - 1; i++) {
		bomb_rect_right[i].x = 4 * SIZE_OF_SPRITE;
		bomb_rect_right[i].y = bomb_rect_right[2*NB_ANIM_BOMBS-1-i].y;
		bomb_rect_right[i].w = SIZE_OF_SPRITE;
		bomb_rect_right[i].h = SIZE_OF_SPRITE;
	}

	for(int i = 0; i < NB_ANIM_BOMBS; i++) {
		bomb_rect_h[i].x = 6 * SIZE_OF_SPRITE;
		bomb_rect_h[i].y = SIZE_OF_SPRITE * i;
		bomb_rect_h[i].w = SIZE_OF_SPRITE;
		bomb_rect_h[i].h = SIZE_OF_SPRITE;
	}
	for(int i = NB_ANIM_BOMBS; i < 2*NB_ANIM_BOMBS - 1; i++) {
		bomb_rect_h[i].x = 6 * SIZE_OF_SPRITE;
		bomb_rect_h[i].y = bomb_rect_h[2*NB_ANIM_BOMBS-1-i].y;
		bomb_rect_h[i].w = SIZE_OF_SPRITE;
		bomb_rect_h[i].h = SIZE_OF_SPRITE;
	}
}

void bombs_unload()
{
	SDL_FreeSurface(bombs);
}

void sprite_load() {
	map_load();
	bonus_load();
	banner_load();
	player_load();
	monster_load();
	bombs_load();
	menu_load();
}

void sprite_free() {
	map_unload();
	bonus_unload();
	banner_unload();
	player_unload();
	monster_unload();
	bombs_unload();
	menu_unload();
}

SDL_Surface* sprite_get_bombs() {
	return bombs;
}

// Sprite function

SDL_Rect sprite_get_rect_bomb_anim(int i, int j) {
	assert(0 <= i && 0 <= j && i < SIZE_TAB_ANIM_BOMB && j < 2*NB_ANIM_BOMBS);
	SDL_Rect* rect = bomb[i];
	return rect[j];
}

SDL_Surface* sprite_get_number(short number) {
	assert(number >= 0 && number <= 10);
	return numbers[number];
}

SDL_Surface* sprite_get_number_white(short number) {
	assert(number >= 0 && number <= 10);
	return numbers_w[number];
}

SDL_Surface* sprite_get_players(int id) {
	return players[id-1];
}

SDL_Rect sprite_get_rect_player_anim(int i, int id, enum way direction) {
	SDL_Rect* rect = player[id-1][direction];
	return rect[i];
}

SDL_Surface* sprite_get_monster(m_type type, enum way direction) {
	SDL_Surface** monster = monsters[type];
	return monster[direction];
}

SDL_Surface* sprite_get_banner_life() {
	assert(banner_life);
	return banner_life;
}

SDL_Surface* sprite_get_banner_bomb() {
	assert(banner_bomb);
	return banner_bomb;
}

SDL_Surface* sprite_get_banner_line() {
	assert(banner_line);
	return banner_line;
}

SDL_Surface* sprite_get_banner_range() {
	assert(banner_range);
	return banner_range;
}

SDL_Surface* sprite_get_bonus(bonus_type_t bonus_type) {
	assert(bonus[bonus_type-1]);
	return bonus[bonus_type-1];
}

SDL_Surface* sprite_get_menu(select_menu_t select_menu) {
	//assert(menu[select_menu]);
	return menu[select_menu];
}

SDL_Surface* sprite_get_score(int i) {
	assert(i >= 0 && i < NB_TXT_SCORE);
	return score[i];
}

SDL_Surface* sprite_get_map_multi(int i) {
	assert(i < nb_map_multi);
	return map_multi[i];
}

int sprite_get_nb_map_multi() {
	return nb_map_multi;
}

int sprite_get_max_width() {
	return max_width;
}

SDL_Rect* sprite_get_rect_stars() {
	return(menu_rect_stars);
}

SDL_Surface* sprite_get_tree() {
	assert(tree);
	return tree;
}

SDL_Surface* sprite_get_box() {
	assert(box);
	return box;
}

SDL_Surface* sprite_get_key() {
	assert(key);
	return key;
}

SDL_Surface* sprite_get_flag() {
	assert(flag);
	return flag;
}

SDL_Surface* sprite_get_princess() {
	assert(princess);
	return princess;
}

SDL_Surface* sprite_get_empty() {
	assert(empty);
	return empty;
}

SDL_Surface* sprite_get_wall_b() {
	assert(wall_b);
	return wall_b;
}

SDL_Surface* sprite_get_wall_g() {
	assert(wall_g);
	return wall_g;
}

SDL_Surface* sprite_get_wall_h() {
	assert(wall_h);
	return wall_h;
}

SDL_Surface* sprite_get_wall_d() {
	assert(wall_d);
	return wall_d;
}

SDL_Surface* sprite_get_angle_bg() {
	assert(angle_bg);
	return angle_bg;
}

SDL_Surface* sprite_get_angle_bd() {
	assert(angle_bd);
	return angle_bd;
}

SDL_Surface* sprite_get_angle_hg() {
	assert(angle_hg);
	return angle_hg;
}

SDL_Surface* sprite_get_angle_hd() {
	assert(angle_hd);
	return angle_hd;
}

SDL_Surface* sprite_get_bloc() {
	assert(bloc);
	return bloc;
}

SDL_Surface* sprite_get_stone() {
	assert(stone);
	return stone;
}

SDL_Surface* sprite_get_door() {
	assert(door);
	return door;
}

SDL_Surface* sprite_get_closed_door() {
	assert(closed_door);
	return closed_door;
}
