#include "game_over.h"
#include <stdio.h>
#include <stdlib.h>

typedef enum {
    GAME_OVER,
    CONFIGURACOES_GAMEMOVER
} EstadoTela;

Texture2D background;
Texture2D icon;
Texture2D pergaminho;
Music musica;

static bool initialized = false;

int RunGameOver(const int largura, const int altura) {
    static int saiu = 0;
    static int clicou_x = 0;

    if (!initialized)
    {
        background = LoadTexture("/home/CIN/gafv/Downloads/game_over.png");
        icon = LoadTexture("/home/CIN/gafv/Downloads/configuracao.png");
        pergaminho = LoadTexture("/home/CIN/gafv/Downloads/pergaminho.png");
        musica = LoadMusicStream("/home/CIN/gafv/Downloads/menu.wav");

        initialized = true;
    }

    const Color LIGHTBLUE = (Color){173, 216, 230, 255};
    const Color LIGHTCORAL = (Color){240, 128, 128, 255};
    const Color DARKRED = (Color){139, 0, 0, 255}; 
    const Color darkgray = (Color){169, 169, 169, 255};

    static Color corBotaoJogarNovamente = BLUE;
    static Color corBotaoSair = RED;

    const Rectangle botaoJogarNovamente = {largura/2 - 125, 430, 250, 50};
    const Rectangle botaoSair = {largura/2 - 50, 500, 100, 50};
    const Rectangle botaoConfiguracao = {largura - 60, 20, 40, 40};
    const Rectangle botaoVoltar = {largura/2 + 90, 450, 75, 40};
    const Rectangle desligarSom = {largura/2 - 160, 180, 115, 40};
    const Rectangle telacheia = {largura/2 - 160, 230, 190, 40};

    static EstadoTela telaAtual = GAME_OVER;

    PlayMusicStream(musica);

    static bool somLigado = true;
    static bool telaCheia = false;

    Vector2 mouse = GetMousePosition();

    if (CheckCollisionPointRec(mouse, botaoJogarNovamente)) {
        corBotaoJogarNovamente = LIGHTBLUE;
    } else {
        corBotaoJogarNovamente = BLUE;
    }

    if (CheckCollisionPointRec(mouse, botaoSair)) {
        corBotaoSair = LIGHTCORAL; 
    } else {
        corBotaoSair = RED;
    }

    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
        if (telaAtual == GAME_OVER) {
            if (CheckCollisionPointRec(mouse, botaoJogarNovamente)) {
                clicou_x = 1;
                return 1;
            }
            if (CheckCollisionPointRec(mouse, botaoSair)) {
                saiu = 1;
                clicou_x = 1;
                return 2;
            }
            if (CheckCollisionPointRec(mouse, botaoConfiguracao)) {
                telaAtual = CONFIGURACOES_GAMEMOVER;
            }
        } else if (telaAtual == CONFIGURACOES_GAMEMOVER) {
            if (CheckCollisionPointRec(mouse, botaoVoltar)) {
                telaAtual = GAME_OVER;
            }
        }
    }

    BeginDrawing();
    ClearBackground(RAYWHITE);

    Rectangle sourceRecBg = { 0, 0, (float)background.width, (float)background.height };
    Rectangle destRecBg = { 0, 0, (float)largura, (float)altura };
    Vector2 origin = { 0, 0 };
    DrawTexturePro(background, sourceRecBg, destRecBg, origin, 0.0f, WHITE);

    if(telaAtual != CONFIGURACOES_GAMEMOVER) {
        Rectangle sourceRecIcon = { 0, 0, (float)icon.width, (float)icon.height };
        DrawTexturePro(icon, sourceRecIcon, botaoConfiguracao, origin, 0.0f, WHITE);
    }

    if (telaAtual == GAME_OVER) {
        UpdateMusicStream(musica);

        DrawRectangleRec(botaoJogarNovamente, corBotaoJogarNovamente);
        DrawRectangleRec(botaoSair, corBotaoSair);

        int fontSize = 80;
        int x_start = 170;
        int y_start = 50;

        DrawText("GAME", x_start + 5, y_start + 5, fontSize, BLACK);
        DrawText("GAME", x_start, y_start, fontSize, DARKRED);

        int largura_game = MeasureText("GAME", fontSize);
        DrawText("OVER", x_start + largura_game + 15, y_start + 5, fontSize, BLACK);
        DrawText("OVER", x_start + largura_game + 10, y_start, fontSize, DARKRED);

        DrawText("Jogar Novamente", 283, 440, 28, BLACK);  
        DrawText("Sair", 370, 510, 30, BLACK);  
            
    } else if (telaAtual == CONFIGURACOES_GAMEMOVER) {
        UpdateMusicStream(musica);

        Rectangle sourceRecPergaminho = { 0, 0, (float)pergaminho.width, (float)pergaminho.height };
        Rectangle destRecPergaminho = { largura/2 - 350, altura/2 - 320, 700, 650 };
        DrawTexturePro(pergaminho, sourceRecPergaminho, destRecPergaminho, origin, 0.0f, WHITE);

        DrawText("CONFIGURAÇÕES", 310, 130, 20, BLACK);

        // Desenha o botão de voltar
        DrawText("Voltar", largura/2 + 95, 460, 20, BLACK);

        if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
            if (CheckCollisionPointRec(mouse, desligarSom)) {
                if (somLigado) {
                    somLigado = false;
                    StopMusicStream(musica);
                } else {
                    somLigado = true;
                    PlayMusicStream(musica);
                }
            }
            if (CheckCollisionPointRec(mouse, telacheia)) {
                if (!telaCheia) {
                    telaCheia = true;
                    ToggleFullscreen();
                } else {
                    telaCheia = false;
                    if (IsWindowFullscreen()) {
                        ToggleFullscreen();
                        SetWindowSize(largura, altura);
                    }
                }
            }
        }

        if (somLigado) {
            DrawText("Som: [ON]", desligarSom.x + 5, desligarSom.y + 10, 20, BLACK);
        } else {
            DrawText("Som: [OFF]", desligarSom.x + 5, desligarSom.y + 10, 20, darkgray);
        }

        if (telaCheia) {
            DrawText("Tela Cheia: [ON]", telacheia.x + 5, telacheia.y + 10, 20, BLACK);
        } else {
            DrawText("Tela Cheia: [OFF]", telacheia.x + 5, telacheia.y + 10, 20, darkgray);
        }
    }

    EndDrawing();

    return 0;
}

void DeInitGameOver(void)
{
    UnloadTexture(background);
    UnloadTexture(icon);
    UnloadTexture(pergaminho);
    UnloadMusicStream(musica);
}
