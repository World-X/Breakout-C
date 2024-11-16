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

//===============> Pelota

Pelota::Pelota(void)
{
    posicion = {0};
    velocidad = {0};
    radio = 0.0f;
    color = {0};
}

Pelota::Pelota(Vector2 _posicion, Vector2 _velocidad, float _radio, Color _color)
{
    posicion = _posicion;
    velocidad = _velocidad;
    radio = _radio;
    color = _color;
}

void Pelota::AcabaDeColisionar(bool _horizontal)
{
    if (_horizontal)
    {
        velocidad.x *= -1.0f;
        posicion.x += velocidad.x * GetFrameTime();
    }
    else
    {
        velocidad.y *= -1.0f;
        posicion.y += velocidad.y * GetFrameTime();
    }
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

//===============> Paleta

Paleta::Paleta(void)
{
    posicion = {0};
    tamaño = {0};
    velocidad = 0.0f;
    segmentos = 0;
    redondez = 0.0f;
    color = {0};
}

Paleta::Paleta(Vector2 _posicion, Vector2 _tamaño, float _velocidad, int _segmentos, float _redondez, Color _color)
{
    posicion = _posicion;
    tamaño = _tamaño;
    velocidad = _velocidad;
    segmentos = _segmentos;
    redondez = _redondez;
    color = _color;
}

Rectangle Paleta::ObtenerRectangulo()
{
    return (Rectangle){posicion.x, posicion.y, tamaño.x, tamaño.y};
}

void Paleta::Actualizar(void)
{
    if (IsKeyDown(KEY_LEFT))
    {
        posicion.x -= velocidad * GetFrameTime();
    }
    if (IsKeyDown(KEY_RIGHT))
    {
        posicion.x += velocidad * GetFrameTime();
    }
    LimitarMovimiento();
}

void Paleta::Dibujar()
{
    DrawRectangleRounded((Rectangle){posicion.x, posicion.y, tamaño.x, tamaño.y}, redondez, segmentos, color);
}

void Paleta::LimitarMovimiento()
{
    if (posicion.x < 0.0f)
    {
        posicion.x = 0.0f;
    }
    if (posicion.x > juegoAncho - tamaño.x)
    {
        posicion.x = juegoAncho - tamaño.x;
    }
}

//===============> CPUPaleta : Paleta

CPUPaleta::CPUPaleta(void)
{
}

CPUPaleta::CPUPaleta(Vector2 _posicion, Vector2 _tamaño, float _velocidad, int _segmentos, float _redondez, Color _color) : Paleta(_posicion, _tamaño, _velocidad, _segmentos, _redondez, _color)
{
}

void CPUPaleta::Actualizar(Vector2 _pelotaPosicion)
{
    if (posicion.x + tamaño.x / 2 < _pelotaPosicion.x)
    {
        posicion.x += velocidad * GetFrameTime();
    }
    if (posicion.x + tamaño.x / 2 > _pelotaPosicion.x)
    {
        posicion.x -= velocidad * GetFrameTime();
    }
    LimitarMovimiento();
}

//===============> Ladrillo

Ladrillo::Ladrillo(void)
{
    posicion = {0};
    tamaño = {0};
    redondez = 0.0f;
    segmentos = 0;
    color = {0};
    activo = false;
    puntos = 0;
}

Ladrillo::Ladrillo(Vector2 _posicion, Vector2 _tamaño, float _redondez, int _segmentos, Color _color, bool _activo, int _puntos)
{
    posicion = _posicion;
    tamaño = _tamaño;
    redondez = _redondez;
    segmentos = _segmentos;
    color = _color;
    activo = _activo;
    puntos = _puntos;
}

Rectangle Ladrillo::ObtenerRectangulo()
{
    return (Rectangle){posicion.x, posicion.y, tamaño.x, tamaño.y};
}

void Ladrillo::Dibujar()
{
    if (activo)
    {
        DrawRectangleRounded((Rectangle){posicion.x, posicion.y, tamaño.x, tamaño.y}, redondez, segmentos, color);
    }
}

//===============> Obstaculo

Obstaculo::Obstaculo(void)
{
    posicion = {0};
    tamaño = {0};
    color_principal = {0};
    color_contorno = {0};
    grosor = 0.0f;
}

Obstaculo::Obstaculo(Vector2 _posicion, Vector2 _tamaño, Color _color_principal, Color _color_contorno, float _grosor)
{
    posicion = _posicion;
    tamaño = _tamaño;
    color_principal = _color_principal;
    color_contorno = _color_contorno;
    grosor = _grosor;
}

Rectangle Obstaculo::ObtenerRectangulo()
{
    return (Rectangle){posicion.x, posicion.y, tamaño.x, tamaño.y};
}

void Obstaculo::Dibujar()
{
    DrawRectangle(posicion.x, posicion.y, tamaño.x, tamaño.y, color_principal);
    DrawRectangleLinesEx((Rectangle){posicion.x, posicion.y, tamaño.x, tamaño.y}, grosor, color_contorno);
}
