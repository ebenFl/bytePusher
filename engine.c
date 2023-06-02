#include <stdio.h>
#include <stdlib.h>
#include "graphics.h"

#define MEMSIZE 0x1000008

// first two bytes of mem are the keyboard state [0-1]
// pc is at the third byte and addresses are 3 bytes long [2-4]
// value of zz means pixel(xx, yy) is at address zzxxyy [5]
// value of xxyy means: audio sample zz is at address xxyyzz [6-7]
unsigned char mem[MEMSIZE] = {0};
unsigned char *pc = NULL;

pixel screen[DIM][DIM];
SDL_Rect pixels[DIM][DIM];

void load_mem(unsigned char *mem, char *fn)
{

    FILE *fp;
    fp = fopen(fn, "r");
    if (!fp)
    {
        printf("Could not read file %s\n", fn);
        exit(1);
    }
    else
    {
        /* Determine file size */
        fseek(fp, 0, SEEK_END);
        size_t size = ftell(fp);
        fseek(fp, 0, SEEK_SET);

        if (size > MEMSIZE)
        {
            printf("File too large!");
            exit(1);
        }
        // read file into memory
        fread(mem, size, 1, fp);
    }
}

/*
    one 'cpu cycle' for bytepusher
    is doing the bytepush instruction 65536 times
    the byte push instruction involves 3 big-endian 24 bit addresses
    A,B,C at the pc
    copy 1 byte form A to B
    then Jump to C
*/
void cycle(unsigned char *mem, unsigned char *pc)
{
    // start at mem[2] since first two bytes are reserved for keys
    pc = mem + (mem[2] << 16 | mem[3] << 8 | mem[4]);
    int i = 65536;
    do
    {
        // read byte at address A into B
        mem[pc[3] << 16 | pc[4] << 8 | pc[5]] =
            mem[pc[0] << 16 | pc[1] << 8 | pc[2]];
        // jump to C
        pc = mem + (pc[6] << 16 | pc[7] << 8 | pc[8]);
    } while (--i);
}

int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        printf("Must provide program to run!");
        return 1;
    }
    else
    {
        load_mem(mem, argv[1]);
        g_init();
        while(g_poll(mem)) {
            cycle(mem,pc);
            g_draw(mem,screen);
            SDL_Delay(16);
        }
        g_cleanup();
        return 0;
    }
}