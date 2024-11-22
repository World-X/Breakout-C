//==============================> Librerías

#include "raylib.h"
#include "raymath.h"
#define RAYGUI_IMPLEMENTATION
#include "raygui.h"
#include "resource_dir.h"
#include "breakout.h"

//==============================> Macros

//===============> Funciones

#define MAX(a, b) ((a) > (b) ? (a) : (b))
#define MIN(a, b) ((a) < (b) ? (a) : (b))

//===============> Constantes literales

#define MENU_PRINCIPAL_LADRILLOS_FILAS 5
#define MENU_PRINCIPAL_LADRILLOS_COLUMNAS 10

//==============================> Enumeraciones

typedef enum
{
    ESCENA_MENU_PRINCIPAL = 0,
    ESCENA_JUEGO
} Escena;

//==============================> Variables globales

//===============> Tamaño del juego

int juegoAncho = 1920;
int juegoAlto = 1080;
int mitadJuegoAncho = juegoAncho / 2;
int mitadJuegoAlto = juegoAlto / 2;

Escena escenaActual = ESCENA_MENU_PRINCIPAL;

//==============================> Prototipos de funciones

void CicloMenuPrincipal();
void CicloJuego();

//==============================> Función principal

int main()
{
    SearchAndSetResourceDir("resources");

    //===============> Inicialización de ventana

    SetConfigFlags(ConfigFlags::FLAG_WINDOW_RESIZABLE | ConfigFlags::FLAG_VSYNC_HINT);
    int ventanaAncho = 1280;
    int ventanaAlto = 720;
    const char *juegoTitulo = "Breakout";
    InitWindow(ventanaAncho, ventanaAlto, juegoTitulo);
    SetTargetFPS(60);

    //===============> Inicialización de audio

    InitAudioDevice();
    SetMasterVolume(0.3f);
    Sound colisionWav = LoadSound("colision.wav");

    //===============> Configuración de raylib

    SetTraceLogLevel(TraceLogLevel::LOG_TRACE);
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

    const Color defaultBackgroundColor = (Color){0x18, 0x18, 0x18, 0xFF};

    RenderTexture2D juegoRenderTextura = LoadRenderTexture(juegoAncho, juegoAlto);
    SetTextureFilter(juegoRenderTextura.texture, TextureFilter::TEXTURE_FILTER_BILINEAR);

    const int anchoBotonMenuPrincipal = 420;
    const int altoBotonMenuPrincipal = 100;
    const int posicionYBotonMenuPrincipal = 600;
    const int margenBotonMenuPrincipal = 120;
    const Rectangle botonJugarRect = {mitadJuegoAncho - anchoBotonMenuPrincipal / 2, posicionYBotonMenuPrincipal, anchoBotonMenuPrincipal, altoBotonMenuPrincipal};
    const Rectangle botonLeaderboardRect = {mitadJuegoAncho - anchoBotonMenuPrincipal / 2, posicionYBotonMenuPrincipal + margenBotonMenuPrincipal, anchoBotonMenuPrincipal, altoBotonMenuPrincipal};
    const Rectangle botonSalirRect = {mitadJuegoAncho - anchoBotonMenuPrincipal / 2, posicionYBotonMenuPrincipal + margenBotonMenuPrincipal * 2, anchoBotonMenuPrincipal, altoBotonMenuPrincipal};

    Pelota pelotaMenuPrincipal = Pelota((Vector2){mitadJuegoAncho, mitadJuegoAlto}, (Vector2){(float)GetRandomValue(300, 420), (float)GetRandomValue(300, 420)}, 20.0f, (Color){0xC8, 0xC8, 0xC8, 0xFF});

    if (GetRandomValue(0, 1) == 0)
    {
        pelotaMenuPrincipal.velocidad.x *= -1.0f;
    }

    CPUPaleta cpuPaletaMenuPrincipal = CPUPaleta((Vector2){mitadJuegoAncho, juegoAlto - 60.0f}, (Vector2){200.0f, 20.0f}, 420.0f, 4, 2.0f, (Color){186, 0xFF, 0xFF, 0xFF});
    CPUPaleta cpuPaletaMenuPrincipalEnemigo = CPUPaleta((Vector2){mitadJuegoAncho, 260.0f}, (Vector2){100.0f, 20.0f}, 210.0f, 4, 2.0f, (Color){0xFF, 186, 186, 0xFF});

    Color ladrillosColores[MENU_PRINCIPAL_LADRILLOS_FILAS] = {(Color){0xFF, 0x00, 0x00, 0xFF}, (Color){0xFF, 0xA5, 0x00, 0xFF}, (Color){0xFF, 0xFF, 0x00, 0xFF}, (Color){0x00, 0x7F, 0x00, 0xFF}, (Color){0x00, 0x00, 0xFF, 0xFF}};
    const float ladrillosSeparacion = 12.0f;
    Ladrillo ladrillosMenuPrincipal[MENU_PRINCIPAL_LADRILLOS_FILAS][MENU_PRINCIPAL_LADRILLOS_COLUMNAS];
    for (int fila = 0; fila < MENU_PRINCIPAL_LADRILLOS_FILAS; fila++)
    {
        for (int columna = 0; columna < MENU_PRINCIPAL_LADRILLOS_COLUMNAS; columna++)
        {
            Vector2 tamaño = (Vector2){(juegoAncho - ladrillosSeparacion * (MENU_PRINCIPAL_LADRILLOS_COLUMNAS + 1)) / MENU_PRINCIPAL_LADRILLOS_COLUMNAS, 30.0f};
            Vector2 posicion = (Vector2){columna * (tamaño.x + ladrillosSeparacion) + ladrillosSeparacion, fila * (tamaño.y + ladrillosSeparacion) + ladrillosSeparacion};
            ladrillosMenuPrincipal[fila][columna] = Ladrillo(posicion, tamaño, 0.5f, 4, ladrillosColores[fila], true, 100 * (MENU_PRINCIPAL_LADRILLOS_FILAS - fila));
        }
    }

    const Vector2 obstaculoTamaño = {96.0f, 96.0f};
    Obstaculo obstaculoMenuPrincipal[6];
    obstaculoMenuPrincipal[0] = Obstaculo((Vector2){306.0f, 720.0f}, (Vector2){96.0f, 96.0f}, BLACK, WHITE, 2.0f);
    obstaculoMenuPrincipal[1] = Obstaculo((Vector2){574.0f, 524.0f}, (Vector2){128.0f, 64.0f}, BLACK, WHITE, 2.0f);
    obstaculoMenuPrincipal[2] = Obstaculo((Vector2){1254.0f, 852.0f}, (Vector2){96.0f, 96.0f}, BLACK, WHITE, 2.0f);
    obstaculoMenuPrincipal[3] = Obstaculo((Vector2){173.0f, 389.0f}, (Vector2){64.0f, 64.0f}, BLACK, WHITE, 2.0f);
    obstaculoMenuPrincipal[4] = Obstaculo((Vector2){1599.0f, 591.0f}, (Vector2){96.0f, 96.0f}, BLACK, WHITE, 2.0f);
    obstaculoMenuPrincipal[5] = Obstaculo((Vector2){1329.0f, 564.0f}, (Vector2){64.0f, 128.0f}, BLACK, WHITE, 2.0f);

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

        Vector2 mouseOffset = {0};
        mouseOffset.x = (ventanaAncho - (juegoAncho * juegoEscala)) * 0.5f;
        mouseOffset.y = (ventanaAlto - (juegoAlto * juegoEscala)) * 0.5f;
        const Vector2 mouse = GetMousePosition();
        Vector2 mouseVirtual = {0};
        mouseVirtual.x = (mouse.x - mouseOffset.x) / juegoEscala;
        mouseVirtual.y = (mouse.y - mouseOffset.y) / juegoEscala;
        mouseVirtual = Vector2Clamp(mouseVirtual, (Vector2){0, 0}, (Vector2){(float)juegoAncho, (float)juegoAlto});
        SetMouseOffset(-mouseOffset.x, -mouseOffset.y);
        SetMouseScale(1 / juegoEscala, 1 / juegoEscala);

        //===============> Actualizar lógica del juego

        pelotaMenuPrincipal.Actualizar();
        cpuPaletaMenuPrincipal.Actualizar(pelotaMenuPrincipal.posicion);
        cpuPaletaMenuPrincipalEnemigo.Actualizar(pelotaMenuPrincipal.posicion);

        if (IsKeyPressed(KEY_UP) || IsKeyPressed(KEY_DOWN))
        {
            pelotaMenuPrincipal.velocidad.y *= -1.0f;
        }
        else if (IsKeyPressed(KEY_LEFT) || IsKeyPressed(KEY_RIGHT))
        {
            pelotaMenuPrincipal.velocidad.x *= -1.0f;
        }
        else if (IsKeyPressed(KEY_MINUS))
        {
            pelotaMenuPrincipal.velocidad = Vector2Scale(pelotaMenuPrincipal.velocidad, 0.5f);
        }
        else if (IsKeyPressed(KEY_EQUAL))
        {
            pelotaMenuPrincipal.velocidad = Vector2Scale(pelotaMenuPrincipal.velocidad, 2.0f);
        }
        //===============> Colisiones

        Vector2 pelotaMenuPrincipalVirtualPosicion = {pelotaMenuPrincipal.posicion.x + pelotaMenuPrincipal.velocidad.x * GetFrameTime(), pelotaMenuPrincipal.posicion.y + pelotaMenuPrincipal.velocidad.y * GetFrameTime()};

        // Colisión de la pelota con la paleta del jugador
        if (CheckCollisionCircleRec(pelotaMenuPrincipalVirtualPosicion, pelotaMenuPrincipal.radio, cpuPaletaMenuPrincipal.ObtenerRectangulo()))
        {
            pelotaMenuPrincipal.CambiarDireccionAlColisionarConRectangulo(cpuPaletaMenuPrincipal.ObtenerRectangulo());
            PlaySound(colisionWav);
        }

        // Colisión de la pelota con la paleta enemiga
        if (pelotaMenuPrincipal.velocidad.y < 0)
        {
            if (CheckCollisionCircleRec(pelotaMenuPrincipalVirtualPosicion, pelotaMenuPrincipal.radio, cpuPaletaMenuPrincipalEnemigo.ObtenerRectangulo()))
            {
                pelotaMenuPrincipal.CambiarDireccionAlColisionarConRectangulo(cpuPaletaMenuPrincipalEnemigo.ObtenerRectangulo());
                PlaySound(colisionWav);
            }
        }

        // Colisiones de la pelota con los ladrillos
        for (int i = 0; i < MENU_PRINCIPAL_LADRILLOS_FILAS; i++)
        {
            for (int j = 0; j < MENU_PRINCIPAL_LADRILLOS_COLUMNAS; j++)
            {
                if (ladrillosMenuPrincipal[i][j].activo)
                {
                    if (CheckCollisionCircleRec(pelotaMenuPrincipalVirtualPosicion, pelotaMenuPrincipal.radio, ladrillosMenuPrincipal[i][j].ObtenerRectangulo()))
                    {
                        ladrillosMenuPrincipal[i][j].activo = false;
                        pelotaMenuPrincipal.CambiarDireccionAlColisionarConRectangulo(ladrillosMenuPrincipal[i][j].ObtenerRectangulo());
                        PlaySound(colisionWav);
                    }
                }
            }
        }

        // Colisiones de la pelota con los obstáculos estáticos
        for (int i = 0; i < 6; i++)
        {
            if (CheckCollisionCircleRec(pelotaMenuPrincipalVirtualPosicion, pelotaMenuPrincipal.radio, obstaculoMenuPrincipal[i].ObtenerRectangulo()))
            {
                pelotaMenuPrincipal.CambiarDireccionAlColisionarConRectangulo(obstaculoMenuPrincipal[i].ObtenerRectangulo());
                PlaySound(colisionWav);
            }
        }

        //===============> Renderización

        BeginTextureMode(juegoRenderTextura);

        ClearBackground(defaultBackgroundColor);

        DrawRectangleGradientV(0, 0, juegoAncho, juegoAlto, (Color){0xFF, 0xFF, 0xFF, 0x0F}, (Color){0x00, 0x00, 0x00, 0x0F});

        for (int i = 0; i < MENU_PRINCIPAL_LADRILLOS_FILAS; i++)
        {
            for (int j = 0; j < MENU_PRINCIPAL_LADRILLOS_COLUMNAS; j++)
            {
                if (ladrillosMenuPrincipal[i][j].activo)
                {
                    ladrillosMenuPrincipal[i][j].Dibujar();
                }
            }
        }

        pelotaMenuPrincipal.Dibujar();
        cpuPaletaMenuPrincipal.Dibujar();
        cpuPaletaMenuPrincipalEnemigo.Dibujar();

        for (int i = 0; i < 6; i++)
        {
            obstaculoMenuPrincipal[i].Dibujar();
        }

        DrawText(juegoTitulo, mitadJuegoAncho - MeasureText(juegoTitulo, 160) / 2, 290, 160, WHITE);

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
        DrawText(TextFormat("Mouse: (%.0f, %.0f)", mouseVirtual.x, mouseVirtual.y), 10, 10, 20, WHITE);
        EndDrawing();
    }

    //===============> Cierre

    UnloadRenderTexture(juegoRenderTextura);
    UnloadSound(colisionWav);
    CloseAudioDevice();
    CloseWindow();
    return 0;
}

//==============================> Funciones
