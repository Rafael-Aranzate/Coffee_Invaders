#include <stdio.h>
#include <stdlib.h>
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_mixer.h>
#include <SDL_ttf.h>
#include <stdbool.h>
#include <time.h>

#define SCREEN_W 1280
#define SCREEN_H 720
#define VELOCIDADE_JOGADOR 8
#define VELOCIDADE_INIMIGO 5
#define VELOCIDADE_TIRO 10
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

	SDL_Texture *textura;
} ENTIDADE;

OUTPUT output;
TTF_Font* BebasNeue;
ENTIDADE* menu;
ENTIDADE* background;
ENTIDADE* jogador;
ENTIDADE* inimigo1;
ENTIDADE* tiro;

SDL_Color branco = {255, 255, 255};

//INICALIZA TODOS OS COMPONENTES DO SDL
void iniciar(char* titulo);
//GERENCIA OS COMANDOS
void input(void);
//KEYDOWN E KEYUP GERENCIAM AS TECLAS
void KeyDown(SDL_KeyboardEvent* evento);
void KeyUp(SDL_KeyboardEvent* evento);
void imprimirTextura(ENTIDADE* entidade, int x, int y, int w, int h);
void defMenu();
void defBackground();
void defJogador();
void defExplosao();
void defTiro();
void defInimigo1();
static int colisaoInimigo(ENTIDADE* entidade1, ENTIDADE* entidade2);
void mensagem(char* texto, char* texto2, SDL_Color cor, double x, double y, double w, double h);

bool jogando = false;
bool stateMenu = true;
bool aberto = true;

int main(int argc, char** argv) {

    char titulo[50] = "Coffee Invaders";
    int stage = 0, spawn = 10;
    unsigned int lastTime = 0, currentTime = 0;
    unsigned long long int pontuacao = 0;
    char pontuacaoString[1000], hp[10], fase[10];


    iniciar(titulo);
    BebasNeue = TTF_OpenFont("/home/rafael_aranzate/Desktop/Comp_I/Jogo/BebasNeue-Regular.ttf", 24);
    SDL_SetRenderDrawColor(output.renderer, 111, 133, 255, 255);

    defBackground();
    defJogador();
    defInimigo1();
    defTiro();
    defMenu();

    SDL_RenderPresent(output.renderer);
    while(aberto) {
        while(stateMenu) {
            SDL_RenderClear(output.renderer);
            input();
            imprimirTextura(menu, menu->x, menu->y, menu->w, menu->h);
            mensagem("COFFEE INVADERS", NULL, branco, SCREEN_W / 2.9, SCREEN_H / 2.8, SCREEN_H / 2, SCREEN_H / 9);
            mensagem("PRESSIONE \"ENTER\" PARA INICIAR", NULL, branco, SCREEN_W / 2.9, SCREEN_H / 1.2, 400, 50);
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
            currentTime = SDL_GetTicks();

            sprintf(pontuacaoString, "Pontos: %d", pontuacao);
            sprintf(hp, "HP: %d", jogador->vida);
            sprintf(fase, "Fase: %d", stage);
            if(stage > 0) {
                imprimirTextura(background, background->x, background->y, background->w, background->h);
            }
            input();
            //SECAO DE MOVIMENTO DO JOGADOR
            //
            
            if (output.teclado[SDL_SCANCODE_UP]) {
                jogador->y -= VELOCIDADE_JOGADOR;
                //hitboxJogador->y -= VELOCIDADE_JOGADOR;
            }
            //Limite de borda eixo y
            if (jogador->y <= 0) {
                jogador->y += VELOCIDADE_JOGADOR;
                //hitboxJogador->y += VELOCIDADE_JOGADOR;
            }
            if (output.teclado[SDL_SCANCODE_DOWN]) {
                jogador->y += VELOCIDADE_JOGADOR;
                //hitboxJogador->y += VELOCIDADE_JOGADOR;
            }
            //Limite de borda eixo -y
            if ((jogador->y + jogador->h) >= SCREEN_H) {
                jogador->y -= VELOCIDADE_JOGADOR;
                //hitboxJogador->y -= VELOCIDADE_JOGADOR;
            }

            if (output.teclado[SDL_SCANCODE_LEFT]) {
                jogador->x -= VELOCIDADE_JOGADOR;
                //hitboxJogador->x -= VELOCIDADE_JOGADOR;
            }
            //Limite de borda eixo -x
            if (jogador->x <= 0) {
                jogador->x += VELOCIDADE_JOGADOR;
                //hitboxJogador->x += VELOCIDADE_JOGADOR;
            }
            if (output.teclado[SDL_SCANCODE_RIGHT]) {
                jogador->x += VELOCIDADE_JOGADOR;
                //hitboxJogador->x += VELOCIDADE_JOGADOR;
            }
            //Limite de borda eixo x
            if ((jogador->x + jogador->w) >= SCREEN_W) {
                jogador->x -= VELOCIDADE_JOGADOR;
                //hitboxJogador->x -= VELOCIDADE_JOGADOR;
            }
            //TIRO DO JOGADOR
            if (output.teclado[SDL_SCANCODE_Z]) {
                tiro->vida = 1;
            }

            if (tiro->y < 0) {
                tiro->vida = 0;
            }
            if(inimigo1->x >= SCREEN_W) {
                inimigo1->x = 0;
            }
            if(inimigo1->y >= SCREEN_H) {
                inimigo1->y = 0;
                jogador->vida++;
            }

            if (colisaoInimigo(jogador, inimigo1) == 1) {
                jogador->x -= VELOCIDADE_JOGADOR;
                if(currentTime > lastTime + 1000) {
                    jogador->vida--;
                    lastTime = currentTime;
                }
            }

            if (colisaoInimigo(jogador, inimigo1) == 2) {
                jogador->x += VELOCIDADE_JOGADOR;
                if(currentTime > lastTime + 1000) {
                    jogador->vida--;
                    lastTime = currentTime;
                }
            }

            if(colisaoInimigo(tiro, inimigo1) == 1) {
                tiro->vida = 0;
                inimigo1->vida--;
            }

            if(tiro->vida == 1) {
                tiro->x += tiro->dx;
                tiro->y += tiro->dy;
            }
            
            inimigo1->x += inimigo1->dx;
            inimigo1->y += inimigo1->dy;
            
            if (tiro->vida == 1) {
                imprimirTextura(tiro, tiro->x, tiro->y, tiro->w, tiro->h);
            }

            if(tiro->vida == 0) {
                //if (currentTime > lastTime + 500) {
                    tiro->x = NULL;
                    tiro->y = NULL;
                    //lastTime = currentTime;
            // }
            }

            if (inimigo1->vida > 0) {
                imprimirTextura(inimigo1, inimigo1->x, inimigo1->y, inimigo1->w, inimigo1->h);
            }
            if(inimigo1->vida == 0) {
                pontuacao+= 100;
                inimigo1->x = NULL;
                inimigo1->y = NULL;
            }

            if(jogador->vida == 0) {
                jogando = false;
                stateMenu = true;
                jogador->vida = 10;
                defMenu();
            }

            imprimirTextura(jogador, jogador->x, jogador->y, jogador->w, jogador->h);
            mensagem(hp, NULL , branco, 10, 10, SCREEN_W / 9, SCREEN_H / 13);
            mensagem(fase, NULL, branco, SCREEN_W / 1.3, 10, SCREEN_W / 9, SCREEN_H / 13);
            mensagem(pontuacaoString, NULL, branco, SCREEN_W / 2.3, 10, SCREEN_W / 9, SCREEN_H / 13);
            SDL_RenderPresent(output.renderer);

            SDL_Delay(16);
        }
    }
    free(background);
    free(jogador);
    free(tiro);
    free(inimigo1);
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
            aberto = false;
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
	if (evento->repeat == 0 && evento->keysym.scancode < TECLAS) {

        output.teclado[evento->keysym.scancode] = 0;

        if (evento->keysym.scancode == SDL_SCANCODE_ESCAPE) {
            menu = false;
			jogando = false;
            aberto = false;
		}
	}
}

void KeyDown(SDL_KeyboardEvent *evento){
	if (evento->repeat == 0 && evento->keysym.scancode < TECLAS) {
		
        output.teclado[evento->keysym.scancode] = 1;

        if (evento->keysym.scancode == SDL_SCANCODE_ESCAPE) {
            stateMenu = false;
			jogando = false;
            aberto = false;
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
    jogador->w = SCREEN_W / 9;
    jogador->h = SCREEN_H / 10;
    jogador->vida = 10;

    SDL_Rect* hitboxJogador = {jogador->x, jogador->y, jogador->w, jogador->h};
}

void defInimigo1() {
    inimigo1 = malloc(sizeof(ENTIDADE));

    inimigo1->textura = IMG_Load("/home/rafael_aranzate/Desktop/Comp_I/Jogo/Sprites/PNG/Enemies/enemyBlack1.png");
    inimigo1->x = SCREEN_W / 2;
    inimigo1->y = SCREEN_H / 2;
    inimigo1->w = SCREEN_W / 10;
    inimigo1->h = SCREEN_H / 10;
    inimigo1->dx = 5;
    inimigo1->dy = 2;
    inimigo1->vida = 10;

    SDL_Rect* hitboxInimigo1 = {inimigo1->x, inimigo1->y, inimigo1->w, inimigo1->h};
}

void defTiro() {
    tiro =  malloc(sizeof(ENTIDADE));

    tiro->textura = IMG_Load("/home/rafael_aranzate/Desktop/Comp_I/Jogo/Sprites/PNG/Lasers/laserRed01.png");
    tiro->dx = 0;
    tiro->dy = -VELOCIDADE_TIRO;
    tiro->w = SCREEN_H / 36;
    tiro->h = SCREEN_H / 18;
    tiro->vida = 1;
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

void mensagem(char* texto, char* texto2 ,SDL_Color cor, double x, double y, double w, double h) {

    //strcat(texto, texto2);
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

static int colisaoInimigo(ENTIDADE* entidade1, ENTIDADE* entidade2) {
        //COLISAO ESQUERDA
        if (entidade1->x + entidade1->w >= entidade2->x && entidade1->x + entidade1->w <= entidade2->x + entidade2->w && entidade1->y <= entidade2->y + entidade2->h && entidade1->y >= entidade2->y - entidade2->h) {
            return 1;
        }
        //COLISAO DIREITA
        if (entidade1->x <= entidade2->x + entidade2->w && entidade1->x >= entidade2->x - entidade2->w && entidade1->y <= entidade2->y + entidade2->h && entidade1->y >= entidade2->y - entidade2->h) {
            return 2;
        }
        //COLISAO CIMA
        if (entidade1->y >= entidade2->y && entidade1->y <= entidade2->y + entidade2->h && entidade1->x >= entidade2->x -entidade2->w && entidade1->x <= entidade2->x + entidade2->w) {
            return 3;
        }
        //COLISAO BAIXO
        if (entidade1->y + entidade1->h >= entidade2->y && entidade1->y <= entidade2->y && entidade1->x >= entidade2->x -entidade2->w && entidade1->x <= entidade2->x + entidade2->w) {
            return 4;
        }
}
