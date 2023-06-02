#ifndef _GRAPHICSH_
#define _GRAPHICSH_
#include <stdio.h>
#include <SDL2/SDL.h>

#define DIM 256 /* screen dimensions (which will be scaled by two) */

typedef struct pixel {
    unsigned char a; 
    unsigned char r; 
    unsigned char g; 
    unsigned char b; 
} pixel;

// keep these static so the functions can 
// handle them without them passed through as params
static SDL_Window *window = NULL;
static SDL_Renderer * renderer = NULL;
static SDL_Texture* texture = NULL;

/* BytePusher uses a hexadecimal keymapping */
static const SDL_Keycode keymap[] = { 
	SDLK_0, SDLK_1, SDLK_2, SDLK_3, SDLK_4, SDLK_5, SDLK_6, SDLK_7,
	SDLK_8, SDLK_9, SDLK_a, SDLK_b, SDLK_c, SDLK_d, SDLK_e, SDLK_f,
};


void g_init();

void g_cleanup();

int g_poll(unsigned char* mem);

void g_draw(unsigned char* mem, pixel screen[][DIM]);

#endif