#ifndef NIVEL_2_H
#define NIVEL_2_H

#include "raylib.h"
#include <string.h>

typedef struct {
    int mapa[10][20];          // Mapa do nível
    int vida_jogador;          // Vida inicial
    int moedas_iniciais;       // Moedas iniciais
    Texture2D textura_caminho; // Textura do caminho
    Texture2D textura_grama;   // Textura da grama
} Nivel2;

Nivel2 IniciarNivel2();

#endif
