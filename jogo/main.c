#include "raylib.h"
#include <stdio.h>
#include "torres.h"
#include "menu.h"
#include "inimigos.h"
#include <raymath.h>
#include "nivel_2.h"


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

typedef enum {
    JOGO,
    CONFIGURACOES
} EstadoTela;

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

    int fase_atual = 1;
    Nivel2 nivel2 = {0};

    int continua;

    continua = IniciarMenu();

      if (continua == 1) {
            return 0;
        }

    // Variável para controlar o tempo de movimentação
    double lastMoveTime = 0.0;
    double moveInterval = 0.9; // Intervalo de movimento em segundos

    int visitados[LINHAS_MAPA][COLUNAS_MAPA] = {0};  // Inicializa todos como 0 (não visitados)

    const int largura = TILE_SIZE * COLUNAS_MAPA + 150;  // Inclui espaço para menu lateral
    const int altura = TILE_SIZE * LINHAS_MAPA;

    InitWindow(largura, altura, "Jogo de Torres");
    SetTargetFPS(60);

    // Carrega o sprite do inimigo
    Texture2D inimigoSprite = LoadTexture("personagens/Inimigos/zombie.png");

    if (inimigoSprite.width == 0 || inimigoSprite.height == 0) {
        // Se a textura não carregar corretamente, exibe uma mensagem de erro
        printf("Erro ao carregar o sprite do inimigo!\n");
        return -1;
    }

    // Define o sourceRec para a textura inteira
    Rectangle sourceRec = { 0.0f, 0.0f, (float)inimigoSprite.width, (float)inimigoSprite.height };

    // Carregar texturas do mapa
    Texture2D grama_textura = LoadTexture("jogo/imagens/grama.png");
    Texture2D caminho_textura = LoadTexture("jogo/imagens/caminho.png");
    Texture2D sprite_mago = LoadTexture("jogo/imagens/torre_mago.png");
    Texture2D sprite_arqueiro = LoadTexture("jogo/imagens/torre_arqueiro.png");
    Texture2D sprite_soldado = LoadTexture("jogo/imagens/torre_geral.png");
    Texture2D coracacao_vida = LoadTexture("jogo/imagens/coracao_vida.png");
    Texture2D imagem_moeda = LoadTexture("jogo/imagens/moeda.png");
    Texture2D icon = LoadTexture("Menu/imagens/configuracao.png");
    Texture2D pergaminho = LoadTexture("Menu/imagens/pergaminho.png");
    Texture2D caminho_brick = LoadTexture("jogo/imagens/caminho_brick.png");
    Texture2D textura_grama_n = LoadTexture("jogo/imagens/reck.png");

    Music musica = LoadMusicStream("Menu/Som/menu.wav");

    // Botões das configurações 
    Rectangle botaoConfiguracao = {largura - 95, 600, 40, 40};
    Rectangle botaoVoltar = {largura/2 + 90, 450, 75, 40};
    Rectangle desligarSom = {largura/2 - 160, 180, 115, 40};
    Rectangle telacheia = {largura/2 - 160, 230, 190, 40};
    Rectangle botaoSair = {largura/2 - 160, 280, 140, 40};

    EstadoTela telaAtual = JOGO;

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

    // Cria o inimigo
    Inimigo inimigo = CriarInimigo(100, 1, inimigoSprite);
    // Define a posição inicial do inimigo para o primeiro '1' da esquerda
    inimigo.posX = 9;
    inimigo.posY = 0;

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

        // Obtém o tempo atual
        double currentTime = GetTime();

        Vector2 mousePos = GetMousePosition();
        int tileX = mousePos.x / TILE_SIZE;
        int tileY = mousePos.y / TILE_SIZE;

        PlayMusicStream(musica);

        bool somLigado = true;
        bool telaCheia = false;

        // Clique no menu lateral
        if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
            if (mousePos.x > menuX) {
                int buttonIndex = (mousePos.y - 90) / altura_botao;
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
                mapa[tileY][tileX] == 0 &&
                !existe_torre_no_tile(tileX, tileY, soldados, num_soldados, arqueiros, num_arqueiros, magos, num_magos)) {
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

        if (moedas ==0 && fase_atual == 1) {
            fase_atual = 2;
            nivel2 = IniciarNivel2();
            
            // Atualiza para a fase 2
            memcpy(mapa, nivel2.mapa, sizeof(nivel2.mapa));
            vida = nivel2.vida_jogador;
            moedas = nivel2.moedas_iniciais;
            
            // Reinicia torres
            num_soldados = 0;
            num_arqueiros = 0;
            num_magos = 0;
        }

        // --- DESENHO ---
        BeginDrawing();
        ClearBackground(RAYWHITE);

        // Desenhar mapa
        for (int y = 0; y < LINHAS_MAPA; y++) {
            for (int x = 0; x < COLUNAS_MAPA; x++) {
            Vector2 pos = {x * TILE_SIZE, y * TILE_SIZE};
            Texture2D textura;
            if (fase_atual == 1) {
                textura = (mapa[y][x] == 1) ? caminho_textura : grama_textura;
            } else {
                textura = (mapa[y][x] == 1) ? caminho_brick : textura_grama_n;
        }
        
        DrawTextureRec(textura, (Rectangle){0, 0, TILE_SIZE, TILE_SIZE}, pos, WHITE);
        DrawTextureRec(textura, (Rectangle){0, 0, TILE_SIZE, TILE_SIZE}, pos, WHITE);
    }
        }
        // Movimentação do inimigo
        if (currentTime - lastMoveTime >= moveInterval) {
            MovimentarInimigo(&inimigo, mapa, visitados);
            lastMoveTime = currentTime;
        }


        double agora = GetTime();
        Vector2 inimigo_pos = { inimigo.posX * TILE_SIZE + TILE_SIZE / 2, inimigo.posY * TILE_SIZE + TILE_SIZE / 2 };

        // Soldados
        for (int i = 0; i < num_soldados; i++) {
            if (!soldados[i].ativo) continue;
            float dist = Vector2Distance(soldados[i].posicao, inimigo_pos);
            if (dist <= soldados[i].alcance) {
                if (agora - soldados[i].tempoUltimoTiro >= soldados[i].cooldown) {
                    inimigo.vida -= soldados[i].dano;
                    soldados[i].tempoUltimoTiro = agora;
                    soldados[i].tempoFimTiro = agora + 0.1;
                    printf("Soldado %d atirou! Vida inimigo: %d\n", i, inimigo.vida);
                }
                if (agora <= soldados[i].tempoFimTiro) {
                    DrawLineV(soldados[i].posicao, inimigo_pos, RED);
                }
            }
        }

        // Arqueiros
        for (int i = 0; i < num_arqueiros; i++) {
            if (!arqueiros[i].ativo) continue;
            float dist = Vector2Distance(arqueiros[i].posicao, inimigo_pos);
            if (dist <= arqueiros[i].alcance) {
                if (agora - arqueiros[i].tempoUltimoTiro >= arqueiros[i].cooldown) {
                    inimigo.vida -= arqueiros[i].dano;
                    arqueiros[i].tempoUltimoTiro = agora;
                    arqueiros[i].tempoFimTiro = agora + 0.1;
                    printf("Arqueiro %d atirou! Vida inimigo: %d\n", i, inimigo.vida);
                }
                if (agora <= arqueiros[i].tempoFimTiro) {
                    DrawLineV(arqueiros[i].posicao, inimigo_pos, GREEN);
                }
            }
        }

        // Magos
        for (int i = 0; i < num_magos; i++) {
            if (!magos[i].ativo) continue;
            float dist = Vector2Distance(magos[i].posicao, inimigo_pos);
            if (dist <= magos[i].alcance) {
                if (agora - magos[i].tempoUltimoTiro >= magos[i].cooldown) {
                    inimigo.vida -= magos[i].dano;
                    magos[i].tempoUltimoTiro = agora;
                    magos[i].tempoFimTiro = agora + 0.1;
                    printf("Mago %d atirou! Vida inimigo: %d\n", i, inimigo.vida);
                }
                if (agora <= magos[i].tempoFimTiro) {
                    DrawLineV(magos[i].posicao, inimigo_pos, BLUE);
                }
            }
        }




        if (inimigo.vida > 0) {
            Vector2 position = { inimigo.posY * 64.0f, inimigo.posX * 64.0f };
            Vector2 scale = { 64.0f / inimigoSprite.width, 64.0f / inimigoSprite.height };
            DrawTexturePro(inimigoSprite, sourceRec, (Rectangle){position.x, position.y, 64.0f, 64.0f},
                        (Vector2){0.0f, 0.0f}, 0.0f, WHITE);
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
        DrawText("LOJA", menuX + 50, 10, 20, RAYWHITE);
        DrawText("DE", menuX + 60, 30, 20, RAYWHITE);
        DrawText("TORRES", menuX + 30, 50, 20, RAYWHITE);

        for (int i = 0; i < TORRE_TOTAL; i++) {
            int yPos = 90 + i * altura_botao;
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

        // Tela de configurações
        Vector2 mouse = GetMousePosition();
        Vector2 origin = {0, 0};


        if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
            if (telaAtual == JOGO) {
                if (CheckCollisionPointRec(mouse, botaoConfiguracao)) {
                    telaAtual = CONFIGURACOES;
                }
            }
            else if (telaAtual == CONFIGURACOES) {
                if (CheckCollisionPointRec(mouse, botaoVoltar)) {
                    telaAtual = JOGO;
                }
            }
        }

        if (telaAtual == CONFIGURACOES) {
            UpdateMusicStream(musica);

            Rectangle sourceRecPergaminho = { 0, 0, (float)pergaminho.width, (float)pergaminho.height };
            Rectangle destRecPergaminho = { largura/2 - 350, altura/2 - 320, 700, 650 };
            DrawTexturePro(pergaminho, sourceRecPergaminho, destRecPergaminho, origin, 0.0f, WHITE);

            DrawText("CONFIGURAÇÕES", 630, 130, 20, BLACK);
            
            DrawRectangleRec(desligarSom, LIGHTGRAY);
            DrawRectangleRec(telacheia, LIGHTGRAY);
            DrawRectangleRec(botaoVoltar, LIGHTGRAY);
            DrawRectangleRec(botaoSair, LIGHTGRAY);
            DrawText("Voltar", largura/2 + 95, 460, 20, BLACK);
            DrawText("Sair do jogo", largura/2 - 155, 290, 20, BLACK);
            
            if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
                if (CheckCollisionPointRec(mouse, desligarSom)) {
                    somLigado = !somLigado;
                    if (somLigado) {
                        PlayMusicStream(musica);
                    } else {
                        StopMusicStream(musica);
                    }
                }
                else if (CheckCollisionPointRec(mouse, botaoSair)) {
                    break;
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
                    ToggleFullscreen();
                    if (!telaCheia && IsWindowFullscreen()) {
                        SetWindowSize(largura, altura);
                    }
                }
            }
            
            if (telaCheia) {
                DrawText("Tela Cheia: [ON]", telacheia.x + 5, telacheia.y + 10, 20, BLACK);
            } else {
                DrawText("Tela Cheia: [OFF]", telacheia.x + 5, telacheia.y + 10, 20, DARKGRAY);
            }
        }

        if(telaAtual != CONFIGURACOES) {
            Rectangle sourceRecIcon = { 0, 0, (float)icon.width, (float)icon.height };
            DrawTexturePro(icon, sourceRecIcon, botaoConfiguracao, origin, 0.0f, WHITE);
        }

        // Mostrar moedas
        Rectangle sourceMoeda = {0, 0, imagem_moeda.width, imagem_moeda.height};
        Rectangle destMoeda = {20, 20, 32, 32}; 

        const char* textoMoedas = TextFormat("%d coins", moedas);
        int xMoedas = 62;
        int yMoedas = 20;
        int fontSizeMoedas = 30;

        DrawTexturePro(imagem_moeda, sourceMoeda, destMoeda, origin, 0.0f, WHITE);
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
    UnloadTexture(icon);
    UnloadTexture(pergaminho);
    UnloadTexture(coracacao_vida);
    UnloadTexture(imagem_moeda);
    UnloadTexture(caminho_brick);
    UnloadTexture(textura_grama_n);

    CloseWindow();
    return 0;
}
