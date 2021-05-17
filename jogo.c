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
#define VELOCIDADE_JOGADOR SCREEN_W / 120
#define VELOCIDADE_TIRO SCREEN_W / 80
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
ENTIDADE* inimigo2;
ENTIDADE* tiro;
ENTIDADE* cafe;
ENTIDADE* explosao;
Mix_Chunk* tiroSFX;
Mix_Music* musica;
FILE* file;

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
void defTiro();
void defCafe();
void defExplosao();
void defInimigo1();
void defInimigo2();
static int colisaoInimigo(ENTIDADE* entidade1, ENTIDADE* entidade2);
void mensagem(char* texto, char* texto2, SDL_Color cor, double x, double y, double w, double h);

bool jogando = false;
bool stateMenu = true;
bool aberto = true;
bool nomeMenu = false;
bool instrucoes = false;

int main(int argc, char** argv) {

    char titulo[50] = "Coffee Invaders";
    int stage = 1, spawn = 0, i;
    unsigned int lastTime = 0, currentTime = 0;
    unsigned long long int pontuacao = 0;
    char pontuacaoString[1000], hp[10], fase[10], nome1[30], nome2[30], nome3[30], pontuacao1[100], pontuacao2[100], pontuacao3[100], outputName[30];

    iniciar(titulo);
    BebasNeue = TTF_OpenFont("../Coffee_Invaders/BebasNeue-Regular.ttf", 24);
    SDL_SetRenderDrawColor(output.renderer, 111, 133, 255, 255);
    Mix_Chunk* tiroSFX = Mix_LoadWAV("../Coffee_Invaders/Assets/sfx_laser1.ogg");
    Mix_Music* musica = Mix_LoadMUS("../Coffee_Invaders/Assets/bensound-endlessmotion.mp3");
    Mix_PlayMusic(musica, -1);
    

    SDL_RenderPresent(output.renderer);
    while(aberto) {

        for (i= 0; i < sizeof(outputName); i++) {
            outputName[i] = NULL;
        }
        int tamanhoPontuacao = 0;

        defBackground();
        defJogador();
        defInimigo1();
        defInimigo2();
        defTiro();
        defMenu();
        defCafe();
        defExplosao();
        file = fopen("../Coffee_Invaders/highscore.txt", "r");

        fscanf(file, "%s", nome1);
        fscanf(file, "%s", pontuacao1);
        fscanf(file, "%s", nome2);
        fscanf(file, "%s", pontuacao2);
        fscanf(file, "%s", nome3);
        fscanf(file, "%s", pontuacao3);

        i = 0;
        while(stateMenu) {
            SDL_RenderClear(output.renderer);
            input();
            imprimirTextura(menu, menu->x, menu->y, menu->w, menu->h);
            mensagem("COFFEE INVADERS", NULL, branco, SCREEN_W / 2.9, SCREEN_H / 2.8, SCREEN_H / 2, SCREEN_H / 9);
            mensagem("PRESSIONE \"ENTER\" PARA INICIAR", NULL, branco, SCREEN_W / 2.9, SCREEN_H / 1.2, SCREEN_H / 1.9, SCREEN_W / 20);
            mensagem("SEGURE \"I\" - INSTRUCOES, \"H\" - HISTORIA, C - CREDITOS OU \"P\" - PONTUACOES", NULL, branco, SCREEN_W / 4, SCREEN_H / 1.1, SCREEN_W / 2, SCREEN_W / 20);
            if(output.teclado[SDL_SCANCODE_RETURN]) {
                stateMenu = false;
                jogando = true;
            }
            if(output.teclado[SDL_SCANCODE_I]) {
                SDL_RenderClear(output.renderer);
                imprimirTextura(menu, menu->x, menu->y, menu->w, menu->h);
                mensagem("UTILIZE AS SETAS DIRECIONAIS PARA CONTROLAR A NAVE", NULL, branco, SCREEN_W / 3.2, SCREEN_H / 2.8, SCREEN_H / 1.5, SCREEN_H / 9);
                mensagem("APERTE Z PARA ATIRAR", NULL, branco, SCREEN_W / 3.2, SCREEN_H / 2.2, SCREEN_H / 1.5, SCREEN_H / 9);
                input();
                SDL_RenderPresent(output.renderer);
            }
            if(output.teclado[SDL_SCANCODE_H]) {
                SDL_RenderClear(output.renderer);
                imprimirTextura(menu, menu->x, menu->y, menu->w, menu->h);
                mensagem("ALIENIGENAS MALIGNOS INVADIRAM A TERRA E ROUBARAM TODO", NULL, branco, SCREEN_W / 3.5, SCREEN_H / 2.8, SCREEN_H / 1.2, SCREEN_H / 9);
                mensagem("O ESTOQUE DE UMA BEBIDA ADORADA POR MUITOS, O CAFE.", NULL, branco, SCREEN_W / 3.5, SCREEN_H / 2.3, SCREEN_H / 1.2, SCREEN_H / 9);
                mensagem("CABE A VOCE RECUPERAR O CAFE E SALVAR O PLANETA!", NULL, branco, SCREEN_W / 3.5, SCREEN_H / 1.94, SCREEN_H / 1.2, SCREEN_H / 9);
                input();
                SDL_RenderPresent(output.renderer);
            }
            if(output.teclado[SDL_SCANCODE_C]) {
                SDL_RenderClear(output.renderer);
                imprimirTextura(menu, menu->x, menu->y, menu->w, menu->h);
                mensagem("DESENVOLVIDO POR: RAFAEL ROCHA ARANZATE", NULL, branco, SCREEN_W / 5, SCREEN_H / 2.8, SCREEN_H,  SCREEN_H / 5);
                input();
                SDL_RenderPresent(output.renderer);
            }
            if(output.teclado[SDL_SCANCODE_P] && file != NULL) {
                SDL_RenderClear(output.renderer);
                imprimirTextura(menu, menu->x, menu->y, menu->w, menu->h);
                mensagem(nome1, NULL, branco, SCREEN_W / 8, SCREEN_H / 2.8, SCREEN_H / 2, SCREEN_H / 10);
                mensagem(pontuacao1, NULL, branco, SCREEN_W / 2, SCREEN_H / 2.8, SCREEN_H / 3, SCREEN_H / 10);
                mensagem(nome2, NULL, branco, SCREEN_W / 8, SCREEN_H / 2.3, SCREEN_H / 2, SCREEN_H / 10);
                mensagem(pontuacao2, NULL, branco, SCREEN_W / 2, SCREEN_H / 2.3, SCREEN_H / 3, SCREEN_H / 10);
                mensagem(nome3, NULL, branco, SCREEN_W / 8, SCREEN_H / 1.94, SCREEN_H / 2, SCREEN_H / 10);
                mensagem(pontuacao3, NULL, branco, SCREEN_W / 2, SCREEN_H / 1.94, SCREEN_H / 3, SCREEN_H / 10);
                
                input();
                SDL_RenderPresent(output.renderer);
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

            if(stage % 2 == 0) {
                imprimirTextura(background, background->x, background->y, background->w, background->h);
            }
            input();
            //SECAO DE MOVIMENTO DO JOGADOR
            
            if (output.teclado[SDL_SCANCODE_UP]) {
                jogador->y -= VELOCIDADE_JOGADOR;
            }
            //Limite de borda eixo y
            if (jogador->y <= SCREEN_H / 14) {
                jogador->y += VELOCIDADE_JOGADOR;
            }
            if (output.teclado[SDL_SCANCODE_DOWN]) {
                jogador->y += VELOCIDADE_JOGADOR;
            }
            //Limite de borda eixo -y
            if ((jogador->y + jogador->h) >= SCREEN_H) {
                jogador->y -= VELOCIDADE_JOGADOR;
            }

            if (output.teclado[SDL_SCANCODE_LEFT]) {
                jogador->x -= VELOCIDADE_JOGADOR;
            }
            //Limite de borda eixo -x
            if (jogador->x >= NULL && jogador->x <= SCREEN_H / 10) {
                jogador->x += VELOCIDADE_JOGADOR;
            }
            if (output.teclado[SDL_SCANCODE_RIGHT]) {
                jogador->x += VELOCIDADE_JOGADOR;
            }
            //Limite de borda eixo x
            if ((jogador->x + jogador->w) >= SCREEN_W) {
                jogador->x -= VELOCIDADE_JOGADOR;
            }

            //TIRO DO JOGADOR
            if (output.teclado[SDL_SCANCODE_Z]) {
                tiro->vida = 1;
                Mix_PlayChannel(-1, tiroSFX, 0);
            }

            if (tiro->y < 0) {
                tiro->vida = 0;
            }
            if(inimigo1->x >= SCREEN_W) {
                inimigo1->x = 0;
            }
            if(inimigo1->y >= SCREEN_H) {
                inimigo1->y = 10;
            }
            if(inimigo2->x <= 0) {
                inimigo2->x = SCREEN_W;
            }
            if(inimigo2->y <= 10) {
                inimigo2->y = SCREEN_H;
            }

            if (colisaoInimigo(jogador, inimigo1) == 1) {
                jogador->x -= VELOCIDADE_JOGADOR;
                tiro->x = NULL;
                tiro->y = NULL;
                if(currentTime > lastTime + 1000) {
                    jogador->vida--;
                    lastTime = currentTime;
                }
            }
        
            if (colisaoInimigo(jogador, inimigo1) == 2) {
                jogador->x += VELOCIDADE_JOGADOR;
                tiro->x = NULL;
                tiro->y = NULL;
                if(currentTime > lastTime + 1000) {
                    jogador->vida--;
                    lastTime = currentTime;
                }
            }

            if (colisaoInimigo(jogador, inimigo2) == 1) {
                jogador->x -= VELOCIDADE_JOGADOR;
                tiro->x = NULL;
                tiro->y = NULL;
                if(currentTime > lastTime + 1000) {
                    jogador->vida--;
                    lastTime = currentTime;
                }
            }
        
            if (colisaoInimigo(jogador, inimigo2) == 2) {
                jogador->x += VELOCIDADE_JOGADOR;
                tiro->x = NULL;
                tiro->y = NULL;
                if(currentTime > lastTime + 1000) {
                    jogador->vida--;
                    lastTime = currentTime;
                }
            }

            if(colisaoInimigo(tiro, inimigo1) == 1) {
                tiro->vida = 0;
                inimigo1->vida--;
            }

            if(colisaoInimigo(tiro, inimigo2) == 1) {
                tiro->vida = 0;
                inimigo1->vida--;
            }
            
            if (tiro->vida == 1) {
                tiro->x += tiro->dx;
                tiro->y += tiro->dy;
    
                imprimirTextura(tiro, tiro->x, tiro->y, tiro->w, tiro->h);
            }

            if(tiro->vida == 0) {
                tiro->x = jogador->x + jogador->w / 2.4;
                tiro->y = jogador->y - jogador->h / 3; 
            }

            if (inimigo1->vida > 0) {
                imprimirTextura(inimigo1, inimigo1->x, inimigo1->y, inimigo1->w, inimigo1->h);
                inimigo1->x += inimigo1->dx;
                inimigo1->y += inimigo1->dy;
            }

            if (inimigo2->vida > 0) {
                imprimirTextura(inimigo2, inimigo2->x, inimigo2->y, inimigo2->w, inimigo2->h);
                inimigo2->x += inimigo2->dx;
                inimigo2->y += inimigo2->dy;
            }

            if(inimigo1->vida == 0 & spawn < 5) {
                pontuacao+= 100;
                if(currentTime > lastTime + 1000) {
                    imprimirTextura(explosao, inimigo1->x, inimigo1->y, explosao->h, explosao->w);
                    lastTime = currentTime;
                }
                inimigo1->vida = 10;
                spawn++;
                inimigo1->x = rand() % SCREEN_W;
                inimigo1->y = rand() % SCREEN_H / 3;
            }

            if(inimigo2->vida == 0 && spawn < 5) {
                pontuacao+= 100;
                if(currentTime > lastTime + 1000) {
                    imprimirTextura(explosao, inimigo1->x, inimigo1->y, explosao->h, explosao->w);
                    lastTime = currentTime;
                }
                inimigo2->vida = 10;
                spawn++;
                inimigo2->x = rand() % SCREEN_W;
                inimigo2->y = rand() % SCREEN_H / 3;
            }

            if(jogador->vida == 0) {
                jogando = false;
                jogador->vida = 10;
                free(jogador);
                free(tiro);
                free(inimigo1);
                nomeMenu = true;
            }
            if(spawn == 5){
                inimigo1->vida = 0;
                inimigo2->vida = 0;
                inimigo1->x = NULL;
                inimigo1->y = NULL;
                inimigo2->x = NULL;
                inimigo2->y = NULL;
                cafe->x = SCREEN_W / 2 ;
                cafe->y = SCREEN_H / 1.3;
                imprimirTextura(cafe, cafe->x, cafe->y, cafe->w, cafe->h);
            }

            if(colisaoInimigo(jogador, cafe) == 1) {
                pontuacao += 1000;
                cafe->x = NULL;
                cafe->y = NULL;
                spawn = 0;
                stage += 1;
            }
            if(colisaoInimigo(jogador, cafe) == 2) {
                pontuacao += 1000;
                cafe->x = NULL;
                cafe->y = NULL;
                spawn = 0;
                stage += 1;
            }

            imprimirTextura(jogador, jogador->x, jogador->y, jogador->w, jogador->h);
            mensagem(hp, NULL , branco, 10, 10, SCREEN_W / 9, SCREEN_H / 13);
            mensagem(fase, NULL, branco, SCREEN_W / 1.3, 10, SCREEN_W / 9, SCREEN_H / 13);
            mensagem(pontuacaoString, NULL, branco, SCREEN_W / 2.3, 10, SCREEN_W / 9, SCREEN_H / 13);
            SDL_RenderPresent(output.renderer);

            SDL_Delay(16);
        }

        while(nomeMenu) {
            SDL_RenderClear(output.renderer);
            input();
            imprimirTextura(menu, menu->x, menu->y, menu->w, menu->h);
            mensagem("DIGITE SEU NOME: ", NULL, branco, SCREEN_W / 2.9, SCREEN_H / 2.8, SCREEN_H / 2, SCREEN_H / 9);
            if(output.teclado[SDL_SCANCODE_A]) {
                outputName[i] = 'A';
                i++;
                SDL_Delay(300);
            }
            else if(output.teclado[SDL_SCANCODE_B]) {
                outputName[i] = 'B';
                i++;
                SDL_Delay(300);
            }
            else if(output.teclado[SDL_SCANCODE_C]) {
                outputName[i] = 'C';
                i++;
                SDL_Delay(300);
            }
            else if(output.teclado[SDL_SCANCODE_D]) {
                outputName[i] = 'D';
                i++;
                SDL_Delay(300);
            }
            else if(output.teclado[SDL_SCANCODE_E]) {
                outputName[i] = 'E';
                i++;
                SDL_Delay(300);
            }
            else if(output.teclado[SDL_SCANCODE_F]) {
                outputName[i] = 'F';
                i++;
                SDL_Delay(300);
            }
            else if(output.teclado[SDL_SCANCODE_G]) {
                outputName[i] = 'G';
                i++;
                SDL_Delay(300);
            }
            else if(output.teclado[SDL_SCANCODE_H]) {
                outputName[i] = 'H';
                i++;
                SDL_Delay(300);
            }
            else if(output.teclado[SDL_SCANCODE_I]) {
                outputName[i] = 'I';
                i++;
                SDL_Delay(300);
            }
            else if(output.teclado[SDL_SCANCODE_J]) {
                outputName[i] = 'J';
                i++;
                SDL_Delay(300);
            }
            else if(output.teclado[SDL_SCANCODE_K]) {
                outputName[i] = 'K';
                i++;
                SDL_Delay(300);
            }
            else if(output.teclado[SDL_SCANCODE_L]) {
                outputName[i] = 'L';
                i++;
                SDL_Delay(300);
            }
            else if(output.teclado[SDL_SCANCODE_M]) {
                outputName[i] = 'M';
                i++;
                SDL_Delay(300);
            }
            else if(output.teclado[SDL_SCANCODE_N]) {
                outputName[i] = 'N';
                i++;
                SDL_Delay(300);
            }
            else if(output.teclado[SDL_SCANCODE_O]) {
                outputName[i] = 'O';
                i++;
                SDL_Delay(300);
            }
            else if(output.teclado[SDL_SCANCODE_P]) {
                outputName[i] = 'P';
                i++;
                SDL_Delay(300);
            }
            else if(output.teclado[SDL_SCANCODE_Q]) {
                outputName[i] = 'Q';
                i++;
                SDL_Delay(300);
            }
            else if(output.teclado[SDL_SCANCODE_R]) {
                outputName[i] = 'R';
                i++;
                SDL_Delay(300);
            }
            else if(output.teclado[SDL_SCANCODE_S]) {
                outputName[i] = 'S';
                i++;
                SDL_Delay(300);
            }
            else if(output.teclado[SDL_SCANCODE_T]) {
                outputName[i] = 'T';
                i++;
                SDL_Delay(300);
            }
            else if(output.teclado[SDL_SCANCODE_U]) {
                outputName[i] = 'U';
                i++;
                SDL_Delay(300);
            }
            else if(output.teclado[SDL_SCANCODE_V]) {
                outputName[i] = 'V';
                i++;
                SDL_Delay(300);
            }
            else if(output.teclado[SDL_SCANCODE_W]) {
                outputName[i] = 'W';
                i++;
                SDL_Delay(300);
            }
            else if(output.teclado[SDL_SCANCODE_X]) {
                outputName[i] = 'X';
                i++;
                SDL_Delay(300);
            }
            else if(output.teclado[SDL_SCANCODE_Y]) {
                outputName[i] = 'Y';
                i++;
                SDL_Delay(300);
            }
            else if(output.teclado[SDL_SCANCODE_Z]) {
                outputName[i] = 'Z';
                i++;
                SDL_Delay(300);
            }
            else if(output.teclado[SDL_SCANCODE_BACKSPACE]) {
                outputName[i-1] = NULL;
                if(i >= 0) {
                    i--;
                }
                SDL_Delay(150);
            }
            else if(output.teclado[SDL_SCANCODE_RETURN]) {
                for(i=0; pontuacaoString[i] != '\0'; i++) {
                    tamanhoPontuacao++;
                }
                nomeMenu = false;
                sprintf(pontuacaoString, "%d", pontuacao);
                //SDL_RWwrite(file, outputName, sizeof(outputName), 1);
                FILE *file = fopen("../Coffee_Invaders/highscore.txt", "a+");
                if(file!=NULL){
                fputs(outputName, file);
                fputs("\n", file);
                fputs(pontuacaoString, file);
                fputs("\n", file);
                }
                fclose(file);
                stateMenu = true;
                SDL_Delay(300);
            }
            mensagem(outputName, NULL, branco, SCREEN_W / 3, SCREEN_H / 2, SCREEN_H / 2, SCREEN_H / 9);
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
    Mix_OpenAudio(MIX_DEFAULT_FREQUENCY,MIX_DEFAULT_FORMAT,2,4096);

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
            nomeMenu = false;
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
            aberto = false;
            menu = false;
			jogando = false;
            nomeMenu = false;
		}
	}
}

void KeyDown(SDL_KeyboardEvent *evento){
	if (evento->repeat == 0 && evento->keysym.scancode < TECLAS) {
		
        output.teclado[evento->keysym.scancode] = 1;

        if (evento->keysym.scancode == SDL_SCANCODE_ESCAPE) {
            aberto = false;
            stateMenu = false;
			jogando = false;
            nomeMenu = false;
		}
	}
}

void defMenu() {
    menu = malloc(sizeof(ENTIDADE));

    menu->textura = IMG_Load("../Coffee_Invaders/Assets/menu.jpg");
    menu->x = 0;
    menu->y = 0;
    menu->w = SCREEN_W;
    menu->h = SCREEN_H;
}

void defBackground() {
    background = malloc(sizeof(ENTIDADE));

    background->textura = IMG_Load("../Coffee_Invaders/Assets/purple.png");
    background->x = 0;
    background->y = 0;
    background->w = SCREEN_W;
    background->h = SCREEN_H;
}

void defJogador() {
    jogador = malloc(sizeof(ENTIDADE));

    jogador->textura = IMG_Load("../Coffee_Invaders/Assets/player.png");
    jogador->x = SCREEN_W / 2 - 20;
    jogador->y = SCREEN_H - 100;
    jogador->w = SCREEN_W / 9;
    jogador->h = SCREEN_H / 10;
    jogador->vida = 10;

}

void defInimigo1() {
    inimigo1 = malloc(sizeof(ENTIDADE));

    inimigo1->textura = IMG_Load("../Coffee_Invaders/Assets/enemyBlack1.png");
    inimigo1->x = SCREEN_W / 2;
    inimigo1->y = SCREEN_H / 2;
    inimigo1->w = SCREEN_W / 10;
    inimigo1->h = SCREEN_H / 10;
    inimigo1->dx = 9;
    inimigo1->dy = 1;
    inimigo1->vida = 5;

}

void defInimigo2() {
    inimigo2 = malloc(sizeof(ENTIDADE));

    inimigo2->textura = IMG_Load("../Coffee_Invaders/Assets/enemyRed2.png");
    inimigo2->x = SCREEN_W / 4;
    inimigo2->y = SCREEN_H / 4;
    inimigo2->w = SCREEN_W / 10;
    inimigo2->h = SCREEN_H / 10;
    inimigo2->dx = -3;
    inimigo2->dy = -6;
    inimigo2->vida = 5;

}

void defTiro() {
    tiro =  malloc(sizeof(ENTIDADE));

    tiro->textura = IMG_Load("../Coffee_Invaders/Assets/laserRed01.png");
    tiro->dx = 0;
    tiro->dy = -VELOCIDADE_TIRO;
    tiro->w = SCREEN_H / 36;
    tiro->h = SCREEN_H / 18;
    tiro->vida = 0;
}

void defCafe() {
    cafe = malloc(sizeof(ENTIDADE));

    cafe->textura = IMG_Load("../Coffee_Invaders/Assets/cafe.png");
    cafe->x = NULL;
    cafe->y = NULL;
    cafe->w = SCREEN_H / 10;
    cafe->h = SCREEN_H / 10;
}
void defExplosao() {
    explosao = malloc(sizeof(ENTIDADE));

    explosao->textura = IMG_Load("../Coffee_Invaders/Assets/explosao.png");
    explosao->x = NULL;
    explosao->y = NULL;
    explosao->w = SCREEN_W / 4;
    explosao->h = SCREEN_H / 4;
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
