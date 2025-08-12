#include <raylib.h>
#include <stdbool.h> 
#ifndef inimigos_H
#define inimigos_H

#define LINHAS_MAPA 10
#define COLUNAS_MAPA 20

typedef struct {
    int vida;
    int velocidade;
    Texture2D sprite; // imagem
    int posX; // posicao
    int posY;
    bool morto; 
} Inimigo;

Inimigo CriarInimigo(int life, int speed, Texture2D sprite);
bool PodeSeMover(int mapa[LINHAS_MAPA][COLUNAS_MAPA], int visitados[LINHAS_MAPA][COLUNAS_MAPA], int dx, int dy);
void MovimentarInimigo(Inimigo *inimigo, int mapa[LINHAS_MAPA][COLUNAS_MAPA], int visitados[LINHAS_MAPA][COLUNAS_MAPA]);

void DesenharInimigoComSprite(Inimigo inimigo, Texture2D textura, Rectangle sourceRec); 
#endif