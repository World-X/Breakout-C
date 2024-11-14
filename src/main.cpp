//==============================> Librerías

#include "raylib.h"

//==============================> Función principal

int main()
{
    //===============> Inicialización
    InitWindow(1280, 720, "Breakout");
    SetTargetFPS(60);
    //===============> Ciclo principal
    while (!WindowShouldClose())
    {
        BeginDrawing();
        ClearBackground(RAYWHITE);
        DrawText("¡Hola, mundo!", 30, 30, 40, BLACK);
        EndDrawing();
    }
    //===============> Cierre
    CloseWindow();
    return 0;
}