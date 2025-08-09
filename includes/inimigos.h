#include <raylib.h>
#ifndef inimigos_H
#define inimigos_H
#define MAPA_LINHAS 10
#define MAPA_COLUNAS 20
typedef struct {
    int vida;
    int velocidade;
    Texture2D sprite; // imagem
    int posX; // posicao
    int posY;
} Inimigo;

Inimigo CriarInimigo(int life, int speed, Texture2D sprite);
bool PodeSeMover(int mapa[MAPA_LINHAS][MAPA_COLUNAS], int visitados[MAPA_LINHAS][MAPA_COLUNAS], int dx, int dy);
void MovimentarInimigo(Inimigo *inimigo, int mapa[MAPA_LINHAS][MAPA_COLUNAS], int visitados[MAPA_LINHAS][MAPA_COLUNAS]);

void DesenharInimigoComSprite(Inimigo inimigo, Texture2D textura, Rectangle sourceRec); 
#endif
