//==============================> Librerías

#include "raylib.h"

//==============================> Función principal

int main()
{
    //===============> Inicialización

    SetConfigFlags(FLAG_WINDOW_RESIZABLE | FLAG_VSYNC_HINT);
    int ventanaAncho = 1280;
    int ventanaAlto = 720;
    InitWindow(ventanaAncho, ventanaAlto, "Breakout");
    SetTargetFPS(60);

    //===============> Ciclo principal

    while (!WindowShouldClose())
    {
        //===============> Lógica

        if (IsWindowResized)
        {
            ventanaAncho = GetScreenWidth();
            ventanaAlto = GetScreenHeight();
        }

        //===============> Dibujo

        BeginDrawing();
        ClearBackground(RAYWHITE);
        DrawText("¡Hola, mundo!", 30, 30, 40, BLACK);
        EndDrawing();
    }

    //===============> Cierre

    CloseWindow();
    return 0;
}