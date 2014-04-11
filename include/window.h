#ifndef WINDOW_H_
#define WINDOW_H_

#include <SDL/SDL.h>

void window_create(int width, int height);
void window_free();
void window_resize(int width, int height);
void window_refresh();

// display a SDL_Surface at location (x,y)
void window_display_image(SDL_Surface* surface, int x, int y);
void window_display_sprite(SDL_Surface* sprite, SDL_Rect rect, int x, int y);

void window_write(int x, int y);

// set every pixel of the window to white
void window_clear();

#endif /* WINDOW_H_ */
