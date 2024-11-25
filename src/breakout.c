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

//===============> General

float CalcularMovimiento(float velocidad)
{
    return velocidad * GetFrameTime();
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
    Vector2 pelotaVirtualPosicion = {pelota->posicion.x + pelota->velocidad.x * GetFrameTime(), pelota->posicion.y + pelota->velocidad.y * GetFrameTime()};
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
        MoverPelota(pelota, (Direccion)VERTICAL);
    }
    if (veniaDesdeIzquierda && entroPorLaIzquierda || veniaDesdeDerecha && entroPorLaDerecha)
    {
        CambiarDireccionPelota(pelota, (Direccion)HORIZONTAL);
        MoverPelota(pelota, (Direccion)HORIZONTAL);
    }
}

void MoverPelota(Pelota *pelota, Direccion direccion)
{
    if (direccion == (Direccion)HORIZONTAL)
    {
        pelota->posicion.x += CalcularMovimiento(pelota->velocidad.x);
    }
    else if (direccion == (Direccion)VERTICAL)
    {
        pelota->posicion.y += CalcularMovimiento(pelota->velocidad.y);
    }
}

void ActualizarPelota(Pelota *pelota)
{
    MoverPelota(pelota, (Direccion)HORIZONTAL);
    MoverPelota(pelota, (Direccion)VERTICAL);
    if (pelota->posicion.x - pelota->radio <= 0.0f || pelota->posicion.x + pelota->radio >= juegoAncho)
    {
        CambiarDireccionPelota(pelota, (Direccion)HORIZONTAL);
        MoverPelota(pelota, (Direccion)HORIZONTAL);
    }
    if (pelota->posicion.y - pelota->radio <= 0.0f || pelota->posicion.y + pelota->radio >= juegoAlto)
    {
        CambiarDireccionPelota(pelota, (Direccion)VERTICAL);
        MoverPelota(pelota, (Direccion)VERTICAL);
    }
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
        paleta->posicion.x -= CalcularMovimiento(paleta->velocidad);
    }
    if (IsKeyDown(KEY_RIGHT))
    {
        paleta->posicion.x += CalcularMovimiento(paleta->velocidad);
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
        cpu_paleta->posicion.x += CalcularMovimiento(cpu_paleta->velocidad);
    }
    if (cpu_paleta->posicion.x + cpu_paleta->tamaño.x / 2 > posicion_pelota.x)
    {
        cpu_paleta->posicion.x -= CalcularMovimiento(cpu_paleta->velocidad);
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

void DibujarObstaculo(Obstaculo *obstaculo)
{
    DrawRectangle(obstaculo->posicion.x, obstaculo->posicion.y, obstaculo->tamaño.x, obstaculo->tamaño.y, obstaculo->color_principal);
    DrawRectangleLinesEx((Rectangle){obstaculo->posicion.x, obstaculo->posicion.y, obstaculo->tamaño.x, obstaculo->tamaño.y}, obstaculo->grosor, obstaculo->color_contorno);
}

Rectangle ObtenerRectanguloObstaculo(Obstaculo *obstaculo)
{
    return (Rectangle){obstaculo->posicion.x, obstaculo->posicion.y, obstaculo->tamaño.x, obstaculo->tamaño.y};
}