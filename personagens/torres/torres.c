#include "torres.h"
#include <math.h>



static Texture2D sprite_soldado;
static Texture2D sprite_arqueiro;
static Texture2D sprite_mago;



void iniciar_torres(){
    sprite_soldado = LoadTexture("inimigo.png");
    sprite_arqueiro = LoadTexture("arqueiro.png");
    sprite_mago = LoadTexture("mago.png"); //apenas enquanto n tiver o sprite fixo

    
}


Soldado criar_soldado(Vector2 posicao, float largura) { //inicia cada personagem e seus atributos
    return (Soldado){
        .posicao = posicao,
        .dano = 80,
        .alcance = 0.5f * largura,
        .cooldown = 1.0f,
        .custo = 10,
        .ativo = true
    };
}

Arqueiro criar_arqueiro(Vector2 posicao, float largura) {
    return (Arqueiro){
        .posicao = posicao,
        .dano = 50,
        .alcance = 1.5f * largura,
        .cooldown = 2.0f,
        .custo = 20,
        .ativo = true
    };
}

Mago criar_mago(Vector2 posicao, float largura) {
    return (Mago){
        .posicao = posicao,
        .dano = 120,
        .alcance = 2.5f * largura,
        .cooldown = 3.0f,
        .custo = 40,
        .ativo = true
    };
}

void desenhar_soldado(Soldado soldado) {
    if (!soldado.ativo) return;
    Rectangle src = {0, 0, sprite_soldado.width, sprite_soldado.height};
    Rectangle dest = {soldado.posicao.x, soldado.posicao.y, sprite_soldado.width, sprite_soldado.height};
    DrawTexturePro(sprite_soldado, src, dest, (Vector2){0}, 0, WHITE);
}

void desenhar_arqueiro(Arqueiro arqueiro) {
    if (!arqueiro.ativo) return;
    Rectangle src = {0, 0, sprite_arqueiro.width, sprite_arqueiro.height};
    Rectangle dest = {arqueiro.posicao.x, arqueiro.posicao.y, sprite_arqueiro.width, sprite_arqueiro.height};
    DrawTexturePro(sprite_arqueiro, src, dest, (Vector2){0}, 0, WHITE);
}

void desenhar_mago(Mago mago) {
    if (!mago.ativo) return;
    Rectangle src = {0, 0, sprite_mago.width, sprite_mago.height};
    Rectangle dest = {mago.posicao.x, mago.posicao.y, sprite_mago.width, sprite_mago.height};
    DrawTexturePro(sprite_arqueiro, src, dest, (Vector2){0}, 0, WHITE);
}


void liberar_torres() {
    UnloadTexture(sprite_soldado);
    UnloadTexture(sprite_arqueiro);
    UnloadTexture(sprite_mago);
}
