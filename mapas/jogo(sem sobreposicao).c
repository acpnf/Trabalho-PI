#include "raylib.h"
#include <stdio.h>
#include "torres.h"

#define TILE_SIZE 64
#define LINHAS_MAPA 10
#define COLUNAS_MAPA 20
#define MAX_TORRES 100

typedef enum {
    TORRE_SOLDADO = 0,
    TORRE_ARQUEIRO,
    TORRE_MAGO,
    TORRE_TOTAL
} TipoTorre;


bool existe_torre_no_tile(int x, int y, Soldado* soldados, int num_soldados, Arqueiro* arqueiros, int num_arqueiros, Mago* magos, int num_magos) {
    int centroX = x * TILE_SIZE + TILE_SIZE / 2;
    int centroY = y * TILE_SIZE + TILE_SIZE / 2;

    for (int i = 0; i < num_soldados; i++) {
        if (soldados[i].posicao.x == centroX && soldados[i].posicao.y == centroY) {
            return true;
        }
    }

    for (int i = 0; i < num_arqueiros; i++) {
        if (arqueiros[i].posicao.x == centroX && arqueiros[i].posicao.y == centroY) {
            return true;
        }
    }

    for (int i = 0; i < num_magos; i++) {
        if (magos[i].posicao.x == centroX && magos[i].posicao.y == centroY) {
            return true;
        }
    }

    return false;
}

int main(void) {
    const int largura = TILE_SIZE * COLUNAS_MAPA + 150;
    const int altura = TILE_SIZE * LINHAS_MAPA;

    InitWindow(largura, altura, "Jogo de Torres");
    SetTargetFPS(60);

    Texture2D grama_textura = LoadTexture("grama.png");
    Texture2D caminho_textura = LoadTexture("caminho.png");
    Texture2D sprite_mago = LoadTexture("torre_mago.png");
    Texture2D sprite_arqueiro = LoadTexture("torre_arqueiro.png");
    Texture2D sprite_soldado = LoadTexture("torre_geral.png");

    iniciar_torres();

    int mapa[LINHAS_MAPA][COLUNAS_MAPA] = {
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 0, 0, 0, 1, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 1, 1, 1, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 0, 0},
        {0, 1, 1, 1, 0, 0, 0, 1, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 0, 0},
        {0, 1, 0, 1, 0, 0, 0, 1, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 0, 0},
        {0, 1, 0, 1, 0, 0, 0, 1, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 0, 0},
        {1, 1, 0, 1, 1, 1, 1, 1, 0, 0, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0}
    };

    Soldado soldados[MAX_TORRES];
    Arqueiro arqueiros[MAX_TORRES];
    Mago magos[MAX_TORRES];
    int num_soldados = 0;
    int num_arqueiros = 0;
    int num_magos = 0;

    int moedas = 100;
    int custos[TORRE_TOTAL] = {CUSTO_SOLDADO, CUSTO_ARQUEIRO, CUSTO_MAGO};
    const char* nomes_torres[TORRE_TOTAL] = {"Soldado", "Arqueiro", "Mago"};

    bool arrastando = false;
    TipoTorre torre_arrastada = TORRE_SOLDADO;

    int menuWidth = 150;
    int altura_botao = 50;
    int menuX = largura - menuWidth;

    while (!WindowShouldClose()) {
        Vector2 mousePos = GetMousePosition();
        int tileX = mousePos.x / TILE_SIZE;
        int tileY = mousePos.y / TILE_SIZE;

        if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
            if (mousePos.x > menuX) {
                int buttonIndex = (mousePos.y - 40) / altura_botao;
                if (buttonIndex >= 0 && buttonIndex < TORRE_TOTAL && moedas >= custos[buttonIndex]) {
                    arrastando = true;
                    torre_arrastada = buttonIndex;
                }
            }
        }

        // --- ALTERADO: verificação se já existe torre antes de adicionar ---
        if (IsMouseButtonReleased(MOUSE_LEFT_BUTTON) && arrastando) {
            arrastando = false;
            if (tileX >= 0 && tileX < COLUNAS_MAPA &&
                tileY >= 0 && tileY < LINHAS_MAPA &&
                mousePos.x < menuX &&
                mapa[tileY][tileX] == 0 &&
                !existe_torre_no_tile(tileX, tileY, soldados, num_soldados, arqueiros, num_arqueiros, magos, num_magos))
            {
                Vector2 pos = {
                    tileX * TILE_SIZE + TILE_SIZE / 2,
                    tileY * TILE_SIZE + TILE_SIZE / 2
                };

                switch (torre_arrastada) {
                    case TORRE_SOLDADO:
                        if (num_soldados < MAX_TORRES) {
                            soldados[num_soldados++] = criar_soldado(pos, 2 * TILE_SIZE);
                            moedas -= CUSTO_SOLDADO;
                        }
                        break;
                    case TORRE_ARQUEIRO:
                        if (num_arqueiros < MAX_TORRES) {
                            arqueiros[num_arqueiros++] = criar_arqueiro(pos, 4 * TILE_SIZE);
                            moedas -= CUSTO_ARQUEIRO;
                        }
                        break;
                    case TORRE_MAGO:
                        if (num_magos < MAX_TORRES) {
                            magos[num_magos++] = criar_mago(pos, 6 * TILE_SIZE);
                            moedas -= CUSTO_MAGO;
                        }
                        break;
                }
            }
        }

        BeginDrawing();
        ClearBackground(RAYWHITE);

        for (int y = 0; y < LINHAS_MAPA; y++) {
            for (int x = 0; x < COLUNAS_MAPA; x++) {
                Vector2 pos = {x * TILE_SIZE, y * TILE_SIZE};
                Texture2D textura = (mapa[y][x] == 1) ? caminho_textura : grama_textura;
                DrawTextureRec(textura, (Rectangle){0, 0, TILE_SIZE, TILE_SIZE}, pos, WHITE);
            }
        }

        if (tileX >= 0 && tileX < COLUNAS_MAPA && tileY >= 0 && tileY < LINHAS_MAPA) {
            bool ocupado = existe_torre_no_tile(tileX, tileY, soldados, num_soldados, arqueiros, num_arqueiros, magos, num_magos);
            Color cor_cursor = (mapa[tileY][tileX] == 0 && !ocupado) ? (Color){0, 0, 255, 100} : (Color){255, 0, 0, 100};
            DrawRectangle(tileX * TILE_SIZE, tileY * TILE_SIZE, TILE_SIZE, TILE_SIZE, cor_cursor);
        }

        for (int i = 0; i < num_soldados; i++) {
            desenhar_soldado(soldados[i], TILE_SIZE, TILE_SIZE);
        }
        for (int i = 0; i < num_arqueiros; i++) {
            desenhar_arqueiro(arqueiros[i], TILE_SIZE, TILE_SIZE);
        }
        for (int i = 0; i < num_magos; i++) {
            desenhar_mago(magos[i], TILE_SIZE, TILE_SIZE);
        }

        DrawRectangle(menuX, 0, menuWidth, altura, (Color){50, 50, 50, 200});
        DrawText("LOJA DE TORRES", menuX + 10, 10, 20, RAYWHITE);

        for (int i = 0; i < TORRE_TOTAL; i++) {
            int yPos = 40 + i * altura_botao;
            bool podeComprar = (moedas >= custos[i]);
            Color cor = podeComprar ? RAYWHITE : GRAY;

            DrawRectangle(menuX + 10, yPos, menuWidth - 20, altura_botao - 10, (Color){70, 70, 70, 200});
            DrawText(nomes_torres[i], menuX + 20, yPos + 5, 20, cor);
            DrawText(TextFormat("%d moedas", custos[i]), menuX + 20, yPos + 25, 15, cor);
        }

        if (arrastando) {
            Texture2D sprite = (torre_arrastada == TORRE_SOLDADO) ? sprite_soldado :
                              (torre_arrastada == TORRE_ARQUEIRO) ? sprite_arqueiro :
                                                                    sprite_mago;

            DrawTexturePro(
                sprite,
                (Rectangle){0, 0, sprite.width, sprite.height},
                (Rectangle){mousePos.x, mousePos.y, TILE_SIZE, TILE_SIZE},
                (Vector2){TILE_SIZE / 2, TILE_SIZE / 2},
                0,
                WHITE
            );
        }

        DrawText(TextFormat("Moedas: %d", moedas), 20, 20, 30, GOLD);
        DrawText("Arraste torres para o mapa", 20, altura - 30, 20, LIGHTGRAY);

        EndDrawing();
    }

    liberar_torres();
    UnloadTexture(grama_textura);
    UnloadTexture(caminho_textura);
    UnloadTexture(sprite_soldado);
    UnloadTexture(sprite_arqueiro);
    UnloadTexture(sprite_mago);

    CloseWindow();
    return 0;
}
