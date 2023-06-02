
#include "graphics.h"

/* Initialize video subsystem */
void g_init()
{
    if (SDL_Init(SDL_INIT_AUDIO | SDL_INIT_VIDEO) < 0)
    {
        printf("SDL_Init failed: %s\n", SDL_GetError());
        exit(1);
    }
    else
    {
        window = SDL_CreateWindow("Bytepusher", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, DIM * 2, DIM * 2, SDL_WINDOW_SHOWN | SDL_WINDOW_ALWAYS_ON_TOP);
        renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
        texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_ARGB32, SDL_TEXTUREACCESS_STREAMING, DIM, DIM);
        SDL_RenderSetLogicalSize(renderer, DIM, DIM);
    }
}

/* cleanup function*/
void g_cleanup()
{
    SDL_DestroyTexture(texture);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
}

/* poll input */
int g_poll(unsigned char *mem)
{
    SDL_Event e;
    short inputdata = 0x0;
    unsigned int i;

    // keyboard input stored in first two bytes of ram
    // since the odering of the machine is big endian we have to flip components
    inputdata = mem[0] << 8 | mem[1];

    while (SDL_PollEvent(&e))
    {
        switch (e.type)
        {
        case SDL_QUIT:
            return 0;
        case SDL_KEYDOWN:
        case SDL_KEYUP:
            for (i = 0; i < sizeof(keymap) / sizeof(*keymap); i++)
            {
                if (e.key.keysym.sym == keymap[i])
                {
                    inputdata = inputdata ^ (1 << i);
                }
                else if (e.key.keysym.sym == SDLK_ESCAPE)
                {
                    return 0;
                }
            }
            break;
        }
    }
    // write the new key data back to memory
    // in big endian format
    mem[0] = inputdata >> 8;
    mem[1] = inputdata & 0xFF;
    return 1;
}

void g_draw(unsigned char *mem, pixel screen[][DIM])
{
    SDL_RenderClear(renderer);

    int x, y;
    for (x = 0; x < DIM; x++)
    {
        for (y = 0; y < DIM; y++)
        {
            unsigned char px = mem[(mem[5] << 16) | (y << 8) | x]; // grab pixel value
            // colors are stored in 6 * 6 * 6 color cube
            // red * 36 + green*6 + blue
            // that each have an intensity value of 0-5
            // to extract the intensity we mod by six
            // and to extract the next color we divide by 6
            // also since our color components are 8 bits long for pixel
            // we multiply by 0x33 according to the docs
            screen[y][x].b = (px % 6) * 0x33;
            px /= 6;
            screen[y][x].g = (px % 6) * 0x33;
            px /= 6;
            screen[y][x].r = (px % 6) * 0x33;
            screen[y][x].a = 0xFF;
        }
    }

    SDL_UpdateTexture(texture, NULL, screen, DIM * sizeof(pixel));

    SDL_RenderCopy(renderer, texture, NULL, NULL);
    SDL_RenderPresent(renderer);
}
