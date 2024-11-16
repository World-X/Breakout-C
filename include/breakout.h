//==============================> Seguridad

#ifndef BREAKOUT_H
#define BREAKOUT_H

//==============================> Clases

class Pelota
{
public:
    Vector2 posicion;
    Vector2 velocidad;
    float radio;
    Color color;

    Pelota(void);
    Pelota(Vector2 _posicion, Vector2 _velocidad, float _radio, Color _color);
    void AcabaDeColisionar(bool _horizontal);
    void Actualizar();
    void Dibujar();
};

class Paleta
{
public:
    Vector2 posicion;
    Vector2 tamaño;
    float velocidad;
    int segmentos;
    float redondez;
    Color color;

    Paleta(void);
    Paleta(Vector2 _posicion, Vector2 _tamaño, float _velocidad, int _segmentos, float _redondez, Color _color);
    Rectangle ObtenerRectangulo();
    void Actualizar();
    void Dibujar();

protected:
    void LimitarMovimiento();
};

class CPUPaleta : public Paleta
{
public:
    CPUPaleta(void);
    CPUPaleta(Vector2 _posicion, Vector2 _tamaño, float _velocidad, int _segmentos, float _redondez, Color _color);
    void Actualizar(Vector2 _pelotaPosicion);
};

class Ladrillo
{
public:
    Vector2 posicion;
    Vector2 tamaño;
    float redondez;
    int segmentos;
    Color color;
    bool activo;
    int puntos;

    Ladrillo(void);
    Ladrillo(Vector2 _posicion, Vector2 _tamaño, float _redondez, int _segmentos, Color _color, bool _activo, int _puntos);
    Rectangle ObtenerRectangulo();
    void Dibujar();
};

class Obstaculo
{
public:
    Vector2 posicion;
    Vector2 tamaño;
    Color color_principal;
    Color color_contorno;
    float grosor;

    Obstaculo(void);
    Obstaculo(Vector2 _posicion, Vector2 _tamaño, Color _color_principal, Color _color_contorno, float _grosor);
    Rectangle ObtenerRectangulo();
    void Dibujar();
};

//==============================> Fin

#endif