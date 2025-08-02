#ifndef TORRES_H
#define TORRES_H

#include <raylib.h>


#define TAMANHO_TELA 1000  //resolução
#define ALCANCE_SOLDADO 0.5f * TAMANHO_TELA
#define ALCANCE_ARQUEIRO 3.5f * 64  // 3.5 tiles (assumindo TILE_SIZE=64)
#define ALCANCE_MAGO 2.0f * 64


typedef struct {
    Vector2 posicao;
    int dano;
    float alcance;
    float cooldown;
    int custo;
    bool ativo;
} Soldado;

typedef struct {
    Vector2 posicao;
    int dano;
    float alcance;
    float cooldown;
    int custo;
    bool ativo;
} Arqueiro;

typedef struct {
    Vector2 posicao;
    int dano;
    float alcance;
    float cooldown;
    int custo;
    bool ativo;
} Mago;


static inline Soldado criarSoldado(Vector2 pos) {
    
}

static inline Arqueiro criarArqueiro(Vector2 pos) {
  
}

static inline Mago criarMago(Vector2 pos) {
}

#endif
