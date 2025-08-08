#include "torres.h"
#include <math.h>
#include "raylib.h"

static Texture2D sprite_soldado;
static Texture2D sprite_arqueiro;
static Texture2D sprite_mago;

void iniciar_torres(){
    sprite_mago = LoadTexture("personagens/torres/pngsTeste/tower_4_transparent-removebg-preview.png");
    sprite_arqueiro = LoadTexture("personagens/torres/pngsTeste/tower_5_transparent-removebg-preview.png");
    sprite_soldado = LoadTexture("personagens/torres/pngsTeste/tower_6_transparent-removebg-preview.png");
}

Soldado criar_soldado(Vector2 posicao, float alcance) {
    Soldado s;
    s.posicao = posicao;
    s.dano = 10;
    s.alcance = alcance;
    s.cooldown = 0.5f;
    s.custo = CUSTO_SOLDADO;
    s.ativo = true;
    return s;
}

// Criar Arqueiro
Arqueiro criar_arqueiro(Vector2 posicao, float alcance) {
    Arqueiro a;
    a.posicao = posicao;
    a.dano = 15;
    a.alcance = alcance;
    a.cooldown = 1.0f;
    a.custo = CUSTO_ARQUEIRO;
    a.ativo = true;
    return a;
}

// Criar Mago
Mago criar_mago(Vector2 posicao, float alcance) {
    Mago m;
    m.posicao = posicao;
    m.dano = 25;
    m.alcance = alcance;
    m.cooldown = 2.0f;
    m.custo = CUSTO_MAGO;
    m.ativo = true;
    return m;
}

// Função auxiliar para desenhar textura mantendo proporção e encaixando dentro de maxSize x maxSize
static void desenhar_textura_proporcional(Texture2D sprite, Vector2 pos, float maxSize) {
    float scale;
    if (sprite.width > sprite.height)
        scale = maxSize / (float)sprite.width;
    else
        scale = maxSize / (float)sprite.height;

    float width = sprite.width * scale;
    float height = sprite.height * scale;

    DrawTexturePro(sprite,
                   (Rectangle){0, 0, sprite.width, sprite.height},
                   (Rectangle){pos.x - width / 2, pos.y - height / 2, width, height},
                   (Vector2){0, 0},  // origem do desenho no canto superior esquerdo do retângulo destino
                   0, WHITE);
}

void desenhar_soldado(Soldado soldado, float width, float height) {
    if (!soldado.ativo) return;
    desenhar_textura_proporcional(sprite_soldado, soldado.posicao, width);
}

void desenhar_arqueiro(Arqueiro arqueiro, float width, float height) {
    if (!arqueiro.ativo) return;
    desenhar_textura_proporcional(sprite_arqueiro, arqueiro.posicao, width);
}

void desenhar_mago(Mago mago, float width, float height) {
    if (!mago.ativo) return;
    desenhar_textura_proporcional(sprite_mago, mago.posicao, width);
}


void liberar_torres() {
    UnloadTexture(sprite_soldado);
    UnloadTexture(sprite_arqueiro);
    UnloadTexture(sprite_mago);
}
