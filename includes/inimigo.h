#ifndef INIMIGOS_H
#define INIMIGOS_H

typedef struct {
    Vector2 posicao;
    int dano;
    float alcance;
    float cooldown;
    int custo;
    bool ativo;
    int velocidade;
} Inimigo;





#endif // INIMIGOS_H
