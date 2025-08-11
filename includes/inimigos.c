#include <raylib.h>
#include "inimigos.h"
#include <stdbool.h>

Inimigo CriarInimigo(int life, int speed, Texture2D sprite) {
    return (Inimigo){
        .vida       = life,
        .velocidade = speed,
        .sprite     = sprite,
        .posX       = 0,
        .posY       = 0,
        .morto      = false,
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

    // Aumenta o contador da célula atual
    //visitados[dx][dy]++;

    // Verifica direções possíveis
    bool pode[4] = {
        PodeSeMover(mapa, visitados, dx, dy + 1), // Direita
        PodeSeMover(mapa, visitados, dx - 1, dy), // Cima
        PodeSeMover(mapa, visitados, dx + 1, dy), // Baixo
        PodeSeMover(mapa, visitados, dx, dy - 1)  // Esquerda
    };

    // Movimento prioritário para fase 2
    if (pode[0]) {
        inimigo->posY += 1; // Direita
    } 
    else if (pode[1]) {
        inimigo->posX -= 1; // Cima
    }
    else if (pode[2]) {
        inimigo->posX += 1; // Baixo
    }
    else if (pode[3]) {
        inimigo->posY -= 1; // Esquerda
    }
}


void DesenharInimigoComSprite(Inimigo inimigo, Texture2D textura, Rectangle sourceRec) {
    // Calculando a posição do inimigo no mapa, multiplicando por 64 (tamanho das células)
    Vector2 position = { inimigo.posY * 64.0f, inimigo.posX * 64.0f };
    // Desenhando o sprite do inimigo
    DrawTextureRec(textura, sourceRec, position, WHITE);
}