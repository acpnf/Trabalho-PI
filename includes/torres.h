#ifndef TORRES_H
#define TORRES_H

#include <raylib.h>
#include <stdbool.h>

// --- Constantes ---
#define CUSTO_SOLDADO 10
#define CUSTO_ARQUEIRO 20
#define CUSTO_MAGO 40

// --- Estruturas ---
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

// --- Funções Públicas ---
void iniciar_torres(void);
void liberar_torres(void);

Soldado criar_soldado(Vector2 posicao, float largura_mundo);
Arqueiro criar_arqueiro(Vector2 posicao, float largura_mundo);
Mago criar_mago(Vector2 posicao, float largura_mundo);

void desenhar_soldado(Soldado soldado);
void desenhar_arqueiro(Arqueiro arqueiro);
void desenhar_mago(Mago mago);

#endif
 
