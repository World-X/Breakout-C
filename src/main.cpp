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
        DrawText("Esquina inferior derecha.", GetScreenWidth() - 300, GetScreenHeight() - 30, 20, RED);
        EndDrawing();
    }
    CloseWindow();
    return 0;
}