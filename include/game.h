#ifndef GAME_H_
#define GAME_H_

#include <player.h>
#include <level.h>
#include <constant.h>

struct game;

// Create a new game
struct game* game_new();

struct level* game_next_lvl(struct game* game);

// Free a game
void game_free(struct game* game);

// Return the player of the current game
struct player* game_get_player(struct game* game);

// Return the current level
struct level* game_get_curr_level(struct game* game);

// SDL_GetTicks minus the pause time
int game_get_frame(struct game* game);

// Display the game of the screen
void game_display(struct game* game);

// update
enum state game_update(enum state state, struct game* game, int key, key_event_t key_event);

#endif /* GAME_H_ */
