#include <stdio.h>
#include <stdbool.h>
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_mixer.h>
#define TELA_W 1024
#define TELA_H 768

//Botoes a serem utilizados
enum KeyPressSurfaces {
    KEY_PRESS_SURFACE_DEFAULT,
    KEY_PRESS_SURFACE_UP,
    KEY_PRESS_SURFACE_DOWN,
    KEY_PRESS_SURFACE_LEFT,
    KEY_PRESS_SURFACE_RIGHT,
    KEY_PRESS_SURFACE_TOTAL
};

//Inicializar SDL, com o titulo
bool init(char* titulo);
//Carregar midias
bool carregarMidia();
void fechar();

//Carrega imagens individuais
SDL_Surface* carregarSuperficie(char* path);
//janela
SDL_Window* janela = NULL;
//A superficie exposta na janela
SDL_Surface* superficie = NULL;
//Imagem correspondente a um botao pressionado
SDL_Surface* imagemTecla[KEY_PRESS_SURFACE_TOTAL];
//Imagem atual
SDL_Surface* imagemAtual = NULL;

struct sprite {
    SDL_Texture *textura;
    int w, h;
};

int main(int argc, char** argv) {

    char titulo[50] = "Jogo - PE";

 
    init(titulo);
    
    carregarMidia();

    bool jogando = true;
   imagemAtual = imagemTecla[KEY_PRESS_SURFACE_DEFAULT];

    while(jogando) {

        SDL_Event evento;

        while (SDL_PollEvent(&evento)) {
            if(evento.type == SDL_QUIT) {
                jogando = false;
            }
            else if(evento.type == SDL_KEYDOWN) {
                    switch (evento.key.keysym.sym) {
                        case SDLK_ESCAPE:
                            jogando = false;
                            break;
                        case SDLK_UP:
                            imagemAtual = imagemTecla[KEY_PRESS_SURFACE_UP];
                            break;
                        case SDLK_DOWN:
                            imagemAtual = imagemTecla[KEY_PRESS_SURFACE_DOWN];
                            break;
                        case SDLK_LEFT:
                            imagemAtual = imagemTecla[KEY_PRESS_SURFACE_LEFT];
                            break;
                        case SDLK_RIGHT:
                            imagemAtual = imagemTecla[KEY_PRESS_SURFACE_RIGHT];
                            break;
                        default:
                            imagemAtual = imagemTecla[KEY_PRESS_SURFACE_DEFAULT];
                            break;
                        }
            }
        }

        SDL_Delay(17);

        //Aplica a imagem a superficie
        SDL_BlitSurface(imagemAtual, NULL, superficie, NULL);

        //Atualiza a superficie
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

    superficie = SDL_GetWindowSurface(janela);

    return inicializacao;
}

bool carregarMidia() {

    bool inicializacao = true;

    imagemTecla[KEY_PRESS_SURFACE_DEFAULT] = carregarSuperficie("/home/rafael_aranzate/Desktop/Comp_I/Jogo/Sprites/Mario_Sprites.png");

    if (imagemTecla[KEY_PRESS_SURFACE_DEFAULT] == NULL) {
        printf("Nao foi possivel carregar a imagem padrao %s! SDL Error: %s\n", "Mario_Sprites.png", SDL_GetError());
        inicializacao = false;
        }
    
    imagemTecla[KEY_PRESS_SURFACE_UP] = carregarSuperficie("/home/rafael_aranzate/Desktop/Comp_I/Jogo/Sprites/up.png");

    if (imagemTecla[KEY_PRESS_SURFACE_UP] == NULL) {
        printf("Nao foi possivel carregar a imagem up %s! SDL Error: %s\n", "Mario_Sprites.png", SDL_GetError());
        inicializacao = false;
        }

    imagemTecla[KEY_PRESS_SURFACE_DOWN] = carregarSuperficie("/home/rafael_aranzate/Desktop/Comp_I/Jogo/Sprites/down.png");

    if (imagemTecla[KEY_PRESS_SURFACE_DOWN] == NULL) {
        printf("Nao foi possivel carregar a imagem down %s! SDL Error: %s\n", "Mario_Sprites.png", SDL_GetError());
        inicializacao = false;
        }
    
    imagemTecla[KEY_PRESS_SURFACE_LEFT] = carregarSuperficie("/home/rafael_aranzate/Desktop/Comp_I/Jogo/Sprites/left.png");

    if (imagemTecla[KEY_PRESS_SURFACE_LEFT] == NULL) {
        printf("Nao foi possivel carregar a imagem left %s! SDL Error: %s\n", "Mario_Sprites.png", SDL_GetError());
        inicializacao = false;
        }

    imagemTecla[KEY_PRESS_SURFACE_RIGHT] = carregarSuperficie("/home/rafael_aranzate/Desktop/Comp_I/Jogo/Sprites/right.png");

    if (imagemTecla[KEY_PRESS_SURFACE_RIGHT] == NULL) {
        printf("Nao foi possivel carregar a imagem right %s! SDL Error: %s\n", "Mario_Sprites.png", SDL_GetError());
        inicializacao = false;
        }

    imagemTecla[KEY_PRESS_SURFACE_DEFAULT] = carregarSuperficie("/home/rafael_aranzate/Desktop/Comp_I/Jogo/Sprites/Mario_Sprites.png");

    return inicializacao;
}

SDL_Surface* carregarSuperficie(char* path) {
    SDL_Surface* imagemCarregada =  IMG_Load(path);

     if (imagemCarregada == NULL)
    {
        printf( "Nao foi possivel carregar a imagem %s! SDL Error: %s\n", path, SDL_GetError() );
    }

    return imagemCarregada;
}

void fechar() {

    SDL_FreeSurface(superficie);
    superficie = NULL;

    SDL_DestroyWindow(janela);
    janela = NULL;

    SDL_Quit();
}
//gcc -o main main.c `sdl2-config --cflags --libs` -lSDL2 -lSDL2_image
