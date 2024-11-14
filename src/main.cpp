//==============================> Librerías

#include "raylib.h"

//==============================> Macros

#define MAX(a, b) ((a) > (b) ? (a) : (b))
#define MIN(a, b) ((a) < (b) ? (a) : (b))

//==============================> Variables globales

int juegoAncho = 1920;
int juegoAlto = 1080;

//==============================> Función principal

int main()
{
    //===============> Inicialización

    SetTraceLogLevel(LOG_NONE);
    SetConfigFlags(FLAG_WINDOW_RESIZABLE | FLAG_VSYNC_HINT);
    int ventanaAncho = 1280;
    int ventanaAlto = 720;
    InitWindow(ventanaAncho, ventanaAlto, "Breakout");
    SetTargetFPS(60);

    //===============> Variables

    RenderTexture2D juegoRenderTextura = LoadRenderTexture(juegoAncho, juegoAlto);
    SetTextureFilter(juegoRenderTextura.texture, TEXTURE_FILTER_BILINEAR);

    //===============> Ciclo principal

    while (!WindowShouldClose())
    {
        //===============> Lógica

        if (IsWindowResized)
        {
            ventanaAncho = GetScreenWidth();
            ventanaAlto = GetScreenHeight();
        }

        float escala = MIN((float)ventanaAncho / juegoAncho, (float)ventanaAlto / juegoAlto);

        //===============> Renderización

        BeginTextureMode(juegoRenderTextura);
        ClearBackground(RAYWHITE);
        DrawText("¡Hola, mundo!", 30, 30, 40, BLACK);
        DrawRectangle(juegoAncho - 40, juegoAlto - 40, 40, 40, RED);
        EndTextureMode();

        //===============> Dibujo

        BeginDrawing();
        ClearBackground(BLACK);
        DrawTexturePro(juegoRenderTextura.texture, (Rectangle){0, 0, juegoAncho, -juegoAlto}, (Rectangle){(ventanaAncho - juegoAncho * escala) / 2, (ventanaAlto - juegoAlto * escala) / 2, juegoAncho * escala, juegoAlto * escala}, (Vector2){0, 0}, 0, WHITE);
        EndDrawing();
    }

    //===============> Cierre

    UnloadRenderTexture(juegoRenderTextura);
    CloseWindow();
    return 0;
}