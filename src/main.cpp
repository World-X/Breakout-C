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
int mitadJuegoAncho = juegoAncho / 2;
int mitadJuegoAlto = juegoAlto / 2;

//==============================> Función principal

int main()
{
    //===============> Inicialización de ventana y configuración de raylib

    SetTraceLogLevel(TraceLogLevel::LOG_TRACE);
    SetConfigFlags(ConfigFlags::FLAG_WINDOW_RESIZABLE | ConfigFlags::FLAG_VSYNC_HINT);
    int ventanaAncho = 1280;
    int ventanaAlto = 720;
    const char *juegoTitulo = "Breakout";
    InitWindow(ventanaAncho, ventanaAlto, juegoTitulo);
    SetTargetFPS(60);
    SetExitKey(KEY_NULL);

    //===============> Inicialización de GUI

    GuiSetStyle(GuiControl::DEFAULT, GuiDefaultProperty::TEXT_SIZE, 50);
    GuiSetStyle(GuiControl::DEFAULT, GuiDefaultProperty::TEXT_SPACING, 4);

    GuiSetStyle(GuiControl::BUTTON, GuiControlProperty::TEXT_COLOR_NORMAL, ColorToInt(RAYWHITE));
    GuiSetStyle(GuiControl::BUTTON, GuiControlProperty::BASE_COLOR_NORMAL, ColorToInt(DARKGRAY));
    GuiSetStyle(GuiControl::BUTTON, GuiControlProperty::BORDER_COLOR_NORMAL, ColorToInt(RAYWHITE));

    GuiSetStyle(GuiControl::BUTTON, GuiControlProperty::TEXT_COLOR_FOCUSED, ColorToInt(RAYWHITE));
    GuiSetStyle(GuiControl::BUTTON, GuiControlProperty::BASE_COLOR_FOCUSED, ColorToInt(GRAY));
    GuiSetStyle(GuiControl::BUTTON, GuiControlProperty::BORDER_COLOR_FOCUSED, ColorToInt(RAYWHITE));

    GuiSetStyle(GuiControl::BUTTON, GuiControlProperty::TEXT_COLOR_PRESSED, ColorToInt(DARKGRAY));
    GuiSetStyle(GuiControl::BUTTON, GuiControlProperty::BASE_COLOR_PRESSED, ColorToInt(LIGHTGRAY));
    GuiSetStyle(GuiControl::BUTTON, GuiControlProperty::BORDER_COLOR_PRESSED, ColorToInt(DARKGRAY));

    GuiSetIconScale(3);

    //===============> Variables locales a la función principal

    Color defaultBackgroundColor = (Color){24, 24, 24, 255};

    RenderTexture2D juegoRenderTextura = LoadRenderTexture(juegoAncho, juegoAlto);
    SetTextureFilter(juegoRenderTextura.texture, TextureFilter::TEXTURE_FILTER_BILINEAR);

    int anchoBotonMenuPrincipal = 420;
    int altoBotonMenuPrincipal = 100;
    int posicionYBotonMenuPrincipal = 480;
    int margenBotonMenuPrincipal = 120;
    Rectangle botonJugarRect = {mitadJuegoAncho - anchoBotonMenuPrincipal / 2, posicionYBotonMenuPrincipal, anchoBotonMenuPrincipal, altoBotonMenuPrincipal};
    Rectangle botonLeaderboardRect = {mitadJuegoAncho - anchoBotonMenuPrincipal / 2, posicionYBotonMenuPrincipal + margenBotonMenuPrincipal, anchoBotonMenuPrincipal, altoBotonMenuPrincipal};
    Rectangle botonSalirRect = {mitadJuegoAncho - anchoBotonMenuPrincipal / 2, posicionYBotonMenuPrincipal + margenBotonMenuPrincipal * 2, anchoBotonMenuPrincipal, altoBotonMenuPrincipal};

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
        Vector2 mouseVirtual = {0};
        mouseVirtual.x = (mouse.x - (ventanaAncho - (juegoAncho * juegoEscala)) * 0.5f) / juegoEscala;
        mouseVirtual.y = (mouse.y - (ventanaAlto - (juegoAlto * juegoEscala)) * 0.5f) / juegoEscala;
        mouseVirtual = Vector2Clamp(mouseVirtual, (Vector2){0, 0}, (Vector2){(float)juegoAncho, (float)juegoAlto});
        SetMouseOffset(-(ventanaAncho - (juegoAncho * juegoEscala)) * 0.5f, -(ventanaAlto - (juegoAlto * juegoEscala)) * 0.5f);
        SetMouseScale(1 / juegoEscala, 1 / juegoEscala);

        //===============> Renderización

        BeginTextureMode(juegoRenderTextura);
        ClearBackground(defaultBackgroundColor);
        //// GuiDrawText(juegoTitulo, (Rectangle){mitadJuegoAncho, 50, juegoTituloLongitud, 60}, GuiTextAlignment::TEXT_ALIGN_CENTER, GetColor(GuiGetStyle(GuiControl::DEFAULT, GuiControlProperty::TEXT_COLOR_NORMAL)));
        DrawText(juegoTitulo, mitadJuegoAncho - MeasureText(juegoTitulo, 160) / 2, 120, 160, WHITE);
        if (GuiButton(botonJugarRect, "Jugar"))
        {
            TraceLog(LOG_INFO, "JUGAR");
        }
        if (GuiButton(botonLeaderboardRect, "Leaderboard"))
        {
            TraceLog(LOG_INFO, "LEADERBOARD");
        }
        if (GuiButton(botonSalirRect, "Salir"))
        {
            break;
        }
        EndTextureMode();

        //===============> Dibujo

        BeginDrawing();
        ClearBackground(BLACK);
        DrawTexturePro(juegoRenderTextura.texture, (Rectangle){0, 0, (float)juegoAncho, (float)-juegoAlto}, (Rectangle){(ventanaAncho - juegoAncho * juegoEscala) / 2, (ventanaAlto - juegoAlto * juegoEscala) / 2, juegoAncho * juegoEscala, juegoAlto * juegoEscala}, (Vector2){0, 0}, 0, WHITE);
        EndDrawing();
    }

    //===============> Cierre

    UnloadRenderTexture(juegoRenderTextura);
    CloseWindow();
    return 0;
}