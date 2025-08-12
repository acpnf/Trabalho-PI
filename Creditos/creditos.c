#include "raylib.h"
#include "raymath.h"
#include "creditos.h"

int creditos(void)
{
    // Inicialização
    #define MAX_OBSTACULOS 10
    const int largura_tela = 800;
    const int altura_tela = 450;
    InitWindow(largura_tela, altura_tela, "Jogo do Retangulo Saltador");
    
    InitAudioDevice();

    // Carrega os sons e texturas
    Sound som_salto = LoadSound("assets/pulo.wav");
    Sound som_pontuacao = LoadSound("assets/ponto.wav");
    Sound som_subiu_de_nivel = LoadSound("assets/levelup.wav");
    Sound som_game_over = LoadSound("assets/gameover.wav");
    Music musica_nivel1 = LoadMusicStream("assets/musica_level1.wav");
    Music musica_nivel2 = LoadMusicStream("assets/musica_level2.wav");
    Texture2D quadro_jogador1 = LoadTexture("assets/MOVIMENTO DIREITA-1.png");
    Texture2D quadro_jogador2 = LoadTexture("assets/MOVIMENTO DIREITA-2.png");
    Texture2D quadro_jogador3 = LoadTexture("assets/MOVIMENTO DIREITA-3.png");
    // Texturas de cenário (mantidas)
    Texture2D background = LoadTexture("assets/background.png");
    Texture2D background2 = LoadTexture("assets/background2.png");
    Texture2D background3 = LoadTexture("assets/background3.png"); // nuvem dia
    Texture2D background4 = LoadTexture("assets/background4.png"); // nuvem dia
    Texture2D background_noite = LoadTexture("assets/background_noite.png");
    Texture2D background_noite2 = LoadTexture("assets/background_noite2.png");
    Texture2D background_noite3 = LoadTexture("assets/background_noite3.png"); // nuvem noite

    // Texturas específicas dos créditos (NÃO são cenário)
    Texture2D credito1 = LoadTexture("assets/credito.png");
    Texture2D credito2 = LoadTexture("assets/credito2.png");
    Texture2D credito3 = LoadTexture("assets/credito3.png");
    Texture2D credito4 = LoadTexture("assets/credito4.png");
    Texture2D credito5 = LoadTexture("assets/credito5.png");
    Texture2D credito6 = LoadTexture("assets/credito6.png");
    Texture2D credito7 = LoadTexture("assets/credito7.png");
    Texture2D credito8 = LoadTexture("assets/credito8.png");
    Texture2D credito9 = LoadTexture("assets/credito9.png");
    Texture2D credito10 = LoadTexture("assets/credito10.png");
    Texture2D credito11 = LoadTexture("assets/credito11.png");
    Texture2D credito12 = LoadTexture("assets/credito12.png");

    int chao_y = altura_tela - 50;

    // Variáveis de estado do jogo
    bool iniciou = false;
    bool fim_de_jogo = false;
    int pontos = 0;
    int pontuacao_maxima = 0;
    int nivel = 0;
    bool subiu_nivel = false;

    // Tela de créditos (novo comportamento)
    bool mostrar_creditos = true;      // enquanto true mostramos slides / tela de créditos
    bool creditos_aguardando_inicio = false; // true quando créditos terminaram e aguardam tecla para iniciar
        float timer_creditos = 0.0f;
        const float duracao_slide = 2.0f;        // tempo por slide (s)
        const int num_slides = 12;
        const float duracao_creditos = duracao_slide * num_slides; // tempo total dos créditos (s)
        Texture2D creditos_slides[12] = {0};         

    // Configurações do Jogador (Caixa de COLISÃO - não mexer no tamanho)
    Rectangle jogador = { 80, chao_y - 50, 40, 50 };
    float velocidade_jogador = 0;
    const float gravidade = 1000.0f;
    const float velocidade_pulo = -400.0f;
    bool jogador_no_chao = true;

    // Variáveis de animação
    int quadro_atual = 0;
    int contador_quadros = 0;
    int velocidade_animacao = 8;

    // Configurações dos Obstáculos
    Rectangle obstaculos[MAX_OBSTACULOS] = { 0 };
    bool obstaculo_ativo[MAX_OBSTACULOS] = { 0 };
    float velocidade_obstaculo = 300.0f;
    float temporizador_spawn = 0.0f;
    float cooldown_spawn = 1.5f;

    // Configurações de FPS
    SetTargetFPS(60);

    int flag_musica = 0;

    // inicializa array de slides com as texturas de créditos (APENAS estas)
        creditos_slides[0] = credito1;
        creditos_slides[1] = credito2;
        creditos_slides[2] = credito3;
        creditos_slides[3] = credito4;
        creditos_slides[4] = credito5;
        creditos_slides[5] = credito6;
        creditos_slides[6] = credito7;
        creditos_slides[7] = credito8;
        creditos_slides[8] = credito9;
        creditos_slides[9] = credito10;
        creditos_slides[10] = credito11;
        creditos_slides[11] = credito12;

   
    // Nuvens de dia (arrays)
    const int NUM_NUVENS = 6;
    float nuvem_x[NUM_NUVENS];
    float nuvem_speed[NUM_NUVENS];

    for (int i = 0; i < NUM_NUVENS; i++)
    {
        nuvem_x[i] = (float)GetRandomValue(0, largura_tela * 2);
        nuvem_speed[i] = (float)GetRandomValue(20, 110);
    }

    // Nuvem de noite 
    float nuvem_noite_x = 0.0f - background_noite3.width;
    float nuvem_noite_speed = 60.0f;

    // background_noite2: 
    float bn2_scroll = 0.0f;
    float bn2_speed = 30.0f; // pixels por segundo


    // --- LOOP PRINCIPAL ÚNICO ---
    while (!WindowShouldClose())
    {
        // === LÓGICA DE CRÉDITOS / INÍCIO ===

        // Durante os créditos: avançam automaticamente.
        // Pressionar ESPAÇO **pula** os créditos (vai para o final), mas NÃO inicia o jogo.
    if (mostrar_creditos && !creditos_aguardando_inicio)
        {
            timer_creditos += GetFrameTime();

            if (IsKeyPressed(KEY_SPACE))
            {
                // pula para o final dos créditos (não inicia)
                timer_creditos = duracao_creditos;
            }

            if (timer_creditos >= duracao_creditos)
            {
                // créditos terminaram — ficar na última tela até o jogador apertar ESPAÇO para iniciar
                creditos_aguardando_inicio = true;
                // manter mostrar_creditos = true para continuar exibindo a última slide / mensagem
            }
        }

        // Se créditos já terminaram e estivermos aguardando tecla, só começamos quando jogador apertar ESPAÇO
    if (creditos_aguardando_inicio)
        {
            if (IsKeyPressed(KEY_SPACE))
            {
                mostrar_creditos = false;
                creditos_aguardando_inicio = false;
                iniciou = true;
                nivel = 1;
            }
        }

        // Lógica de música de fundo (mantida, só dispara quando nivel == 1 etc.)
        if(nivel == 1 && flag_musica == 0)
        {
            PlayMusicStream(musica_nivel1);
            flag_musica += 1;
        } 
        else if(nivel == 1 && flag_musica == 1)
        {
            UpdateMusicStream(musica_nivel1);
        } 
        else if (nivel == 2 && flag_musica == 1)
        {
            StopMusicStream(musica_nivel1);
            PlayMusicStream(musica_nivel2);
            flag_musica += 1;
        } 
        else if (!subiu_nivel && !fim_de_jogo)
        {
            UpdateMusicStream(musica_nivel2);
        }
        
        // Lógica para iniciar o jogo (se já não iniciou e não estiver em créditos)
    if (!iniciou && !mostrar_creditos && !creditos_aguardando_inicio)
        {
            if (IsKeyPressed(KEY_SPACE)) {
                iniciou = true;
                nivel = 1;
            }
        }

        // Lógica de jogo (só roda quando iniciado e não em créditos)
        if (iniciou && !mostrar_creditos)
        {
            if (subiu_nivel)
            {
                if (IsKeyPressed(KEY_SPACE))
                {   
                    subiu_nivel = false;
                    temporizador_spawn = 0;
                    flag_musica += 1;
                }
            }
            else if (!fim_de_jogo)
            {
                contador_quadros++;
                if (contador_quadros >= (60/velocidade_animacao))
                {
                    contador_quadros = 0;
                    quadro_atual++;
                    if (quadro_atual > 2) quadro_atual = 0;
                }
                
                if (IsKeyPressed(KEY_SPACE) && jogador_no_chao)
                {
                    velocidade_jogador = velocidade_pulo;
                    jogador_no_chao = false;
                    PlaySound(som_salto);
                }

                if (!jogador_no_chao) velocidade_jogador += gravidade * GetFrameTime();
                jogador.y += velocidade_jogador * GetFrameTime();
                if (jogador.y >= chao_y - jogador.height)
                {
                    jogador.y = chao_y - jogador.height;
                    velocidade_jogador = 0;
                    jogador_no_chao = true;
                }

                temporizador_spawn += GetFrameTime();
                if (temporizador_spawn >= cooldown_spawn)
                {
                    temporizador_spawn = 0;
                    cooldown_spawn = GetRandomValue(12, 25) / 10.0f;
                    int chance = GetRandomValue(0, 10);
                    int obstaculos_para_gerar = (chance > 7) ? 2 : 1;
                    int ultima_pos_x = largura_tela;

                    for (int i = 0; i < obstaculos_para_gerar; i++)
                    {
                        for (int j = 0; j < MAX_OBSTACULOS; j++)
                        {
                            if (!obstaculo_ativo[j])
                            {
                                obstaculo_ativo[j] = true;
                                if (nivel == 1)
                                {
                                    obstaculos[j].height = GetRandomValue(30, 55);
                                    obstaculos[j].width = GetRandomValue(20, 35);
                                }
                                else
                                {
                                    obstaculos[j].height = GetRandomValue(40, 63);
                                    obstaculos[j].width = GetRandomValue(25, 40);
                                }
                                obstaculos[j].y = chao_y - obstaculos[j].height;
                                int intervalo = GetRandomValue(37, 70);
                                obstaculos[j].x = ultima_pos_x + (i * intervalo);
                                ultima_pos_x = obstaculos[j].x;
                                break;
                            }
                        }
                    }
                }

                for (int i = 0; i < MAX_OBSTACULOS; i++)
                {
                    if (obstaculo_ativo[i])
                    {
                        obstaculos[i].x -= velocidade_obstaculo * GetFrameTime();
                        if (obstaculos[i].x < -obstaculos[i].width)
                        {
                            obstaculo_ativo[i] = false;
                            pontos++;
                            PlaySound(som_pontuacao);

                            float velocidade_maxima = (nivel == 1) ? 1000.0f : 1500.0f;
                            if (velocidade_obstaculo < velocidade_maxima) velocidade_obstaculo += 10.0f;
                        }
                        if (CheckCollisionRecs(jogador, obstaculos[i]))
                        {
                            fim_de_jogo = true;
                            StopMusicStream(musica_nivel2);
                            flag_musica = 0;
                            PlaySound(som_game_over);
                            if (pontos > pontuacao_maxima) pontuacao_maxima = pontos;
                        }
                    }
                }

                if (nivel == 1 && pontos >= 50)
                {
                    nivel = 2;
                    subiu_nivel = true;
                    PlaySound(som_subiu_de_nivel);
                    velocidade_obstaculo += 500.0f;
                    for (int i = 0; i < MAX_OBSTACULOS; i++) obstaculo_ativo[i] = false;
                }
            }
            else
            {
                if (IsKeyPressed(KEY_ENTER))
                {
                    fim_de_jogo = false;
                    iniciou = true;
                    pontos = 0;
                    nivel = 1;
                    velocidade_obstaculo = 300.0f;
                    jogador.y = chao_y - jogador.height;
                    velocidade_jogador = 0;
                    temporizador_spawn = 0;
                    cooldown_spawn = 1.5f;
                    for (int i = 0; i < MAX_OBSTACULOS; i++) obstaculo_ativo[i] = false;
                }
            }
        }

        // Nuvens de dia (arrays)
        for (int i = 0; i < NUM_NUVENS; i++)
        {
            nuvem_x[i] -= nuvem_speed[i] * GetFrameTime();
            if (nuvem_x[i] < - (float)largura_tela)
            {
                nuvem_x[i] = (float)(largura_tela + GetRandomValue(20, 400));
                nuvem_speed[i] = (float)GetRandomValue(20, 140);
            }
        }
        // Nuvem de noite (apenas uma, para efeito de loop)
        nuvem_noite_x -= nuvem_noite_speed * GetFrameTime();
        // Quando a primeira nuvem sair totalmente da tela, reposiciona para garantir junção fora da área visível
        if (nuvem_noite_x <= -background_noite3.width)
        {
            nuvem_noite_x += background_noite3.width;
        }

        // Atualiza scroll de background_noite2 (fatias)
        bn2_scroll += bn2_speed * GetFrameTime();
        if (bn2_scroll >= (float)background_noite2.width) bn2_scroll -= (float)background_noite2.width;

        // --- SEÇÃO DE DESENHO ÚNICA ---
        BeginDrawing();
            // Variáveis de cor com valores padrões
            Color cor_fundo = WHITE, cor_obstaculo = DARKGRAY, cor_texto = DARKGRAY, cor_chao = DARKGRAY;

            // Se estiver mostrando créditos, desenha a tela de créditos
            if (mostrar_creditos)
            {
                // define qual slide mostrar 
                int slide_index = (int)(timer_creditos / duracao_slide);
                if (slide_index < 0) slide_index = 0;
                if (slide_index >= num_slides) slide_index = num_slides - 1;

                // desenha slide (cobre toda a tela)
                Rectangle sourceRecBg = { 0, 0, (float)creditos_slides[slide_index].width, (float)creditos_slides[slide_index].height };
                Rectangle destRecBg = { 0, 0, (float)largura_tela, (float)altura_tela };
                Vector2 origin = { 0, 0 };
                ClearBackground(WHITE);
                DrawTexturePro(creditos_slides[slide_index], sourceRecBg, destRecBg, origin, 0.0f, WHITE);

                int altura_barra = 8;
                int y_barra = altura_tela - altura_barra - 5; // mais próximo da base

                // indicador de tempo (opcional)
                float progresso = timer_creditos / duracao_creditos;
                if (progresso > 1.0f) progresso = 1.0f;
                DrawRectangle(50, y_barra, (int)((largura_tela - 100) * progresso), altura_barra, GOLD);
            }
            else
            {
                // Determina cores e se desenha background
                if (subiu_nivel) {
                    cor_fundo = WHITE;
                    cor_texto = DARKGRAY;
                    cor_chao = DARKGRAY;
                    cor_obstaculo = DARKGRAY;
                }
                else if (nivel == 1) {
                    cor_fundo = WHITE;
                    cor_obstaculo = DARKGRAY;
                    cor_texto = WHITE;
                    cor_chao = DARKGRAY;
                } 
                else if (nivel == 2) {
                    cor_fundo = BLACK;
                    cor_obstaculo = WHITE;
                    cor_texto = LIGHTGRAY;
                    cor_chao = WHITE;
                } 
                else if (nivel == 0) {
                    cor_fundo = WHITE;
                    cor_texto = DARKGRAY;
                    cor_chao = DARKGRAY;
                    cor_obstaculo = DARKGRAY;
                }

                // Limpa o fundo depois de definir cor_fundo
                ClearBackground(cor_fundo);

                // Desenha backgrounds (se aplicável)
                if (!subiu_nivel) {
                    if (nivel == 1) {
                        Rectangle sourceRecBg = { 0, 0, (float)background.width, (float)background.height };
                        Rectangle destRecBg = { 0, 0, (float)largura_tela, (float)altura_tela };
                        Vector2 origin = { 0, 0 };
                        DrawTexturePro(background, sourceRecBg, destRecBg, origin, 0.0f, WHITE);
                        DrawTexturePro(background2, sourceRecBg, destRecBg, origin, 0.0f, WHITE);

                        // Desenha nuvens de dia (cada nuvem preenche a tela e é deslocada horizontalmente)
                        // desenha background4 estatico (camada fixa de nuvens)
                        {
                            Rectangle src4 = { 0.0f, 0.0f, (float)background4.width, (float)background4.height };
                            Rectangle dst4 = { 0.0f, 0.0f, (float)largura_tela, (float)altura_tela };
                            Vector2 origin = { 0, 0 };
                            DrawTexturePro(background4, src4, dst4, origin, 0.0f, WHITE);
                        }

                        // Desenha background3 em movimento (várias cópias que se deslocam horizontalmente)
                        for (int i = 0; i < NUM_NUVENS; i++)
                        {
                            Texture2D tex = background3;
                            Rectangle src = { 0.0f, 0.0f, (float)tex.width, (float)tex.height };
                            Rectangle dst = { nuvem_x[i], 0.0f, (float)largura_tela, (float)altura_tela };
                            Vector2 origin = { 0, 0 };
                            DrawTexturePro(tex, src, dst, origin, 0.0f, WHITE);
                        }
                    } else if (nivel == 2) {
                        Rectangle sourceRecBg = { 0, 0, (float)background_noite.width, (float)background_noite.height };
                        Rectangle destRecBg = { 0, 0, (float)largura_tela, (float)altura_tela };
                        Vector2 origin = { 0, 0 };
                        // Desenha background_noite2 usando duas fatias da mesma textura para criar scroll contínuo
                        {
                            float tex_w = (float)background_noite2.width;
                            float tex_h = (float)background_noite2.height;
                            float scroll = bn2_scroll; // 0..tex_w

                            // fatia da direita do texture (de scroll..tex_w)
                            Rectangle srcA = { scroll, 0.0f, tex_w - scroll, tex_h };
                            Rectangle dstA = { 0.0f, 0.0f, (tex_w - scroll) / tex_w * (float)largura_tela, (float)altura_tela };
                            Vector2 origin = { 0, 0 };
                            DrawTexturePro(background_noite, srcA, dstA, origin, 0.0f, WHITE);

                            // fatia da esquerda (0..scroll)
                            Rectangle srcB = { 0.0f, 0.0f, scroll, tex_h };
                            Rectangle dstB = { dstA.x + dstA.width, 0.0f, scroll / tex_w * (float)largura_tela, (float)altura_tela };
                            DrawTexturePro(background_noite, srcB, dstB, origin, 0.0f, WHITE);
                        }

                        // Desenha nuvem de noite (duas cópias, alinhadas pela base)
                        int nuvem_y = altura_tela - background_noite3.height;
                        // Desenha cópias suficientes para cobrir toda a tela
                        for (int x = (int)nuvem_noite_x; x < largura_tela; x += background_noite3.width) {
                            DrawTexture(background_noite3, x, nuvem_y, WHITE);
                        }
                        // Também desenha uma cópia extra à esquerda, caso a primeira esteja parcialmente fora
                        if (nuvem_noite_x > 0) {
                            DrawTexture(background_noite3, (int)(nuvem_noite_x - background_noite3.width), nuvem_y, WHITE);
                        }

                        //DrawTexturePro(background_noite2, sourceRecBg, destRecBg, origin, 0.0f, WHITE); // Descomente para ver a lua

                    }
                    // nivel 0: sem textura específica, só o cor_fundo já desenhado
                }

                if (!iniciou)
                {
                    DrawText("Pressione ESPAÇO para começar", largura_tela/2 - MeasureText("Pressione ESPAÇO para começar", 20)/2, altura_tela/2 - 10, 20, DARKGRAY);
                }
                else
                {
                    DrawLine(0, chao_y, largura_tela, chao_y, cor_chao);

                    // AJUSTE DE TAMANHO E POSIÇÃO

                    float escala_visual = 2.5f;
                    
                    Rectangle jogador_destino;
                    jogador_destino.width = jogador.width * escala_visual;
                    jogador_destino.height = jogador.height * escala_visual - 10.0f;
                    
                    jogador_destino.x = jogador.x - (jogador_destino.width - jogador.width) / 2.7;
                    jogador_destino.y = jogador.y + jogador.height - jogador_destino.height + 37.0; 


                    Texture2D textura_quadro_atual;
                    switch (quadro_atual)
                    {
                        case 0: textura_quadro_atual = quadro_jogador1; break;
                        case 1: textura_quadro_atual = quadro_jogador2; break;
                        case 2: textura_quadro_atual = quadro_jogador3; break;
                        default: textura_quadro_atual = quadro_jogador1; break;
                    }

                    DrawTexturePro(textura_quadro_atual, 
                                   (Rectangle){ 0, 0, (float)textura_quadro_atual.width, (float)textura_quadro_atual.height }, 
                                   jogador_destino, 
                                   (Vector2){0, 0}, 0.0f, WHITE);
                    
                    // Descomente para ver a caixa de colisão
                    // DrawRectangleLinesEx(jogador, 2, LIME);

                    for (int i = 0; i < MAX_OBSTACULOS; i++)
                    {
                        if (obstaculo_ativo[i]) DrawRectangleRec(obstaculos[i], cor_obstaculo);
                    }

                    DrawText(TextFormat("PONTOS: %04i", pontos), 20, 20, 20, cor_texto);
                    DrawText(TextFormat("HI: %04i", pontuacao_maxima), 20, 50, 20, cor_texto);
                    DrawText(TextFormat("NÍVEL: %d", nivel), largura_tela - 100, 20, 20, cor_texto);

                    if (subiu_nivel)
                    {
                        DrawText("NÍVEL 2", largura_tela / 2 - MeasureText("NÍVEL 2", 40) / 2, altura_tela / 2 - 60, 40, GOLD);
                        DrawText("Pressione ESPAÇO para continuar", largura_tela / 2 - MeasureText("Pressione ESPAÇO para continuar", 20) / 2, altura_tela / 2, 20, DARKGRAY);
                    }
                    else if (fim_de_jogo)
                    {
                        DrawText("GAME OVER", largura_tela / 2 - MeasureText("GAME OVER", 40) / 2, altura_tela / 2 - 40, 40, RED);
                        DrawText("Pressione ENTER para reiniciar", largura_tela / 2 - MeasureText("Pressione ENTER para reiniciar", 20) / 2, altura_tela / 2 + 10, 20, GRAY);
                    }
                }
            }
        EndDrawing();
    }

    // Descarrega tudo
    UnloadTexture(quadro_jogador1);
    UnloadTexture(quadro_jogador2);
    UnloadTexture(quadro_jogador3);
    // Descarregar backgrounds do jogo

    UnloadTexture(background);
    UnloadTexture(background2);
    UnloadTexture(background3);
    UnloadTexture(background4);
    UnloadTexture(background_noite);
    UnloadTexture(background_noite2);
    UnloadTexture(background_noite3);

    // Descarregar texturas de créditos (novas)

    UnloadTexture(credito1);
    UnloadTexture(credito2);
    UnloadTexture(credito3);
    UnloadTexture(credito4);
    UnloadTexture(credito5);
    UnloadTexture(credito6);
    UnloadTexture(credito7);
    UnloadTexture(credito8);
    UnloadTexture(credito9);
    UnloadTexture(credito10);
    UnloadTexture(credito11);
    UnloadTexture(credito12);

    // Parar e descarregar músicas (opcional, mas recomendado)
    StopMusicStream(musica_nivel1);
    StopMusicStream(musica_nivel2);
    UnloadMusicStream(musica_nivel1);
    UnloadMusicStream(musica_nivel2);

    UnloadSound(som_salto);
    UnloadSound(som_pontuacao);
    UnloadSound(som_subiu_de_nivel);
    UnloadSound(som_game_over);
    CloseAudioDevice();

    CloseWindow();
    return 0;
}
