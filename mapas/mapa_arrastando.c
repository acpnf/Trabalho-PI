#include "raylib.h"
#include <stdio.h>
#include "torres.h"
#include "menu.h"

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

int main(void) {

    IniciarMenu(); // Inicia o menu antes do jogo

    const int largura = TILE_SIZE * COLUNAS_MAPA + 150;  // Inclui espaço para menu lateral
    const int altura = TILE_SIZE * LINHAS_MAPA;

    InitWindow(largura, altura, "Jogo de Torres");
    SetTargetFPS(60);

    // Carregar texturas do mapa
    Texture2D grama_textura = LoadTexture("mapas/imagens/grama.png");
    Texture2D caminho_textura = LoadTexture("mapas/imagens/caminho.png");
    Texture2D sprite_mago = LoadTexture("personagens/torres/pngsTeste/tower_4_transparent-removebg-preview.png");
    Texture2D sprite_arqueiro = LoadTexture("personagens/torres/pngsTeste/tower_5_transparent-removebg-preview.png");
    Texture2D sprite_soldado = LoadTexture("personagens/torres/pngsTeste/tower_6_transparent-removebg-preview.png");
    Texture2D coracacao_vida = LoadTexture("mapas/imagens/coracao_vida.png");
    Texture2D imagem_moeda = LoadTexture("mapas/imagens/moeda.png");

    // Inicializar torres (carrega os sprites internos)
    iniciar_torres();

    int mapa[LINHAS_MAPA][COLUNAS_MAPA] = {
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 1, 1, 1, 0, 0, 1, 0, 0, 0, 1, 1, 1, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 0, 0},
        {0, 1, 1, 1, 0, 0, 0, 1, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 0, 0},
        {0, 1, 0, 1, 0, 0, 0, 1, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 0, 0},
        {0, 1, 0, 1, 0, 0, 0, 1, 0, 0, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0},
        {1, 1, 0, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}
    };

    // Sistema de torres
    Soldado soldados[MAX_TORRES];
    Arqueiro arqueiros[MAX_TORRES];
    Mago magos[MAX_TORRES];
    int num_soldados = 0;
    int num_arqueiros = 0;
    int num_magos = 0;

    // Sistema de recursos
    int moedas = 100;
    int custos[TORRE_TOTAL] = {CUSTO_SOLDADO, CUSTO_ARQUEIRO, CUSTO_MAGO};
    const char* nomes_torres[TORRE_TOTAL] = {"Soldado", "Arqueiro", "Mago"};

    // Sistema de vida 
    int vida = 500; 

    // Controle de torres
    bool arrastando = false;
    TipoTorre torre_arrastada = TORRE_SOLDADO;

    int menuWidth = 150;
    int altura_botao = 50;
    int menuX = largura - menuWidth;

    while (!WindowShouldClose()) {
        Vector2 mousePos = GetMousePosition();
        int tileX = mousePos.x / TILE_SIZE;
        int tileY = mousePos.y / TILE_SIZE;

        // Clique no menu lateral
        if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
            if (mousePos.x > menuX) {
                int buttonIndex = (mousePos.y - 40) / altura_botao;
                if (buttonIndex >= 0 && buttonIndex < TORRE_TOTAL && moedas >= custos[buttonIndex]) {
                    arrastando = true;
                    torre_arrastada = buttonIndex;
                }
            }
        }

        // Soltar torre
        if (IsMouseButtonReleased(MOUSE_LEFT_BUTTON) && arrastando) {
            arrastando = false;
            if (tileX >= 0 && tileX < COLUNAS_MAPA &&
                tileY >= 0 && tileY < LINHAS_MAPA &&
                mousePos.x < menuX &&
                mapa[tileY][tileX] == 0)
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

        // --- DESENHO ---
        BeginDrawing();
        ClearBackground(RAYWHITE);

        // Desenhar mapa
        for (int y = 0; y < LINHAS_MAPA; y++) {
            for (int x = 0; x < COLUNAS_MAPA; x++) {
                Vector2 pos = {x * TILE_SIZE, y * TILE_SIZE};
                Texture2D textura = (mapa[y][x] == 1) ? caminho_textura : grama_textura;
                DrawTextureRec(textura, (Rectangle){0, 0, TILE_SIZE, TILE_SIZE}, pos, WHITE);
            }
        }

        // Desenhar tile azul/vermelho no cursor
        if (tileX >= 0 && tileX < COLUNAS_MAPA && tileY >= 0 && tileY < LINHAS_MAPA) {
            Color cor_cursor = (mapa[tileY][tileX] == 0) ? (Color){0, 0, 255, 100} : (Color){255, 0, 0, 100};
            DrawRectangle(tileX * TILE_SIZE, tileY * TILE_SIZE, TILE_SIZE, TILE_SIZE, cor_cursor);
        }

        float tamanho_torre = 100;
        // Desenhar torres no mapa
        for (int i = 0; i < num_soldados; i++) {
            desenhar_soldado(soldados[i], tamanho_torre, tamanho_torre);
        }
        for (int i = 0; i < num_arqueiros; i++) {
            desenhar_arqueiro(arqueiros[i], tamanho_torre, tamanho_torre);
        }
        for (int i = 0; i < num_magos; i++) {
            desenhar_mago(magos[i], tamanho_torre, tamanho_torre);
        }

        // Desenhar menu lateral
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

        // Torre sendo arrastada com o mouse
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

        // Mostrar moedas
        Rectangle sourceMoeda = {0, 0, imagem_moeda.width, imagem_moeda.height};
        Rectangle destMoeda = {20, 20, 32, 32}; 
        Vector2 originMoeda = {0, 0};

        const char* textoMoedas = TextFormat("%d coins", moedas);
        int xMoedas = 62;
        int yMoedas = 20;
        int fontSizeMoedas = 30;

        DrawTexturePro(imagem_moeda, sourceMoeda, destMoeda, originMoeda, 0.0f, WHITE);
        DrawText(TextFormat("%d HP", vida), 62, 65, 25, RED);


        // Borda preta 4 direções
        DrawText(textoMoedas, xMoedas - 1, yMoedas, fontSizeMoedas, BLACK);
        DrawText(textoMoedas, xMoedas + 1, yMoedas, fontSizeMoedas, BLACK);
        DrawText(textoMoedas, xMoedas, yMoedas - 1, fontSizeMoedas, BLACK);
        DrawText(textoMoedas, xMoedas, yMoedas + 1, fontSizeMoedas, BLACK);

        // Texto principal em dourado
        DrawText(textoMoedas, xMoedas, yMoedas, fontSizeMoedas, GOLD);
        DrawText("Arraste torres para o mapa", 20, altura - 30, 20, LIGHTGRAY);

        // Mostrar coracao 
        Rectangle sourceRec = {0, 0, coracacao_vida.width, coracacao_vida.height};
        Rectangle destRec = {20, 60, 32, 32};  
        Vector2 origin = {0, 0};

        DrawTexturePro(coracacao_vida, sourceRec, destRec, origin, 0.0f, WHITE);
        DrawText(TextFormat("%d HP", vida), 62, 65, 25, RED);

        
        const char* textoVida = TextFormat("%d HP", vida);
        int xVida = 62;
        int yVida = 65;
        int fontSize = 25;

        // Borda preta
        DrawText(textoVida, xVida - 1, yVida, fontSize, BLACK);
        DrawText(textoVida, xVida + 1, yVida, fontSize, BLACK);
        DrawText(textoVida, xVida, yVida - 1, fontSize, BLACK);
        DrawText(textoVida, xVida, yVida + 1, fontSize, BLACK);

        // Desenha texto principal em vermelho
        DrawText(textoVida, xVida, yVida, fontSize, RED);
        
        EndDrawing();
    }

    // Liberação de recursos
    liberar_torres();
    UnloadTexture(grama_textura);
    UnloadTexture(caminho_textura);
    UnloadTexture(sprite_soldado);
    UnloadTexture(sprite_arqueiro);
    UnloadTexture(sprite_mago);

    CloseWindow();
    return 0;
}
