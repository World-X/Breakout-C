//==============================> Librerías

#include "raylib.h"
#include "raymath.h"
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

//===============> General

float CalcularMovimiento(float velocidad, float aceleracion)
{
    return velocidad * GetFrameTime() * aceleracion;
}

//===============> Pelota

void CambiarDireccionPelota(Pelota *pelota, Direccion direccion)
{
    if (direccion == (Direccion)HORIZONTAL)
    {
        pelota->velocidad.x *= -1.0f;
    }
    else if (direccion == (Direccion)VERTICAL)
    {
        pelota->velocidad.y *= -1.0f;
    }
}

void CambiarDireccionColisionRectPelota(Pelota *pelota, Rectangle rectangulo)
{
    Vector2 pelotaVirtualPosicion = {pelota->posicion.x + CalcularMovimiento(pelota->velocidad.x, pelota->aceleracion.x), pelota->posicion.y + CalcularMovimiento(pelota->velocidad.y, pelota->aceleracion.y)};
    bool veniaDesdeArriba = pelota->posicion.y < rectangulo.y;
    bool veniaDesdeAbajo = pelota->posicion.y > rectangulo.y + rectangulo.height;
    bool veniaDesdeIzquierda = pelota->posicion.x < rectangulo.x;
    bool veniaDesdeDerecha = pelota->posicion.x > rectangulo.x + rectangulo.width;
    bool entroPorArriba = pelotaVirtualPosicion.y < rectangulo.y;
    bool entroPorAbajo = pelotaVirtualPosicion.y > rectangulo.y + rectangulo.height;
    bool entroPorLaIzquierda = pelotaVirtualPosicion.x < rectangulo.x;
    bool entroPorLaDerecha = pelotaVirtualPosicion.x > rectangulo.x + rectangulo.width;
    if (veniaDesdeArriba && entroPorArriba || veniaDesdeAbajo && entroPorAbajo)
    {
        CambiarDireccionPelota(pelota, (Direccion)VERTICAL);
        if (CheckCollisionCircleRec(pelota->posicion, pelota->radio, rectangulo))
        {
            for (int i = 0; i < 4; i++)
            {
                if (CheckCollisionCircleRec(pelota->posicion, pelota->radio, (Rectangle){rectangulo.x, rectangulo.y + i, rectangulo.width, 1}))
                {
                    pelota->posicion.y = rectangulo.y + i - pelota->radio;
                    break;
                }
                if (CheckCollisionCircleRec(pelota->posicion, pelota->radio, (Rectangle){rectangulo.x, rectangulo.y + rectangulo.height - i, rectangulo.width, 1}))
                {
                    pelota->posicion.y = rectangulo.y + rectangulo.height - i + pelota->radio;
                    break;
                }
            }
        }
    }
    if (veniaDesdeIzquierda && entroPorLaIzquierda || veniaDesdeDerecha && entroPorLaDerecha)
    {
        CambiarDireccionPelota(pelota, (Direccion)HORIZONTAL);
        if (CheckCollisionCircleRec(pelota->posicion, pelota->radio, rectangulo))
        {
            for (int i = 0; i < 4; i++)
            {
                if (CheckCollisionCircleRec(pelota->posicion, pelota->radio, (Rectangle){rectangulo.x + i, rectangulo.y, 1, rectangulo.height}))
                {
                    pelota->posicion.x = rectangulo.x + i - pelota->radio;
                    break;
                }
                if (CheckCollisionCircleRec(pelota->posicion, pelota->radio, (Rectangle){rectangulo.x + rectangulo.width - i, rectangulo.y, 1, rectangulo.height}))
                {
                    pelota->posicion.x = rectangulo.x + rectangulo.width - i + pelota->radio;
                    break;
                }
            }
        }
    }
}

void MoverPelota(Pelota *pelota, Direccion direccion)
{
    if (direccion == (Direccion)HORIZONTAL)
    {
        pelota->posicion.x += CalcularMovimiento(pelota->velocidad.x, pelota->aceleracion.x);
    }
    else if (direccion == (Direccion)VERTICAL)
    {
        pelota->posicion.y += CalcularMovimiento(pelota->velocidad.y, pelota->aceleracion.y);
    }
}

void ActualizarPelota(Pelota *pelota)
{
    MoverPelota(pelota, (Direccion)HORIZONTAL);
    MoverPelota(pelota, (Direccion)VERTICAL);
}

void DibujarPelota(Pelota *pelota)
{
    DrawCircleV(pelota->posicion, pelota->radio, pelota->color);
}

//===============> Paleta

void ActualizarPaleta(Paleta *paleta)
{
    if (IsKeyDown(KEY_LEFT))
    {
        paleta->posicion.x -= CalcularMovimiento(paleta->velocidad, 1.0f);
    }
    if (IsKeyDown(KEY_RIGHT))
    {
        paleta->posicion.x += CalcularMovimiento(paleta->velocidad, 1.0f);
    }
    _LimitarMovimientoPaleta(paleta);
}

void DibujarPaleta(Paleta *paleta)
{
    DrawRectangleRounded((Rectangle){paleta->posicion.x, paleta->posicion.y, paleta->tamaño.x, paleta->tamaño.y}, paleta->redondez, paleta->segmentos, paleta->color);
}

void _LimitarMovimientoPaleta(Paleta *paleta)
{
    if (paleta->posicion.x < 0.0f)
    {
        paleta->posicion.x = 0.0f;
    }
    if (paleta->posicion.x > juegoAncho - paleta->tamaño.x)
    {
        paleta->posicion.x = juegoAncho - paleta->tamaño.x;
    }
}

Rectangle ObtenerRectanguloPaleta(Paleta *paleta)
{
    return (Rectangle){paleta->posicion.x, paleta->posicion.y, paleta->tamaño.x, paleta->tamaño.y};
}

//==========> CPUPaleta

void ActualizarCPUPaleta(Paleta *cpu_paleta, Vector2 posicion_pelota)
{
    if (cpu_paleta->posicion.x + cpu_paleta->tamaño.x / 2 < posicion_pelota.x)
    {
        cpu_paleta->posicion.x += CalcularMovimiento(cpu_paleta->velocidad, 1.0f);
    }
    if (cpu_paleta->posicion.x + cpu_paleta->tamaño.x / 2 > posicion_pelota.x)
    {
        cpu_paleta->posicion.x -= CalcularMovimiento(cpu_paleta->velocidad, 1.0f);
    }
    _LimitarMovimientoPaleta(cpu_paleta);
}

//===============> Ladrillo

void DibujarLadrillo(Ladrillo *ladrillo)
{
    if (ladrillo->activo)
    {
        DrawRectangleRounded((Rectangle){ladrillo->posicion.x, ladrillo->posicion.y, ladrillo->tamaño.x, ladrillo->tamaño.y}, ladrillo->redondez, ladrillo->segmentos, ladrillo->color);
    }
}

Rectangle ObtenerRectanguloLadrillo(Ladrillo *ladrillo)
{
    return (Rectangle){ladrillo->posicion.x, ladrillo->posicion.y, ladrillo->tamaño.x, ladrillo->tamaño.y};
}

//===============> Obstaculo

void MoverObstaculo(Obstaculo *obstaculo, Vector2 inicio, Vector2 fin, float interpolacion)
{
    obstaculo->posicion = Vector2Lerp(inicio, fin, interpolacion);
}

void ActualizarObstaculo(Obstaculo *obstaculo)
{
    obstaculo->modulo = MAX(0.0f, obstaculo->modulo - 2.0f * GetFrameTime());
}

void DibujarObstaculo(Obstaculo *obstaculo)
{
    DrawRectangle(obstaculo->posicion.x, obstaculo->posicion.y, obstaculo->tamaño.x, obstaculo->tamaño.y, ColorLerp(obstaculo->color_principal, WHITE, obstaculo->modulo));
    DrawRectangleLinesEx((Rectangle){obstaculo->posicion.x, obstaculo->posicion.y, obstaculo->tamaño.x, obstaculo->tamaño.y}, obstaculo->grosor, obstaculo->color_contorno);
}

Rectangle ObtenerRectanguloObstaculo(Obstaculo *obstaculo)
{
    return (Rectangle){obstaculo->posicion.x, obstaculo->posicion.y, obstaculo->tamaño.x, obstaculo->tamaño.y};
}
