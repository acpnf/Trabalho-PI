#include <stdio.h>
#include <stdlib.h>
#include <raylib.h>

typedef enum {
    MENU_PRINCIPAL,
    CONFIGURACOES
} EstadoTela;

int main() {
    
    int largura = 800; 
    int altura = 600;

    InitWindow(largura, altura, "Menu");
    SetTargetFPS(60);

    // Inicializa o dispositivo de áudio
    // Isso é necessário para carregar e reproduzir sons
    InitAudioDevice();

    // Carrega as texturas
    Texture2D background = LoadTexture("/home/CIN/gafv/Downloads/menu.png");
    Texture2D icon = LoadTexture("/home/CIN/gafv/Downloads/configuracao.png");
    Texture2D pergaminho = LoadTexture("/home/CIN/gafv/Downloads/pergaminho.png");

    // Carrega a música
    Music musica = LoadMusicStream("/home/CIN/gafv/Downloads/menu.wav");  // ou .mp3
    
    Color LIGHTGREEN = (Color){144, 238, 144, 255};  
    Color LIGHTCORAL = (Color){240, 128, 128, 255}; 

    Color corBotaoIniciar = GREEN;
    Color corBotaoSair = RED;

    // Criação de botões
    Rectangle botaoIniciar = {largura/2 - 100, 330, 200, 50};
    Rectangle botaoSair = {largura/2 - 50, 400, 100, 50};
    Rectangle botaoConfiguracao = {largura - 60, 20, 40, 40};
    Rectangle botaoVoltar = {largura/2 + 90, 450, 75, 40};
    Rectangle desligarSom = {largura/2 - 160, 180, 115, 40};
    Rectangle telacheia = {largura/2 - 160, 230, 190, 40};

    EstadoTela telaAtual = MENU_PRINCIPAL;

    // Inicia a música
    PlayMusicStream(musica);

    bool somLigado = true;
    bool telaCheia = false;
    

    // Loop principal do jogo
    while (!WindowShouldClose()) {
        Vector2 mouse = GetMousePosition();

        if (CheckCollisionPointRec(mouse, botaoIniciar)) {
            corBotaoIniciar = LIGHTGREEN;
        } else {
            corBotaoIniciar = GREEN;
        }

        if (CheckCollisionPointRec(mouse, botaoSair)) {
            corBotaoSair = LIGHTCORAL; 
        } else {
            corBotaoSair = RED;
        }


        if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
            if (telaAtual == MENU_PRINCIPAL) {
                if (CheckCollisionPointRec(mouse, botaoIniciar)) {
                    // Vai para a tela de jogo futuramente
                }
                if (CheckCollisionPointRec(mouse, botaoSair)) {
                    break;
                }
                if (CheckCollisionPointRec(mouse, botaoConfiguracao)) {
                    telaAtual = CONFIGURACOES;
                }
            } else if (telaAtual == CONFIGURACOES) {
                if (CheckCollisionPointRec(mouse, botaoVoltar)) {
                    telaAtual = MENU_PRINCIPAL;
                }
            }
        }

        BeginDrawing();
        ClearBackground(RAYWHITE);

        // Desenha o background
        Rectangle sourceRecBg = { 0, 0, (float)background.width, (float)background.height };
        Rectangle destRecBg = { 0, 0, (float)largura, (float)altura };
        Vector2 origin = { 0, 0 };
        DrawTexturePro(background, sourceRecBg, destRecBg, origin, 0.0f, WHITE);

        // Desenha o ícone de configurações, exceto na tela de configurações
        if(telaAtual != CONFIGURACOES) {
            // Desenha o ícone de configurações
            Rectangle sourceRecIcon = { 0, 0, (float)icon.width, (float)icon.height };
            DrawTexturePro(icon, sourceRecIcon, botaoConfiguracao, origin, 0.0f, WHITE);
        }

        if (telaAtual == MENU_PRINCIPAL) {

            // Atualiza a música de fundo
            UpdateMusicStream(musica); 
            
            DrawRectangleRec(botaoIniciar, corBotaoIniciar);
            DrawRectangleRec(botaoSair, corBotaoSair);


            int fontSize = 80;
            int x_start = 145;
            int y_start = 50;

            DrawText("CIN", x_start + 5, y_start + 5, fontSize, BLACK);
            DrawText("CIN", x_start, y_start, fontSize, RED);

            int largura_cin = MeasureText("CIN", fontSize);
            DrawText("DEFENDA", x_start + largura_cin + 5, y_start + 5, fontSize, BLACK);
            DrawText("DEFENDA", x_start + largura_cin, y_start, fontSize, BLUE);

            DrawText("Iniciar", 355, 340, 30, BLACK);  
            DrawText("Sair", 370, 410, 30, BLACK);  
              
        } else if (telaAtual == CONFIGURACOES) {

            // Atualiza a música de fundo
            UpdateMusicStream(musica);

            // Desenha o pergaminho no centro da tela
            Rectangle sourceRecPergaminho = { 0, 0, (float)pergaminho.width, (float)pergaminho.height };
            Rectangle destRecPergaminho = { largura/2 - 350, altura/2 - 320, 700, 650 };
            DrawTexturePro(pergaminho, sourceRecPergaminho, destRecPergaminho, origin, 0.0f, WHITE);

            DrawText("CONFIGURAÇÕES", 310, 130, 20, BLACK);

            
            DrawRectangleRec(desligarSom, LIGHTGRAY); //SERA RETIRADO DEPOIS
            DrawRectangleRec(telacheia, LIGHTGRAY); //SERA RETIRADO DEPOIS
            DrawRectangleRec(botaoVoltar, LIGHTGRAY); //SERA RETIRADO DEPOIS
            
            
            // Desenha o botão de voltar
            DrawText("Voltar", largura/2 + 95, 460, 20, BLACK);
            
            // Desenha o texto do botão de desligar som
            if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
                // Verifica se o mouse está sobre o botão de desligar som
                if (CheckCollisionPointRec(mouse, desligarSom)) {
                    if (somLigado == true) {

                        somLigado = false; // Desliga o som
                        StopMusicStream(musica); // Para a música se o som estiver desligado
        
                    } else {
                        
                        somLigado = true; // Liga o som
                        PlayMusicStream(musica); // Reproduz a música se o som estiver ligado
                        
                    }
                }
            }

            if (somLigado == true) {
                DrawText("Som: [ON]", desligarSom.x + 5, desligarSom.y + 10, 20, BLACK);
            }
            else {
                DrawText("Som: [OFF]", desligarSom.x + 5, desligarSom.y + 10, 20, DARKGRAY);
            }

            
            if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {

                if (CheckCollisionPointRec(mouse, telacheia)) {
                    if (telaCheia == false) {
                        telaCheia = true; // Alterna para tela cheia
                        ToggleFullscreen();
                    } else {
                        telaCheia = false; // Alterna para janela normal
                        if(IsWindowFullscreen()) {
                            ToggleFullscreen();
                            SetWindowSize(largura, altura);
                        }

                    }
                }
            }
            
            if (telaCheia == true) {
                DrawText("Tela Cheia: [ON]", telacheia.x + 5, telacheia.y + 10, 20, BLACK);
            } else {
                DrawText("Tela Cheia: [OFF]", telacheia.x + 5, telacheia.y + 10, 20, DARKGRAY);
            }
            
        }

        EndDrawing();
    }

    UnloadTexture(background);
    UnloadTexture(icon);
    UnloadTexture(pergaminho);
    CloseWindow();

    return 0;
}
