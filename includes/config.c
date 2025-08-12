#include "config.h"
#include <stdio.h>
#include <raylib.h>

Texture2D inimigoSprite;
Vector2 posDano;

double lastMoveTime = 0.0;
double moveInterval = 1.5; 

bool mostrarDano = false;

int vida = 150;
int moedas = 100;

double tempoMostrarDano = 0.0;