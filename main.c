#include <stdio.h>
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_mixer.h>
#include <stdbool.h>
#include <time.h>

#define SCREEN_W 1024
#define SCREEN_H 768
#define VELOCIDADE_JOGADOR 8
#define VELOCIDADE_TIRO 20


typedef struct {
    SDL_Renderer *renderer;
    SDL_Window *janela;
    int up;
	int down;
	int left;
	int right;
    int atirar;
} OUTPUT;

typedef struct {
	int x;
	int y;
    int dx;
	int dy;
	int health;
	SDL_Texture *textura;
} ENTIDADE;

enum KeyPressSurfaces {
    KEY_PRESS_SURFACE_DEFAULT,
    KEY_PRESS_SURFACE_UP,
    KEY_PRESS_SURFACE_DOWN,
    KEY_PRESS_SURFACE_LEFT,
    KEY_PRESS_SURFACE_RIGHT,
    KEY_PRESS_SURFACE_TOTAL
};

OUTPUT output;
ENTIDADE jogador;
ENTIDADE bala;
ENTIDADE inimigo1;
ENTIDADE inimigo2;
ENTIDADE inimigo3;
ENTIDADE inimigo4;
ENTIDADE background;
bool jogando = true;

void iniciar(char* titulo);
void input(void);
void renderer(void);
void imprimirRenderer(void);
SDL_Texture* carregarTextura(char* arquivo);
void desenharTextura(SDL_Texture* textura, int x, int y, int w, int h);
void KeyDown(SDL_KeyboardEvent *evento);
void KeyUp(SDL_KeyboardEvent *evento);


int main(int arc, char** argv) {

    char titulo[50] = "role de nave";
    int i, quantidadeInimigos, ondas;

    //memset(&output, 0, sizeof(OUTPUT));

    iniciar(titulo);

    jogador.x = SCREEN_W / 2;
    jogador.y = SCREEN_H / 2;
    inimigo1.x = 100;
    inimigo1.y = 120;
    quantidadeInimigos = 5;
    jogador.textura = carregarTextura("/home/rafael_aranzate/Desktop/Comp_I/Jogo/Sprites/PNG/player.png");
    bala.textura = carregarTextura("/home/rafael_aranzate/Desktop/Comp_I/Jogo/Sprites/PNG/Lasers/laserRed01.png");
    inimigo1.textura = carregarTextura("/home/rafael_aranzate/Desktop/Comp_I/Jogo/Sprites/PNG/Enemies/enemyBlack01.png");
    inimigo2.textura = carregarTextura("/home/rafael_aranzate/Desktop/Comp_I/Jogo/Sprites/PNG/Enemies/enemyBlue02.png");
    inimigo3.textura = carregarTextura("/home/rafael_aranzate/Desktop/Comp_I/Jogo/Sprites/PNG/Enemies/enemyGreen03.png");
    inimigo4.textura = carregarTextura("/home/rafael_aranzate/Desktop/Comp_I/Jogo/Sprites/PNG/Enemies/enemyRed04.png");
    background.textura = carregarTextura("/home/rafael_aranzate/Desktop/Comp_I/Jogo/Sprites/Backgrounds/purple.png");


    prepararRenderer();
    imprimirRenderer();

    while(jogando) {

        prepararRenderer();
        desenharTextura(background.textura, 100, 100, SCREEN_W, SCREEN_H);
        input();
        jogador.x += jogador.dx;
        jogador.y += jogador.dy;

        if (output.up) {
			jogador.y -= VELOCIDADE_JOGADOR;
		}
        //Limite de borda eixo y
        if (output.up && jogador.y <= 0) {
			jogador.y += VELOCIDADE_JOGADOR;
		}
		if (output.down) {
			jogador.y += VELOCIDADE_JOGADOR;
		}
        //Limite de borda eixo -y
        if (output.down && jogador.y >= SCREEN_H) {
			jogador.y -= VELOCIDADE_JOGADOR;
		}

		if (output.left) {
			jogador.x -= VELOCIDADE_JOGADOR;
		}
        //Limite de borda eixo -x
        if (output.left && jogador.x <= 0) {
			jogador.x += VELOCIDADE_JOGADOR;
		}
		if (output.right) {
			jogador.x += VELOCIDADE_JOGADOR;
		}
        //Limite de borda eixo x
        if (output.right && jogador.x >= SCREEN_H) {
			jogador.x -= VELOCIDADE_JOGADOR;
		}

        if (output.atirar && bala.health == 0) {
			bala.x = jogador.x + 50;
			bala.y = jogador.y;
			bala.dx = 0;
			bala.dy = -VELOCIDADE_TIRO;
			bala.health = 1;
		}

        bala.x += bala.dx;
		bala.y += bala.dy;

        if (bala.y < 0) {
			bala.health = 0;
		}

        desenharTextura(jogador.textura, jogador.x, jogador.y, NULL, NULL);
        desenharTextura(inimigo1.textura, 200, 200, NULL, NULL);

        if (bala.health == 1) {
			desenharTextura(bala.textura, bala.x, bala.y, NULL, NULL);
        }

        imprimirRenderer();
        SDL_Delay(16);
    }
    SDL_Quit();
    return 0;
}

void iniciar(char* titulo) {

    SDL_Init(SDL_INIT_EVERYTHING);

    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
		printf("Nao foi possivel inicializar o SDL: %s\n", SDL_GetError());
		exit(1);
	}

    output.janela = SDL_CreateWindow(titulo, 1024, 768, SCREEN_W, SCREEN_H, SDL_WINDOW_RESIZABLE);

    if (!output.janela) {
		printf("Erro ao criar janela %d x %d: %s\n", SCREEN_W, SCREEN_H, SDL_GetError());
		exit(1);
	}
    output.renderer = SDL_CreateRenderer(output.janela, -1, 0);

    if (!output.renderer) {
		printf("Erro ao criar renderer: %s\n", SDL_GetError());
		exit(1);
	}
}

void input(void) {
    SDL_Event evento;

    while (SDL_PollEvent(&evento)) {

        if(evento.type == SDL_QUIT) {
                jogando = false;
            }
            switch (evento.type) {
               case SDLK_ESCAPE:
                    jogando = false;
                    break;
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

void prepararRenderer(void) {
    SDL_SetRenderDrawColor(output.renderer, 111, 133, 255, 255);
    SDL_RenderClear(output.renderer);
}

void imprimirRenderer(void) {
    SDL_RenderPresent(output.renderer);
}

SDL_Texture* carregarTextura(char* arquivo) {
    SDL_Texture* textura;
    textura = IMG_LoadTexture(output.renderer, arquivo);

    return textura;
}

void desenharTextura(SDL_Texture* textura, int x, int y, int w, int h) {
    SDL_Rect dest;
    dest.x = x;
    dest.y = y;
    dest.w = w;
    dest.h = h;

    SDL_QueryTexture(textura, NULL, NULL, &dest.w, &dest.h);

    SDL_RenderCopy(output.renderer, textura, NULL, &dest);
}

void KeyDown(SDL_KeyboardEvent *evento){
	if (evento->repeat == 0) {
		if (evento->keysym.scancode == SDL_SCANCODE_UP) {
			output.up = 1;
		}

		if (evento->keysym.scancode == SDL_SCANCODE_DOWN) {
			output.down = 1;
		}

		if (evento->keysym.scancode == SDL_SCANCODE_LEFT) {
			output.left = 1;
		}

		if (evento->keysym.scancode == SDL_SCANCODE_RIGHT) {
			output.right = 1;
		}
        if (evento->keysym.scancode == SDL_SCANCODE_Z) {
	        output.atirar = 1;
        }
        if (evento->keysym.scancode == SDL_SCANCODE_ESCAPE) {
			SDL_Quit();
		}
	}
}
void KeyUp(SDL_KeyboardEvent *evento){
	if (evento->repeat == 0) {
		if (evento->keysym.scancode == SDL_SCANCODE_UP) {
			output.up = 0;
		}

		if (evento->keysym.scancode == SDL_SCANCODE_DOWN) {
			output.down = 0;
		}

		if (evento->keysym.scancode == SDL_SCANCODE_LEFT) {
			output.left = 0;
		}

		if (evento->keysym.scancode == SDL_SCANCODE_RIGHT) {
			output.right = 0;
		}
        if (evento->keysym.scancode == SDL_SCANCODE_Z) {
	        output.atirar = 0;
        }
        if (evento->keysym.scancode == SDL_SCANCODE_ESCAPE) {
			SDL_Quit();
		}
	}
}
//gcc -o main2 main2.c `sdl2-config --cflags --libs` -lSDL2 -lSDL2_image
