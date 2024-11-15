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
    Pelota(Vector2 _posicion, Vector2 _velocidad, float _radio, Color _color);
    void Actualizar();
    void Dibujar();
};

//==============================> Fin

#endif