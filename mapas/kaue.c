#include "raylib.h"
#include <stdio.h>

#define TILE_SIZE 64
#define LINHAS_MAPA 10
#define COLUNAS_MAPA 20

#define CUSTO_ROSA 1
#define CUSTO_AMARELO 3
#define CUSTO_LARANJA 5

typedef enum {
    COR_ROSA = 0,
    COR_AMARELO,
    COR_LARANJA,
    COR_TOTAL
} Cor;

int main(void)
{
    const int largura = TILE_SIZE * COLUNAS_MAPA;
    const int altura = TILE_SIZE * LINHAS_MAPA;

    InitWindow(largura, altura, "jogo");
    SetTargetFPS(60);

    // Carregue suas texturas aqui (certifique-se que os arquivos estão na pasta correta)
    Texture2D grama_textura = LoadTexture("mapas/imagens/grama.png");
    Texture2D caminho_textura = LoadTexture("mapas/imagens/caminho.png");

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

    int moedas = 10;
    Color cores_menu[COR_TOTAL] = {
        {255, 105, 180, 255},  // Rosa
        {255, 255, 0, 255},    // Amarelo
        {255, 165, 0, 255}     // Laranja
    };
    int custos[COR_TOTAL] = {CUSTO_ROSA, CUSTO_AMARELO, CUSTO_LARANJA};

    bool arrastando = false;
    Cor cor_arrastada = COR_ROSA;

    while (!WindowShouldClose())
    {
        Vector2 mouse = GetMousePosition();
        int tileX = mouse.x / TILE_SIZE;
        int tileY = mouse.y / TILE_SIZE;

        int menu_x = largura - 160;
        int menu_y = 20;
        int menu_width = 160;
        int menu_height = 160;

        bool mouse_clicado = IsMouseButtonPressed(MOUSE_LEFT_BUTTON);
        bool mouse_soltou = IsMouseButtonReleased(MOUSE_LEFT_BUTTON);

        for (int i = 0; i < COR_TOTAL; i++)
        {
            int cor_pos_y = menu_y + 40 + i * 30;
            Rectangle botao = { menu_x + 20, cor_pos_y, 20, 20 };

            if (CheckCollisionPointRec(mouse, botao) && mouse_clicado)
            {
                if (moedas >= custos[i]) {
                    arrastando = true;
                    cor_arrastada = i;
                }
            }
        }

        if (mouse_soltou && arrastando)
        {
            if (tileX >= 0 && tileX < COLUNAS_MAPA &&
                tileY >= 0 && tileY < LINHAS_MAPA)
            {
                if (mapa[tileY][tileX] == 0 && moedas >= custos[cor_arrastada])
                {
                    mapa[tileY][tileX] = 2 + cor_arrastada;
                    moedas -= custos[cor_arrastada];
                }
            }
            arrastando = false;
        }

        BeginDrawing();
        ClearBackground(RAYWHITE);

        for (int y = 0; y < LINHAS_MAPA; y++)
        {
            for (int x = 0; x < COLUNAS_MAPA; x++)
            {
                if (mapa[y][x] == 1)
                {
                    DrawTextureRec(caminho_textura, (Rectangle){0, 0, TILE_SIZE, TILE_SIZE},
                                   (Vector2){x * TILE_SIZE, y * TILE_SIZE}, WHITE);
                }
                else
                {
                    DrawTextureRec(grama_textura, (Rectangle){0, 0, TILE_SIZE, TILE_SIZE},
                                   (Vector2){x * TILE_SIZE, y * TILE_SIZE}, WHITE);
                }

                if (mapa[y][x] >= 2)
                {
                    int idx_cor = mapa[y][x] - 2;

                    Vector2 p1 = {x * TILE_SIZE + TILE_SIZE / 2, y * TILE_SIZE}; // topo
                    Vector2 p2 = {x * TILE_SIZE, y * TILE_SIZE + TILE_SIZE}; // inferior esquerdo
                    Vector2 p3 = {x * TILE_SIZE + TILE_SIZE, y * TILE_SIZE + TILE_SIZE}; // inferior direito
                    DrawTriangle(p1, p2, p3, cores_menu[idx_cor]);
                }
            }
        }

        if (tileX >= 0 && tileX < COLUNAS_MAPA && tileY >= 0 && tileY < LINHAS_MAPA)
        {
            Color cor_cursor = (mapa[tileY][tileX] == 0) ? (Color){0, 0, 255, 100} : (Color){255, 0, 0, 100};
            DrawRectangle(tileX * TILE_SIZE, tileY * TILE_SIZE, TILE_SIZE, TILE_SIZE, cor_cursor);
        }

        DrawRectangle(menu_x, menu_y, menu_width, menu_height, (Color){50, 50, 50, 200});
        DrawText("Menu de cores:", menu_x + 10, menu_y + 10, 20, RAYWHITE);

        for (int i = 0; i < COR_TOTAL; i++)
        {
            Color cor_atual = cores_menu[i];
            if (moedas < custos[i])
                cor_atual = (Color){ cor_atual.r / 3, cor_atual.g / 3, cor_atual.b / 3, cor_atual.a };

            int cor_pos_y = menu_y + 40 + i * 30;
            DrawRectangle(menu_x + 20, cor_pos_y, 20, 20, cor_atual);
            DrawText(
                (i == COR_ROSA) ? "Rosa" :
                (i == COR_AMARELO) ? "Amarelo" : "Laranja",
                menu_x + 50, cor_pos_y, 20, RAYWHITE
            );
        }

        char buffer[30];
        snprintf(buffer, sizeof(buffer), "Moedas: %d", moedas);
        DrawText(buffer, menu_x + 10, menu_y + menu_height - 30, 25, GOLD);

        if (arrastando)
        {
            Color cor = cores_menu[cor_arrastada];
            DrawRectangle(mouse.x - 16, mouse.y - 16, TILE_SIZE - 32, TILE_SIZE - 32, cor);
        }

        EndDrawing();
    }

    UnloadTexture(grama_textura);
    UnloadTexture(caminho_textura);

    CloseWindow();
    return 0;
}
