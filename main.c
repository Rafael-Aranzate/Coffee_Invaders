/*IMPORTANTE
CORRIGIR O ERRO NA LOGICA DO TIRO, FAZER O MENU, TESTAR OS TEXTOS NA TELA E CRIAR AS HITBOXES DE ENTIDADES
*/

#include <stdio.h>
#include <stdlib.h>
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_mixer.h>
#include <SDL_ttf.h>
#include <stdbool.h>
#include <time.h>

#define SCREEN_W 1024
#define SCREEN_H 768
#define VELOCIDADE_JOGADOR 8
#define VELOCIDADE_INIMIGO 5
#define VELOCIDADE_TIRO 20
#define TECLAS 350

typedef struct {
    SDL_Renderer *renderer;
    SDL_Window *janela;
    int teclado[TECLAS];
} OUTPUT;

typedef struct {
	int x;
	int y;
    int w;
    int h;
    int dx;
	int dy;
	int vida;
    SDL_Rect* hitbox;
	SDL_Texture *textura;
} ENTIDADE;

OUTPUT output;
TTF_Font* BebasNeue;
ENTIDADE* menu;
ENTIDADE* background;
ENTIDADE* jogador;
ENTIDADE* tiro;

SDL_Color branco = {255, 255, 255};

void iniciar(char* titulo);
void input(void);
void KeyDown(SDL_KeyboardEvent* evento);
void KeyUp(SDL_KeyboardEvent* evento);
void imprimirTextura(ENTIDADE* entidade, int x, int y, int w, int h);
void defMenu();
void defBackground();
void defJogador();
void defHitbox();
static void atirar();
void mensagem(char* texto, int valor,SDL_Color cor, int x, int y, int w, int h);

bool jogando = false;
bool stateMenu = true;

int main(int argc, char** argv) {

    char titulo[50] = "Coffee Invaders";
    int stage = 0;
    unsigned long long int pontuacao;

    tiro =  malloc(sizeof(ENTIDADE));
    tiro->dx = 0;
    tiro->dy = -VELOCIDADE_TIRO;
    tiro->w = 9;
    tiro->h = 54;
    tiro->textura = IMG_Load("/home/rafael_aranzate/Desktop/Comp_I/Jogo/Sprites/PNG/Lasers/laserRed01.png");

    iniciar(titulo);
    BebasNeue = TTF_OpenFont("/home/rafael_aranzate/Desktop/Comp_I/Jogo/BebasNeue-Regular.ttf", 24);
    SDL_SetRenderDrawColor(output.renderer, 111, 133, 255, 255);

    defBackground();
    defJogador();
    defMenu();
    atirar();
    SDL_RenderPresent(output.renderer);

    while(stateMenu) {
        SDL_RenderClear(output.renderer);
        input();
        imprimirTextura(menu, menu->x, menu->y, menu->w, menu->h);
        mensagem("COFFEE INVADERS", NULL, branco, SCREEN_W / 4, SCREEN_H / 3, 400, 50);
        if(output.teclado[SDL_SCANCODE_RETURN]) {
            stateMenu = false;
            free(menu);
            jogando = true;
        }
        SDL_RenderPresent(output.renderer);
        SDL_Delay(16);
    }

    while (jogando) {
        SDL_RenderClear(output.renderer);
        if(stage > 0) {
            imprimirTextura(background, background->x, background->y, background->w, background->h);
        }
        input();

        if (output.teclado[SDL_SCANCODE_UP]) {
			jogador->y -= VELOCIDADE_JOGADOR;
		}
        //Limite de borda eixo y
        if (output.teclado[SDL_SCANCODE_UP] && jogador->y <= 0) {
			jogador->y += VELOCIDADE_JOGADOR;
		}
		if (output.teclado[SDL_SCANCODE_DOWN]) {
			jogador->y += VELOCIDADE_JOGADOR;
		}
        //Limite de borda eixo -y
        if (output.teclado[SDL_SCANCODE_DOWN] && (jogador->y + jogador->h) >= SCREEN_H) {
			jogador->y -= VELOCIDADE_JOGADOR;
		}

		if (output.teclado[SDL_SCANCODE_LEFT]) {
			jogador->x -= VELOCIDADE_JOGADOR;
		}
        //Limite de borda eixo -x
        if (output.teclado[SDL_SCANCODE_LEFT] && jogador->x <= 0) {
			jogador->x += VELOCIDADE_JOGADOR;
		}
		if (output.teclado[SDL_SCANCODE_RIGHT]) {
			jogador->x += VELOCIDADE_JOGADOR;
		}
        //Limite de borda eixo x
        if (output.teclado[SDL_SCANCODE_RIGHT] && (jogador->x + jogador->w) >= SCREEN_W) {
			jogador->x -= VELOCIDADE_JOGADOR;
		}
        if (output.teclado[SDL_SCANCODE_Z]) {
            tiro->vida = 1;

		}
        tiro->x += tiro->dx;
		tiro->y += tiro->dy;
        if (tiro->vida == 1) {
			imprimirTextura(tiro, tiro->x, tiro->y, tiro->w, tiro->h);
        }
        if (tiro->y < 0) {
            tiro->x = jogador->x + 50;
            tiro->y = jogador->y - 35;
			tiro->vida = 0;
		}
        imprimirTextura(jogador, jogador->x, jogador->y, jogador->w, jogador->h);
        mensagem("Vida: ", jogador->vida ,branco, 10, 10, SCREEN_W / 9, SCREEN_H / 13);
        mensagem("Pontos: ", pontuacao, branco, SCREEN_W / 2, 10, SCREEN_W / 9, SCREEN_H / 13);
        SDL_RenderPresent(output.renderer);
        SDL_Delay(16);
    }
    SDL_Quit();
}

void iniciar(char* titulo) {
    SDL_Init(SDL_INIT_EVERYTHING);

    int TTF_Init();
    if(TTF_Init()==-1) {
        printf("TTF_Init: %s\n", TTF_GetError());
        exit(2);
    }

    output.janela = SDL_CreateWindow(titulo, 0, 0, SCREEN_W, SCREEN_H, SDL_WINDOW_OPENGL);

    output.renderer = SDL_CreateRenderer(output.janela, -1, 0);
}

void input(void) {
    SDL_Event evento;

    while (SDL_PollEvent(&evento)) {

        if(evento.type == SDL_QUIT) {
            stateMenu = false;
            jogando = false;
        }
        switch (evento.type) {
            case SDL_KEYDOWN:
                KeyDown(&evento.key);
                break;
            case SDL_KEYUP:
                KeyUp(&evento.key);
                break;
            default:
                break;
        }
    }
}


void KeyUp(SDL_KeyboardEvent *evento){
	if (evento->repeat == 0) {

        output.teclado[evento->keysym.scancode] = 0;

        if (evento->keysym.scancode == SDL_SCANCODE_ESCAPE) {
            menu = false;
			jogando = false;
		}
	}
}

void KeyDown(SDL_KeyboardEvent *evento){
	if (evento->repeat == 0 && evento->keysym.scancode < TECLAS) {
		
        output.teclado[evento->keysym.scancode] = 1;

        if (evento->keysym.scancode == SDL_SCANCODE_ESCAPE) {
            stateMenu = false;
			jogando = false;
		}
	}
}

void defMenu() {
    menu = malloc(sizeof(ENTIDADE));

    menu->textura = IMG_Load("/home/rafael_aranzate/Desktop/Comp_I/Jogo/menu.jpg");
    menu->x = 0;
    menu->y = 0;
    menu->w = SCREEN_W;
    menu->h = SCREEN_H;
}

void defBackground() {
    background = malloc(sizeof(ENTIDADE));

    background->textura = IMG_Load("/home/rafael_aranzate/Desktop/Comp_I/Jogo/Sprites/Backgrounds/purple.png");
    background->x = 0;
    background->y = 0;
    background->w = SCREEN_W;
    background->h = SCREEN_H;
}

void defJogador() {
    jogador = malloc(sizeof(ENTIDADE));

    jogador->textura = IMG_Load("/home/rafael_aranzate/Desktop/Comp_I/Jogo/Sprites/PNG/player.png");
    jogador->x = SCREEN_W / 2 - 20;
    jogador->y = SCREEN_H - 100;
    jogador->w = SCREEN_W / 10;
    jogador->h = SCREEN_H / 10;
    jogador->vida = 10;


}

void defHitbox() {

}

static void atirar() {

    
}

void imprimirTextura(ENTIDADE* entidade, int x, int y, int w, int h) {
    SDL_Texture* surfaceTextura = SDL_CreateTextureFromSurface(output.renderer, entidade->textura);
    SDL_Rect rect;
    rect.x = x;
    rect.y = y;
    rect.w = w;
    rect.h = h;

    SDL_RenderCopy(output.renderer, surfaceTextura, NULL, &rect);
    SDL_DestroyTexture(surfaceTextura);
}

void mensagem(char* texto, int valor ,SDL_Color cor, int x, int y, int w, int h) {

    //sprintf(intToStr, "%d", valor);

    SDL_Surface* textoSurface = TTF_RenderText_Blended(BebasNeue, texto, cor);
    SDL_Texture* texto1 = SDL_CreateTextureFromSurface(output.renderer, textoSurface);

    SDL_Rect texto_rect;

    texto_rect.x = x;
    texto_rect.y = y;
    texto_rect.w = w;
    texto_rect.h = h;

    SDL_RenderCopy(output.renderer, texto1, NULL, &texto_rect);
    SDL_DestroyTexture(texto1);
    SDL_FreeSurface(textoSurface);
}
