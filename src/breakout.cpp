//==============================> Librerías

#include "raylib.h"
#include "breakout.h"

//==============================> Macros

#define MAX(a, b) ((a) > (b) ? (a) : (b))
#define MIN(a, b) ((a) < (b) ? (a) : (b))

//==============================> Variables globales externas

extern int juegoAncho;
extern int juegoAlto;
extern int mitadJuegoAncho;
extern int mitadJuegoAlto;

//==============================> Definiciones de funciones

Pelota::Pelota(Vector2 _posicion, Vector2 _velocidad, float _radio, Color _color)
{
    posicion = _posicion;
    velocidad = _velocidad;
    radio = _radio;
    color = _color;
}

void Pelota::Actualizar()
{
    posicion.x += velocidad.x * GetFrameTime();
    posicion.y += velocidad.y * GetFrameTime();
    if (posicion.x - radio <= 0.0f || posicion.x + radio >= juegoAncho)
    {
        velocidad.x *= -1.0f;
        posicion.x += velocidad.x * GetFrameTime();
    }
    if (posicion.y - radio <= 0.0f || posicion.y + radio >= juegoAlto)
    {
        velocidad.y *= -1.0f;
        posicion.y += velocidad.y * GetFrameTime();
    }
}

void Pelota::Dibujar()
{
    DrawCircleV(posicion, radio, color);
}
