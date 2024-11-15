//==============================> Librerías

#include "raylib.h"
#include "raymath.h"
#define RAYGUI_IMPLEMENTATION
#include "raygui.h"
#include "breakout.h"

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
    int posicionYBotonMenuPrincipal = 600;
    int margenBotonMenuPrincipal = 120;
    Rectangle botonJugarRect = {mitadJuegoAncho - anchoBotonMenuPrincipal / 2, posicionYBotonMenuPrincipal, anchoBotonMenuPrincipal, altoBotonMenuPrincipal};
    Rectangle botonLeaderboardRect = {mitadJuegoAncho - anchoBotonMenuPrincipal / 2, posicionYBotonMenuPrincipal + margenBotonMenuPrincipal, anchoBotonMenuPrincipal, altoBotonMenuPrincipal};
    Rectangle botonSalirRect = {mitadJuegoAncho - anchoBotonMenuPrincipal / 2, posicionYBotonMenuPrincipal + margenBotonMenuPrincipal * 2, anchoBotonMenuPrincipal, altoBotonMenuPrincipal};

    Pelota pelotaMenuPrincipal = Pelota((Vector2){mitadJuegoAncho, mitadJuegoAlto}, (Vector2){(float)GetRandomValue(300, 420), (float)GetRandomValue(300, 420)}, 20.0f, (Color){200, 200, 200, 255});

    if (GetRandomValue(0, 1) == 0)
    {
        pelotaMenuPrincipal.velocidad.x *= -1.0f;
    }

    CPUPaleta cpuPaletaMenuPrincipal = CPUPaleta((Vector2){mitadJuegoAncho, juegoAlto - 60.0f}, (Vector2){200.0f, 20.0f}, 420.0f, 4, 2.0f, (Color){186, 225, 255, 255});

    Color ladrillosColores[5] = {(Color){255, 0, 0, 255}, (Color){255, 165, 0, 255}, (Color){255, 255, 0, 255}, (Color){0, 128, 0, 255}, (Color){0, 0, 255, 255}};

    Ladrillo ladrillosMenuPrincipal[5][10];
    for (int i = 0; i < 5; i++)
    {
        for (int j = 0; j < 10; j++)
        {
            float separacion = 10.0f;
            Vector2 tamaño = (Vector2){(juegoAncho - separacion * 11) / 10, 40.0f};
            Vector2 posicion = (Vector2){j * (tamaño.x + separacion) + separacion, i * (tamaño.y + separacion) + separacion};
            ladrillosMenuPrincipal[i][j] = Ladrillo(posicion, tamaño, 0.5f, 4, ladrillosColores[i], true, 10 * (5 - i));
        }
    }

    //===============> Ciclo principal

    while (!WindowShouldClose())
    {
        //===============> Actualización de ventana

        if (IsWindowResized())
        {
            ventanaAncho = GetScreenWidth();
            ventanaAlto = GetScreenHeight();
        }

        float juegoEscala = MIN((float)ventanaAncho / juegoAncho, (float)ventanaAlto / juegoAlto);

        //===============> Mouse virtual

        Vector2 mouse = GetMousePosition();
        Vector2 mouseVirtual = {0};
        mouseVirtual.x = (mouse.x - (ventanaAncho - (juegoAncho * juegoEscala)) * 0.5f) / juegoEscala;
        mouseVirtual.y = (mouse.y - (ventanaAlto - (juegoAlto * juegoEscala)) * 0.5f) / juegoEscala;
        mouseVirtual = Vector2Clamp(mouseVirtual, (Vector2){0, 0}, (Vector2){(float)juegoAncho, (float)juegoAlto});
        SetMouseOffset(-(ventanaAncho - (juegoAncho * juegoEscala)) * 0.5f, -(ventanaAlto - (juegoAlto * juegoEscala)) * 0.5f);
        SetMouseScale(1 / juegoEscala, 1 / juegoEscala);

        //===============> Lógica

        pelotaMenuPrincipal.Actualizar();
        cpuPaletaMenuPrincipal.Actualizar(pelotaMenuPrincipal.posicion);

        if (CheckCollisionCircleRec(pelotaMenuPrincipal.posicion, pelotaMenuPrincipal.radio, cpuPaletaMenuPrincipal.ObtenerRectangulo()))
        {
            pelotaMenuPrincipal.velocidad.y *= -1.0f;
            pelotaMenuPrincipal.posicion.y += pelotaMenuPrincipal.velocidad.y * GetFrameTime();
        }

        for (int i = 0; i < 5; i++)
        {
            for (int j = 0; j < 10; j++)
            {
                if (ladrillosMenuPrincipal[i][j].activo)
                {
                    if (CheckCollisionCircleRec(pelotaMenuPrincipal.posicion, pelotaMenuPrincipal.radio, ladrillosMenuPrincipal[i][j].ObtenerRectangulo()))
                    {
                        ladrillosMenuPrincipal[i][j].activo = false;
                        pelotaMenuPrincipal.velocidad.y *= -1.0f;
                        pelotaMenuPrincipal.posicion.y += pelotaMenuPrincipal.velocidad.y * GetFrameTime();
                    }
                }
            }
        }

        //===============> Renderización

        BeginTextureMode(juegoRenderTextura);
        ClearBackground(defaultBackgroundColor);
        for (int i = 0; i < 5; i++)
        {
            for (int j = 0; j < 10; j++)
            {
                if (ladrillosMenuPrincipal[i][j].activo)
                {
                    ladrillosMenuPrincipal[i][j].Dibujar();
                }
            }
        }
        pelotaMenuPrincipal.Dibujar();
        cpuPaletaMenuPrincipal.Dibujar();
        DrawText(juegoTitulo, mitadJuegoAncho - MeasureText(juegoTitulo, 160) / 2, 340, 160, WHITE);
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
        DrawFPS(10, ventanaAlto - 30);
        EndDrawing();
    }

    //===============> Cierre

    UnloadRenderTexture(juegoRenderTextura);
    CloseWindow();
    return 0;
}