//==============================> Librerías

#include "raylib.h"
#include "raymath.h"
#define RAYGUI_IMPLEMENTATION
#include "raygui.h"

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

        float juegoEscala = MIN((float)ventanaAncho / juegoAncho, (float)ventanaAlto / juegoAlto);

        Vector2 mouse = GetMousePosition();
        Vector2 virtualMouse = {0};
        virtualMouse.x = (mouse.x - (GetScreenWidth() - (juegoAncho * juegoEscala)) * 0.5f) / juegoEscala;
        virtualMouse.y = (mouse.y - (GetScreenHeight() - (juegoAlto * juegoEscala)) * 0.5f) / juegoEscala;
        virtualMouse = Vector2Clamp(virtualMouse, (Vector2){0, 0}, (Vector2){(float)juegoAncho, (float)juegoAlto});
        SetMouseOffset(-(GetScreenWidth() - (juegoAncho * juegoEscala)) * 0.5f, -(GetScreenHeight() - (juegoAlto * juegoEscala)) * 0.5f);
		SetMouseScale(1 / juegoEscala, 1 / juegoEscala);

        //===============> Renderización

        BeginTextureMode(juegoRenderTextura);
        ClearBackground(GetColor(GuiGetStyle(DEFAULT, BACKGROUND_COLOR)));
        if (GuiButton((Rectangle){10, 10, 100, 40}, "#191#¡Hola, mundo!"))
        {
            TraceLog(LOG_INFO, "¡Hola, mundo!");
        }
        DrawText("¡Hola, mundo!", 30, 30, 40, BLACK);
        DrawRectangle(juegoAncho - 40, juegoAlto - 40, 40, 40, RED);
        EndTextureMode();

        //===============> Dibujo

        BeginDrawing();
        ClearBackground(BLACK);
        DrawTexturePro(juegoRenderTextura.texture, (Rectangle){0, 0, juegoAncho, -juegoAlto}, (Rectangle){(ventanaAncho - juegoAncho * juegoEscala) / 2, (ventanaAlto - juegoAlto * juegoEscala) / 2, juegoAncho * juegoEscala, juegoAlto * juegoEscala}, (Vector2){0, 0}, 0, WHITE);
        EndDrawing();
    }

    //===============> Cierre

    UnloadRenderTexture(juegoRenderTextura);
    CloseWindow();
    return 0;
}