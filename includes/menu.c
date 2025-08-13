#include "menu.h"
#include <stdio.h>
#include <stdlib.h>
#include <raylib.h>

// Estrutura da tela 
typedef enum {
    MENU_PRINCIPAL,
    CONFIGURACOES
} EstadoTela;


int IniciarMenu() {
    int largura = 800; 
    int altura = 600;
    int saiu = 0;
    int clicou_x = 0;

    InitWindow(largura, altura, "Menu");
    SetTargetFPS(60);
    InitAudioDevice();

    Texture2D background = LoadTexture("Menu/imagens/menu.png");
    Texture2D icon = LoadTexture("Menu/imagens/configuracao.png");
    Texture2D pergaminho = LoadTexture("Menu/imagens/pergaminho.png");
    Music musica = LoadMusicStream("Menu/Som/menu.wav");

    // Deifinição das cores 
    Color LIGHTGREEN = (Color){144, 238, 144, 255};  
    Color LIGHTCORAL = (Color){240, 128, 128, 255}; 

    // inicializacao das cores 
    Color corBotaoIniciar = GREEN;
    Color corBotaoSair = RED;

    // definicao do tamano e posicao dos botoes 
    Rectangle botaoIniciar = {largura/2 - 100, 330, 200, 50};
    Rectangle botaoSair = {largura/2 - 50, 400, 100, 50};
    Rectangle botaoConfiguracao = {largura - 60, 20, 40, 40};
    Rectangle botaoVoltar = {largura/2 + 90, 450, 75, 40};
    Rectangle desligarSom = {largura/2 - 160, 180, 115, 40};
    Rectangle telacheia = {largura/2 - 160, 230, 190, 40};

    EstadoTela telaAtual = MENU_PRINCIPAL;

    PlayMusicStream(musica);

    // Presente nas cofiguracoes 
    bool somLigado = true;
    bool telaCheia = false;

    while (!WindowShouldClose()) {
        // pega a posicao do mouse para chegar colisao 
        Vector2 mouse = GetMousePosition();

        // Caso o mouse esteja em cima do botao iniciar 
        if (CheckCollisionPointRec(mouse, botaoIniciar)) {
            corBotaoIniciar = LIGHTGREEN;
        } else {
            corBotaoIniciar = GREEN;
        }
        // Caso o mouse esteja em cima do botao sair 
        if (CheckCollisionPointRec(mouse, botaoSair)) {
            corBotaoSair = LIGHTCORAL; 
        } else {
            corBotaoSair = RED;
        }

        // O que acontece quando o mouse clica em cada botao 
        if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
            if (telaAtual == MENU_PRINCIPAL) {
                if (CheckCollisionPointRec(mouse, botaoIniciar)) {
                    clicou_x = 1;
                    break; //sair do menu, abre a janela do jogo
                }
                if (CheckCollisionPointRec(mouse, botaoSair)) {
                    saiu = 1;
                    clicou_x = 1;
                    break; // sai do menu e nao abre a janela do jogo
                }
                if (CheckCollisionPointRec(mouse, botaoConfiguracao)) {
                    telaAtual = CONFIGURACOES;
                } // abre as configuracoes 
            } else if (telaAtual == CONFIGURACOES) {
                if (CheckCollisionPointRec(mouse, botaoVoltar)) {
                    telaAtual = MENU_PRINCIPAL;
                } // fecha as configuracoes 
            }
        }

        BeginDrawing();
        ClearBackground(RAYWHITE);

        // ajuda a adaptar o backgroung caso a tela seja redimensionada
        Rectangle sourceRecBg = { 0, 0, (float)background.width, (float)background.height };
        Rectangle destRecBg = { 0, 0, (float)largura, (float)altura };
        Vector2 origin = { 0, 0 };
        DrawTexturePro(background, sourceRecBg, destRecBg, origin, 0.0f, WHITE);

        if(telaAtual != CONFIGURACOES) {
            Rectangle sourceRecIcon = { 0, 0, (float)icon.width, (float)icon.height };
            DrawTexturePro(icon, sourceRecIcon, botaoConfiguracao, origin, 0.0f, WHITE);
        }

        if (telaAtual == MENU_PRINCIPAL) {
            UpdateMusicStream(musica); 
            DrawRectangleRec(botaoIniciar, corBotaoIniciar);
            DrawRectangleRec(botaoSair, corBotaoSair);

            // desenho e posicionamento do nome do jogo 
            int fontSize = 80;
            int x_start = 145;
            int y_start = 50;

            DrawText("CIN", x_start + 5, y_start + 5, fontSize, BLACK); // sombra do nome
            DrawText("CIN", x_start, y_start, fontSize, RED);

            int largura_cin = MeasureText("CIN", fontSize); // para ajuda r posiconar corretamente o defenda 
            DrawText("DEFENDA", x_start + largura_cin + 5, y_start + 5, fontSize, BLACK); // sombra do nome
            DrawText("DEFENDA", x_start + largura_cin, y_start, fontSize, BLUE);

            DrawText("Iniciar", 355, 340, 30, BLACK);  
            DrawText("Sair", 370, 410, 30, BLACK);  
              
        } else if (telaAtual == CONFIGURACOES) {
            UpdateMusicStream(musica);

            //  desenha o pergaminho como o fundo das configurações
            Rectangle sourceRecPergaminho = { 0, 0, (float)pergaminho.width, (float)pergaminho.height };
            Rectangle destRecPergaminho = { largura/2 - 350, altura/2 - 320, 700, 650 };
            DrawTexturePro(pergaminho, sourceRecPergaminho, destRecPergaminho, origin, 0.0f, WHITE);

            DrawText("CONFIGURAÇÕES", 310, 130, 20, BLACK);
            // desenha os botoes 
            DrawRectangleRec(desligarSom, LIGHTGRAY);
            DrawRectangleRec(telacheia, LIGHTGRAY);
            DrawRectangleRec(botaoVoltar, LIGHTGRAY);
            DrawText("Voltar", largura/2 + 95, 460, 20, BLACK);
            
            if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
                if (CheckCollisionPointRec(mouse, desligarSom)) {
                    somLigado = !somLigado;
                    if (somLigado) {
                        PlayMusicStream(musica);
                    } else {
                        StopMusicStream(musica);
                    }
                }
            }

            if (somLigado) {
                DrawText("Som: [ON]", desligarSom.x + 5, desligarSom.y + 10, 20, BLACK);
            } else {
                DrawText("Som: [OFF]", desligarSom.x + 5, desligarSom.y + 10, 20, DARKGRAY);
            }

            if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
                if (CheckCollisionPointRec(mouse, telacheia)) {
                    telaCheia = !telaCheia;
                    ToggleFullscreen(); //alterna entre o tamanho original e tela cheia
                    if (!telaCheia && IsWindowFullscreen()) {
                        SetWindowSize(largura, altura); // torna tela cheia
                    }
                }
            }
            
            if (telaCheia) {
                DrawText("Tela Cheia: [ON]", telacheia.x + 5, telacheia.y + 10, 20, BLACK);
            } else {
                DrawText("Tela Cheia: [OFF]", telacheia.x + 5, telacheia.y + 10, 20, DARKGRAY);
            }
        }

        EndDrawing();
    }   

    // Liberação da memoria ocupada penas texturas 
    UnloadTexture(background);
    UnloadTexture(icon);
    UnloadTexture(pergaminho);
    CloseWindow();
    
    if(clicou_x == 1){
        if(saiu){
            return 1;
        } else {
            return 0;
        }
    } else {
        return 1;
    }
}
