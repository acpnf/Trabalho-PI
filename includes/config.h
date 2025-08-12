#ifndef JOGO_H
#define JOGO_H

#include <raylib.h>
//textura do inimigo
extern Texture2D inimigoSprite;
//
extern Vector2 posDano;
//conrole de movimento
extern double lastMoveTime;
extern double moveInterval;

extern bool mostrarDano;

extern int vida;
extern int moedas;

extern double tempoMostrarDano;

#endif // JOGO_H
