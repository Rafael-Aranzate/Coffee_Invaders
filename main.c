#include <stdio.h>
#include <stdbool.h>
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_mixer.h>
#define TELA_W 1024
#define TELA_H 768

bool init();
bool carregarMidia();
void fechar();

SDL_Window* janela = NULL;
SDL_Surface*  superficie = NULL;
SDL_Surface* imagem = NULL;
SDL_Texture* textura = NULL;
SDL_Event evento;

struct sprite {
    SDL_Texture *textura;
    int w, h;
};

enum KeyPressSurfaces {
    KEY_PRESS_SURFACE_DEFAULT,
    KEY_PRESS_SURFACE_UP,
    KEY_PRESS_SURFACE_DOWN,
    KEY_PRESS_SURFACE_LEFT,
    KEY_PRESS_SURFACE_RIGHT,
    KEY_PRESS_SURFACE_TOTAL
};

int main(int argc, char** argv) {

    char titulo[50] = "Jogo - PE";

    SDL_Renderer* renderer;
    SDL_Rect sRect = {1, 26, 16, 32}, dRect = {312,136, 18, 34};

    //Inicializar SDL, com o titulo
    init(titulo);
    //Carregar midias
    carregarMidia();
    //Aplica a imagem na tela
    SDL_BlitSurface(janela, NULL, superficie, NULL);
    //Atualiza a superficie da tela
    SDL_UpdateWindowSurface(janela);

    renderer = SDL_CreateRenderer(janela, -1, 0);

    textura = SDL_CreateTextureFromSurface(renderer, imagem);

    struct sprite player = {textura, imagem->w, imagem->h};

    bool jogando = true;
    SDL_Color fundo =  {111, 133, 255, 255};

    while(jogando) {

        //SDL_Delay(1000/Limite -Δt);

        SDL_Event evento;

        while (SDL_PollEvent(&evento)) {
        switch (evento.type) {
            case SDL_QUIT:
                jogando = false;
                break;
            case SDL_KEYDOWN:
                if(evento.key.keysym.sym == SDLK_ESCAPE)
                    jogando = false;
                break;
            default:
                break;
            }
        }

        SDL_Rect destino = {0,0,player.w,player.h};

        SDL_BlitSurface(imagem, NULL, superficie, NULL);
        SDL_UpdateWindowSurface(janela);
    }

    //Libera recursos e termina o SDL
    fechar();
    return 0;
}

bool init(char* titulo) {

    bool inicializacao = true;

    SDL_Init(SDL_INIT_EVERYTHING);

    if(SDL_Init( SDL_INIT_EVERYTHING) < 0) {
        printf( "Nao foi possivel inicializar o SDL! SDL_Error: %s\n", SDL_GetError());
        inicializacao = false;
    }

    janela = SDL_CreateWindow(titulo, TELA_W, TELA_H, 1024, 768, SDL_WINDOW_RESIZABLE);

    if(janela == NULL) {
            printf("Erro a criar janela! SDL_Error: %s\n", SDL_GetError());
        }

    SDL_GetWindowSurface(janela);

    return inicializacao;
}

bool carregarMidia() {

    bool inicializacao = true;

    imagem = IMG_Load("/home/rafael_aranzate/Desktop/Comp_I/Jogo/Sprites/Mario_Sprites.png");
    if(imagem == NULL) {
        printf("Nao foi possivel carregar a imagem %s! SDL Error: %s\n", "Mario_Sprites.png", SDL_GetError());
        inicializacao = false;
        }

    return inicializacao;
}

bool input(SDL_Event evento, bool* jogando) {

    while (SDL_PollEvent(&evento)) {
        switch (evento.type) {
            case SDL_QUIT:
                jogando = false;
                break;
            case SDL_KEYDOWN:
                if(evento.key.keysym.sym == SDLK_ESCAPE)
                    jogando = false;
                break;
            default:
                break;
            }
        }
        return jogando;
}

void fechar() {

    SDL_FreeSurface(superficie);
    superficie = NULL;

    SDL_DestroyWindow(janela);
    janela = NULL;

    SDL_Quit();
}
//gcc -o main main.c `sdl2-config --cflags --libs` -lSDL2 -lSDL2_image
