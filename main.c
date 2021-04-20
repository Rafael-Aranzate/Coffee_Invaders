#include <stdio.h>
#include <SDL.h>

char titulo[50] = "jogo";

int main(int argc, char** argv) {

    bool acabou = false;

    SDL_Window* janela;
    SDL_Renderer* renderer;
    SDL_Texture* textura;

    SDL_Init(SDL_INIT_EVERYTHING);

    janela = SDL_CreateWindow(titulo, 1024, 768, 1024, 768, SDL_WINDOW_RESIZABLE);

    renderer = SDL_CreateRenderer(janela, -1, SDL_RENDERER_ACCELERATED);

    SDL_Surface* temporaria = SDL_LoadBMP("img.bmp");

    SDL_DestroyWindow(janela);
    
    SDL_Quit();
    return 0;
}
//gcc -o main main.c `sdl2-config --cflags --libs`
