//==============================> Seguridad

#ifndef BREAKOUT_H
#define BREAKOUT_H

//==============================> Macros

#define VECTOR2_CERO \
    (Vector2) { 0.0f, 0.0f }
#define VECTOR2_UNO \
    (Vector2) { 1.0f, 1.0f }

//==============================> Enumeraciones

typedef enum Direccion
{
    HORIZONTAL = 0x01,
    VERTICAL = 0x02
} Direccion;

//==============================> Estructuras

typedef struct Pelota
{
    Vector2 posicion;
    Vector2 velocidad;
    Vector2 aceleracion;
    float radio;
    Color color;
} Pelota;

typedef struct Paleta
{
    Vector2 posicion;
    Vector2 tamaño;
    float velocidad;
    int segmentos;
    float redondez;
    Color color;
} Paleta;

typedef struct Ladrillo
{
    Vector2 posicion;
    Vector2 tamaño;
    float redondez;
    int segmentos;
    Color color;
    bool activo;
    int puntos;
} Ladrillo;

typedef struct Obstaculo
{
    Vector2 posicion;
    Vector2 tamaño;
    Color color_principal;
    Color color_contorno;
    float grosor;
    float modulo;
} Obstaculo;

//==============================> Prototipos de funciones

//===============> General

float CalcularMovimiento(float velocidad, float aceleracion);

//===============> Paleta

void CambiarDireccionPelota(Pelota *pelota, Direccion direccion);
void CambiarDireccionColisionRectPelota(Pelota *pelota, Rectangle rectangulo);
void MoverPelota(Pelota *pelota, Direccion direccion);
void ActualizarPelota(Pelota *pelota);
void DibujarPelota(Pelota *pelota);

//===============> Paleta

void ActualizarPaleta(Paleta *paleta);
void DibujarPaleta(Paleta *paleta);
void _LimitarMovimientoPaleta(Paleta *paleta);
Rectangle ObtenerRectanguloPaleta(Paleta *paleta);

//==========> CPUPaleta

void ActualizarCPUPaleta(Paleta *cpu_paleta, Vector2 posicion_pelota);

//===============> Ladrillo

void DibujarLadrillo(Ladrillo *ladrillo);
Rectangle ObtenerRectanguloLadrillo(Ladrillo *ladrillo);

//===============> Obstaculo

void DibujarObstaculo(Obstaculo *obstaculo);
Rectangle ObtenerRectanguloObstaculo(Obstaculo *obstaculo);

//==============================> Fin

#endif