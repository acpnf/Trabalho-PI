#ifndef TORRES_H
#define TORRES_H

#include <raylib.h>
#include <stdbool.h>

// --- Constantes ---
#define CUSTO_SOLDADO 10
#define CUSTO_ARQUEIRO 20
#define CUSTO_MAGO 40
#define MAX_TORRES 100

// --- Estruturas ---
typedef struct {
    Vector2 posicao;
    int dano;
    float alcance;
    float cooldown;
    int custo;
    bool ativo;
    double tempoFimTiro;
    double tempoUltimoTiro;
} Soldado;

typedef struct {
    Vector2 posicao;
    int dano;
    float alcance;
    float cooldown;
    int custo;
    bool ativo;
    double tempoFimTiro;
    double tempoUltimoTiro;
} Arqueiro;

typedef struct {
    Vector2 posicao;
    int dano;
    float alcance;
    float cooldown;
    int custo;
    bool ativo;
    double tempoFimTiro;
    double tempoUltimoTiro;
} Mago;

// ---Torres---
extern Soldado soldados[MAX_TORRES];
extern Arqueiro arqueiros[MAX_TORRES];
extern Mago magos[MAX_TORRES];
extern int num_soldados;
extern int num_arqueiros;
extern int num_magos;

// --- Funções Públicas ---
void iniciar_torres(void);
void liberar_torres(void);

Soldado criar_soldado(Vector2 posicao, float alcance);
Arqueiro criar_arqueiro(Vector2 posicao, float alcance);
Mago criar_mago(Vector2 posicao, float alcance);

void desenhar_soldado(Soldado soldado, float width, float height);
void desenhar_arqueiro(Arqueiro arqueiro, float width, float height);
void desenhar_mago(Mago mago, float width, float height);

#endif
