#include <raylib.h>
#include "inimigos.h"
#include <stdbool.h>
#define MAPA_LINHAS 10
#define MAPA_COLUNAS 16

Inimigo CriarInimigo(int life, int speed, Texture2D sprite) {
    return (Inimigo){
        .vida       = life,
        .velocidade = speed,
        .sprite     = sprite,
        .posX       = 0,
        .posY       = 0,
    };
}

// Função para verificar se a célula já foi visitada
bool PodeSeMover(int mapa[MAPA_LINHAS][MAPA_COLUNAS], int visitados[MAPA_LINHAS][MAPA_COLUNAS], int dx, int dy) {
    // Verifica se a posição está dentro dos limites e não foi visitada
    if (dx < 0 || dx >= MAPA_LINHAS || dy < 0 || dy >= MAPA_COLUNAS) {
        return false;
    }
    // A célula deve ser livre (1) e não visitada
    return mapa[dx][dy] == 1 && !visitados[dx][dy];
}

// Função para mover o inimigo, agora com controle de visitação
void MovimentarInimigo(Inimigo *inimigo, int mapa[MAPA_LINHAS][MAPA_COLUNAS], int visitados[MAPA_LINHAS][MAPA_COLUNAS]) {

    int dx = inimigo->posX;
    int dy = inimigo->posY;

    // Marca a célula atual como visitada
    visitados[dx][dy] = 1;

    // Tentativa de mover para baixo (direção preferencial)
    if (PodeSeMover(mapa, visitados, dx + 1, dy)) {
        inimigo->posX += 1; // Move para baixo
    }
    // Caso não consiga para baixo, tenta para a direita
    else if (PodeSeMover(mapa, visitados, dx, dy + 1)) {
        inimigo->posY += 1; // Move para a direita
    }
    // Caso não consiga para baixo nem para a direita, tenta para a esquerda
    else if (PodeSeMover(mapa, visitados, dx, dy - 1)) {
        inimigo->posY -= 1; // Move para a esquerda
    }
    // Caso não consiga para baixo, direita nem esquerda, tenta para cima
    else if (PodeSeMover(mapa, visitados, dx - 1, dy)) {
        inimigo->posX -= 1; // Move para cima
    }
}


void DesenharInimigoComSprite(Inimigo inimigo, Texture2D textura, Rectangle sourceRec) {
    // Calculando a posição do inimigo no mapa, multiplicando por 64 (tamanho das células)
    Vector2 position = { inimigo.posY * 64.0f, inimigo.posX * 64.0f };
    // Desenhando o sprite do inimigo
    DrawTextureRec(textura, sourceRec, position, WHITE);
}
