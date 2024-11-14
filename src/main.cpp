#include "raylib.h"

int main()
{
    InitWindow(1280, 720, "Breakout");
    SetTargetFPS(60);
    while (!WindowShouldClose())
    {
        BeginDrawing();
        ClearBackground(RAYWHITE);
        DrawText("¡Hola, mundo!", 30, 30, 40, BLACK);
        EndDrawing();
    }
    CloseWindow();
    return 0;
}