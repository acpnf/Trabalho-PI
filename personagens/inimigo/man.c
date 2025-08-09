#include <raylib.h>
#include <stdio.h>
#include "inimigos.h"

#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 600

int main() {
    // Inicializa a janela
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Testando Inimigo");
    int visitados[10][16] = {0};  // Inicializa todos como 0 (não visitados)
    // Carrega o sprite do inimigo
    Texture2D inimigoSprite = LoadTexture("personagens/inimigo/zombie.png"); // Certifique-se de que o caminho do sprite está correto
    if (inimigoSprite.width == 0 || inimigoSprite.height == 0) {
        // Se a textura não carregar corretamente, exibe uma mensagem de erro
        printf("Erro ao carregar o sprite do inimigo!\n");
        return -1; // Sai do programa se o sprite não carregar
    }

    // Define o sourceRec para a textura inteira
    Rectangle sourceRec = { 0.0f, 0.0f, (float)inimigoSprite.width, (float)inimigoSprite.height };

    // Cria o inimigo
    Inimigo inimigo = CriarInimigo(100, 1, inimigoSprite);

    // Mapa de exemplo (1 é caminho livre, 0 é bloqueio)
    int mapa[10][16] = {
        {1, 0, 1, 1, 1, 0, 0, 0, 1, 1, 1, 0, 1, 0, 1, 1},
        {1, 0, 1, 0, 1, 0, 0, 0, 1, 1, 0, 0, 1, 1, 0, 1},
        {1, 0, 1, 0, 1, 0, 0, 1, 0, 1, 1, 0, 1, 0, 1, 1},
        {1, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {1, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
        {1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 1, 0, 1, 1, 0, 1, 0, 1, 1, 0},
        {1, 0, 1, 0, 1, 0, 1, 0, 0, 1, 1, 1, 0, 1, 1, 0},
        {1, 1, 1, 1, 1, 0, 0, 0, 1, 0, 1, 1, 1, 0, 1, 0},
        {1, 1, 1, 1, 0, 1, 0, 1, 0, 1, 0, 1, 1, 1, 1, 1}
    };

    // Variável para controlar o tempo de movimentação
    double lastMoveTime = 0.0;
    double moveInterval = 0.9; // Intervalo de movimento em segundos

    // Loop principal
    while (!WindowShouldClose()) {
        // Obtém o tempo atual
        double currentTime = GetTime();

        // Se o intervalo de tempo desejado passou, movimenta o inimigo
        if (currentTime - lastMoveTime >= moveInterval) {
            MovimentarInimigo(&inimigo, mapa, visitados);
            lastMoveTime = currentTime;  // Atualiza o tempo do último movimento
        }

        // Inicia o desenho
        BeginDrawing();
        ClearBackground(RAYWHITE);

        // Desenha o mapa (apenas para visualização)
        for (int i = 0; i < 10; i++) {
            for (int j = 0; j < 16; j++) {
                Color cellColor = (mapa[i][j] == 1) ? LIGHTGRAY : DARKGRAY;
                DrawRectangle(j * 64, i * 64, 64, 64, cellColor);
            }
        }
        
        // Desenha o inimigo (escala para 64x64)
        Vector2 position = { inimigo.posY * 64.0f, inimigo.posX * 64.0f };
        Vector2 scale = { 64.0f / inimigoSprite.width, 64.0f / inimigoSprite.height };
        
        // Ajusta o desenho da textura para a célula do mapa
        DrawTexturePro(inimigoSprite, sourceRec, (Rectangle){position.x, position.y, 64.0f, 64.0f}, (Vector2){0.0f, 0.0f}, 0.0f, WHITE);

        // Finaliza o desenho
        EndDrawing();
    }

    // Libera recursos
    UnloadTexture(inimigoSprite);
    CloseWindow(); // Fecha a janela

    return 0;
}
