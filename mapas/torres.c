#include "torres.h"
#include <math.h>
#include "raylib.h"

static Texture2D sprite_soldado;
static Texture2D sprite_arqueiro;
static Texture2D sprite_mago;

void iniciar_torres(){
    sprite_soldado = LoadTexture("inimigo.png");
    sprite_arqueiro = LoadTexture("arqueiro.png");
    sprite_mago = LoadTexture("mago.png");
}

Soldado criar_soldado(Vector2 posicao, float alcance) {
    return (Soldado){
        .posicao = posicao,
        .dano = 80,
        .alcance = alcance,
        .cooldown = 1.0f,
        .custo = 10,
        .ativo = true
    };
}

Arqueiro criar_arqueiro(Vector2 posicao, float alcance) {
    return (Arqueiro){
        .posicao = posicao,
        .dano = 50,
        .alcance = alcance,
        .cooldown = 2.0f,
        .custo = 20,
        .ativo = true
    };
}

Mago criar_mago(Vector2 posicao, float alcance) {
    return (Mago){
        .posicao = posicao,
        .dano = 120,
        .alcance = alcance,
        .cooldown = 3.0f,
        .custo = 40,
        .ativo = true
    };
}

void desenhar_soldado(Soldado soldado, float width, float height) {
    if (!soldado.ativo) return;
    Rectangle dest = {
        soldado.posicao.x,  // centro do tile
        soldado.posicao.y,
        width,
        height
    };
    DrawTexturePro(
        sprite_soldado,
        (Rectangle){0, 0, sprite_soldado.width, sprite_soldado.height},
        dest,
        (Vector2){width / 2, height / 2},  // centraliza
        0,
        WHITE
    );
}

void desenhar_arqueiro(Arqueiro arqueiro, float width, float height) {
    if (!arqueiro.ativo) return;
    Rectangle dest = {
        arqueiro.posicao.x,
        arqueiro.posicao.y,
        width,
        height
    };
    DrawTexturePro(
        sprite_arqueiro,
        (Rectangle){0, 0, sprite_arqueiro.width, sprite_arqueiro.height},
        dest,
        (Vector2){width / 2, height / 2},  // centraliza
        0,
        WHITE
    );
}


void desenhar_mago(Mago mago, float width, float height) {
    if (!mago.ativo) return;
    Rectangle dest = {
        mago.posicao.x,
        mago.posicao.y,
        width,
        height
    };
    DrawTexturePro(
        sprite_mago,
        (Rectangle){0, 0, sprite_mago.width, sprite_mago.height},
        dest,
        (Vector2){width / 2, height / 2},  // centraliza
        0,
        WHITE
    );
}


void liberar_torres() {
    UnloadTexture(sprite_soldado);
    UnloadTexture(sprite_arqueiro);
    UnloadTexture(sprite_mago);
}
