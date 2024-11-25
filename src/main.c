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

#define COLOR_EERIE_BLACK \
    (Color) { 0x18, 0x18, 0x18, 0xFF }
#define COLOR_BITTERSWEET_SHIMMER \
    (Color) { 0xC9, 0x46, 0x46, 0xFF }
#define COLOR_COCOA_BROWN \
    (Color) { 0xC7, 0x6C, 0x37, 0xFF }
#define COLOR_COPPER \
    (Color) { 0xB5, 0x7B, 0x2C, 0xFF }
#define COLOR_APPLE_GREEN \
    (Color) { 0xA3, 0xA4, 0x25, 0xFF }
#define COLOR_PIGMENT_GREEN \
    (Color) { 0x46, 0xA1, 0x46, 0xFF }
#define COLOR_CERULEAN \
    (Color) { 0x43, 0x74, 0x88, 0xFF }
#define COLOR_PALATINATE_BLUE \
    (Color) { 0x40, 0x46, 0xC9, 0xFF }

//==============================> Tipos de datos

typedef unsigned char Byte;

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
int mitadJuegoAncho;
int mitadJuegoAlto;

//===============> Manejo de escenas

Escena escenaActual = ESCENA_MENU_PRINCIPAL;

//==============================> Prototipos de funciones

void CambiarEscena(Escena nueva_escena);
void GenerarLadrillos(Ladrillo *ladrillos, int filas, int columnas, float separacion, const Color *colores, float desplazamiento_vertical);

//==============================> Función principal

int main()
{
    SearchAndSetResourceDir("resources");

    //===============> Inicialización de ventana

    mitadJuegoAncho = juegoAncho / 2;
    mitadJuegoAlto = juegoAlto / 2;
    SetConfigFlags((ConfigFlags)FLAG_WINDOW_RESIZABLE | (ConfigFlags)FLAG_VSYNC_HINT);
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

    SetTraceLogLevel((TraceLogLevel)LOG_TRACE);
    SetExitKey(KEY_NULL);

    //===============> Inicialización de GUI

    GuiSetStyle((GuiControl)DEFAULT, (GuiDefaultProperty)TEXT_SIZE, 50);
    GuiSetStyle((GuiControl)DEFAULT, (GuiDefaultProperty)TEXT_SPACING, 4);

    GuiSetStyle((GuiControl)BUTTON, (GuiControlProperty)TEXT_COLOR_NORMAL, ColorToInt(RAYWHITE));
    GuiSetStyle((GuiControl)BUTTON, (GuiControlProperty)BASE_COLOR_NORMAL, ColorToInt(DARKGRAY));
    GuiSetStyle((GuiControl)BUTTON, (GuiControlProperty)BORDER_COLOR_NORMAL, ColorToInt(RAYWHITE));

    GuiSetStyle((GuiControl)BUTTON, (GuiControlProperty)TEXT_COLOR_FOCUSED, ColorToInt(RAYWHITE));
    GuiSetStyle((GuiControl)BUTTON, (GuiControlProperty)BASE_COLOR_FOCUSED, ColorToInt(GRAY));
    GuiSetStyle((GuiControl)BUTTON, (GuiControlProperty)BORDER_COLOR_FOCUSED, ColorToInt(RAYWHITE));

    GuiSetStyle((GuiControl)BUTTON, (GuiControlProperty)TEXT_COLOR_PRESSED, ColorToInt(DARKGRAY));
    GuiSetStyle((GuiControl)BUTTON, (GuiControlProperty)BASE_COLOR_PRESSED, ColorToInt(LIGHTGRAY));
    GuiSetStyle((GuiControl)BUTTON, (GuiControlProperty)BORDER_COLOR_PRESSED, ColorToInt(DARKGRAY));

    GuiSetIconScale(3);

    //===============> Variables locales a la función principal

    RenderTexture2D juegoRenderTextura = LoadRenderTexture(juegoAncho, juegoAlto);
    SetTextureFilter(juegoRenderTextura.texture, (TextureFilter)TEXTURE_FILTER_BILINEAR);

    //==========> Menú principal

    //=====> Botones

    const int anchoBotonMenuPrincipal = 420;
    const int altoBotonMenuPrincipal = 100;
    const int posicionYBotonMenuPrincipal = 500;
    const int margenBotonMenuPrincipal = 120;

    const Rectangle botonJugarRect = {mitadJuegoAncho - anchoBotonMenuPrincipal / 2, posicionYBotonMenuPrincipal, anchoBotonMenuPrincipal, altoBotonMenuPrincipal};
    const Rectangle botonLeaderboardRect = {mitadJuegoAncho - anchoBotonMenuPrincipal / 2, posicionYBotonMenuPrincipal + margenBotonMenuPrincipal, anchoBotonMenuPrincipal, altoBotonMenuPrincipal};
    const Rectangle botonSalirRect = {mitadJuegoAncho - anchoBotonMenuPrincipal / 2, posicionYBotonMenuPrincipal + margenBotonMenuPrincipal * 2, anchoBotonMenuPrincipal, altoBotonMenuPrincipal};

    //=====> Pelota

    Pelota pelotaMenuPrincipal = (Pelota){(Vector2){mitadJuegoAncho, mitadJuegoAlto}, (Vector2){(float)GetRandomValue(300, 420), (float)GetRandomValue(300, 420)}, 20.0f, (Color){0xC8, 0xC8, 0xC8, 0xFF}};

    if (GetRandomValue(0, 1) == 0)
    {
        CambiarDireccionPelota(&pelotaMenuPrincipal, (Direccion)HORIZONTAL);
    }

    //=====> Paletas

    Paleta jugadorPaletaMenuPrincipal = (Paleta){(Vector2){mitadJuegoAncho, juegoAlto - 140.0f}, (Vector2){200.0f, 20.0f}, 440.0f, 4, 2.0f, (Color){186, 0xFF, 0xFF, 0xFF}};
    Paleta cpuPaletaMenuPrincipal = (Paleta){(Vector2){mitadJuegoAncho, 260.0f}, (Vector2){100.0f, 20.0f}, 200.0f, 4, 2.0f, (Color){0xFF, 186, 186, 0xFF}};

    //=====> Ladrillos

    const Color ladrillosColoresMenuPrincipal[MENU_PRINCIPAL_LADRILLOS_FILAS] = {COLOR_BITTERSWEET_SHIMMER, COLOR_COPPER, COLOR_APPLE_GREEN, COLOR_PIGMENT_GREEN, COLOR_PALATINATE_BLUE};

    Ladrillo ladrillosMenuPrincipal[MENU_PRINCIPAL_LADRILLOS_FILAS][MENU_PRINCIPAL_LADRILLOS_COLUMNAS];
    GenerarLadrillos((Ladrillo *)ladrillosMenuPrincipal, MENU_PRINCIPAL_LADRILLOS_FILAS, MENU_PRINCIPAL_LADRILLOS_COLUMNAS, 12.0f, ladrillosColoresMenuPrincipal, 0.0f);

    //=====> Obstáculos

    Obstaculo obstaculoMenuPrincipal[6];
    obstaculoMenuPrincipal[0] = (Obstaculo){(Vector2){306.0f, 720.0f}, (Vector2){96.0f, 96.0f}, BLACK, WHITE, 2.0f};
    obstaculoMenuPrincipal[1] = (Obstaculo){(Vector2){574.0f, 524.0f}, (Vector2){128.0f, 64.0f}, BLACK, WHITE, 2.0f};
    obstaculoMenuPrincipal[2] = (Obstaculo){(Vector2){1487.0f, 433.0f}, (Vector2){96.0f, 96.0f}, BLACK, WHITE, 2.0f};
    obstaculoMenuPrincipal[3] = (Obstaculo){(Vector2){173.0f, 389.0f}, (Vector2){64.0f, 64.0f}, BLACK, WHITE, 2.0f};
    obstaculoMenuPrincipal[4] = (Obstaculo){(Vector2){1599.0f, 691.0f}, (Vector2){96.0f, 96.0f}, BLACK, WHITE, 2.0f};
    obstaculoMenuPrincipal[5] = (Obstaculo){(Vector2){1329.0f, 664.0f}, (Vector2){64.0f, 128.0f}, BLACK, WHITE, 2.0f};

    //==========> Juego

    //=====> Variables genéricas

    Byte nivelActual = 1;
    unsigned int puntosJugador = 0;
    bool pausaJuego = false;
    bool haEmpezadoJuego = false;
    char puntosTexto[10] = {0};
    char nivelTexto[10] = {0};
    bool acabaDePausar = false;

    //=====> Botones

    const int anchoBotonPausa = 480;
    const int altoBotonPausa = 120;
    const int margenBotonPausa = 160;
    const int separacionBotonPausa = 20;

    const Rectangle botonReanudarRect = {mitadJuegoAncho - anchoBotonPausa / 2, mitadJuegoAlto - altoBotonPausa - separacionBotonPausa / 2, anchoBotonPausa, altoBotonPausa};
    const Rectangle botonSalirJuegoRect = {mitadJuegoAncho - anchoBotonPausa / 2, mitadJuegoAlto + separacionBotonPausa / 2, anchoBotonPausa, altoBotonPausa};

    //=====> Pelota

    Pelota pelotaJuego = (Pelota){(Vector2){(float)GetRandomValue(0, juegoAncho - 20.0f), 275.0f}, (Vector2){0.0f, 0.0f}, 20.0f, (Color){0xC8, 0xC8, 0xC8, 0xFF}};

    //=====> Paletas

    Paleta jugadorPaleta = (Paleta){(Vector2){mitadJuegoAncho, juegoAlto - 140.0f}, (Vector2){300.0f, 20.0f}, 600.0f, 4, 2.0f, (Color){186, 0xFF, 0xFF, 0xFF}};

    //=====> Ladrillos

    const Color ladrillosColoresJuego[3] = {COLOR_PIGMENT_GREEN, COLOR_CERULEAN, COLOR_PALATINATE_BLUE};

    Ladrillo ladrillosJuego[3][5];

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

        if (escenaActual == (Escena)ESCENA_MENU_PRINCIPAL)
        {
            ActualizarPelota(&pelotaMenuPrincipal);
            ActualizarCPUPaleta(&cpuPaletaMenuPrincipal, pelotaMenuPrincipal.posicion);
            ActualizarCPUPaleta(&jugadorPaletaMenuPrincipal, pelotaMenuPrincipal.posicion);

            //! TECLAS DE DEPURACIÓN
            if (IsKeyPressed(KEY_UP) || IsKeyPressed(KEY_DOWN))
            {
                CambiarDireccionPelota(&pelotaMenuPrincipal, (Direccion)VERTICAL);
            }
            else if (IsKeyPressed(KEY_LEFT) || IsKeyPressed(KEY_RIGHT))
            {
                CambiarDireccionPelota(&pelotaMenuPrincipal, (Direccion)HORIZONTAL);
            }
            else if (IsKeyPressed(KEY_MINUS))
            {
                pelotaMenuPrincipal.velocidad = Vector2Scale(pelotaMenuPrincipal.velocidad, 0.5f);
            }
            else if (IsKeyPressed(KEY_EQUAL))
            {
                pelotaMenuPrincipal.velocidad = Vector2Scale(pelotaMenuPrincipal.velocidad, 2.0f);
            }
        }
        else if (escenaActual == (Escena)ESCENA_JUEGO)
        {
            //! TECLAS DE DEPURACIÓN
            if (!pausaJuego)
            {
                if (haEmpezadoJuego)
                {
                    ActualizarPelota(&pelotaJuego);
                }
                ActualizarPaleta(&jugadorPaleta);
            }
        }

        //===============> Colisiones

        if (escenaActual == (Escena)ESCENA_MENU_PRINCIPAL)
        {
            Vector2 pelotaMenuPrincipalVirtualPosicion = {pelotaMenuPrincipal.posicion.x + pelotaMenuPrincipal.velocidad.x * GetFrameTime(), pelotaMenuPrincipal.posicion.y + pelotaMenuPrincipal.velocidad.y * GetFrameTime()};

            // Colisión de la pelota con la paleta del jugador
            if (CheckCollisionCircleRec(pelotaMenuPrincipalVirtualPosicion, pelotaMenuPrincipal.radio, ObtenerRectanguloPaleta(&jugadorPaletaMenuPrincipal)))
            {
                CambiarDireccionColisionRectPelota(&pelotaMenuPrincipal, ObtenerRectanguloPaleta(&jugadorPaletaMenuPrincipal));
                PlaySound(colisionWav);
            }

            // Colisión de la pelota con la paleta enemiga
            if (pelotaMenuPrincipal.velocidad.y < 0)
            {
                if (CheckCollisionCircleRec(pelotaMenuPrincipalVirtualPosicion, pelotaMenuPrincipal.radio, ObtenerRectanguloPaleta(&cpuPaletaMenuPrincipal)))
                {
                    CambiarDireccionColisionRectPelota(&pelotaMenuPrincipal, ObtenerRectanguloPaleta(&cpuPaletaMenuPrincipal));
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
                        if (CheckCollisionCircleRec(pelotaMenuPrincipalVirtualPosicion, pelotaMenuPrincipal.radio, ObtenerRectanguloLadrillo(&ladrillosMenuPrincipal[i][j])))
                        {
                            ladrillosMenuPrincipal[i][j].activo = false;
                            CambiarDireccionColisionRectPelota(&pelotaMenuPrincipal, ObtenerRectanguloLadrillo(&ladrillosMenuPrincipal[i][j]));
                            PlaySound(colisionWav);
                            // puntosJugador += ladrillosMenuPrincipal[i][j].puntos;
                        }
                    }
                }
            }

            // Colisiones de la pelota con los obstáculos estáticos
            for (int i = 0; i < 6; i++)
            {
                if (CheckCollisionCircleRec(pelotaMenuPrincipalVirtualPosicion, pelotaMenuPrincipal.radio, ObtenerRectanguloObstaculo(&obstaculoMenuPrincipal[i])))
                {
                    CambiarDireccionColisionRectPelota(&pelotaMenuPrincipal, ObtenerRectanguloObstaculo(&obstaculoMenuPrincipal[i]));
                    PlaySound(colisionWav);
                }
            }
        }
        else if (escenaActual == (Escena)ESCENA_JUEGO)
        {
            // Colisión de la pelota con la paleta del jugador
            if (CheckCollisionCircleRec(pelotaJuego.posicion, pelotaJuego.radio, ObtenerRectanguloPaleta(&jugadorPaleta)))
            {
                CambiarDireccionColisionRectPelota(&pelotaJuego, ObtenerRectanguloPaleta(&jugadorPaleta));
                PlaySound(colisionWav);
            }

            // Colisiones de la pelota con los ladrillos
            for (int i = 0; i < 3; i++)
            {
                for (int j = 0; j < 5; j++)
                {
                    if (ladrillosJuego[i][j].activo)
                    {
                        if (CheckCollisionCircleRec(pelotaJuego.posicion, pelotaJuego.radio, ObtenerRectanguloLadrillo(&ladrillosJuego[i][j])))
                        {
                            ladrillosJuego[i][j].activo = false;
                            CambiarDireccionColisionRectPelota(&pelotaJuego, ObtenerRectanguloLadrillo(&ladrillosJuego[i][j]));
                            PlaySound(colisionWav);
                            puntosJugador += ladrillosJuego[i][j].puntos;
                            sprintf(puntosTexto, "%u", puntosJugador);
                        }
                    }
                }
            }
        }

        //===============> Renderización

        BeginTextureMode(juegoRenderTextura);

        ClearBackground(COLOR_EERIE_BLACK);

        DrawRectangleGradientV(0, 0, juegoAncho, juegoAlto, (Color){0xFF, 0xFF, 0xFF, 0x0F}, (Color){0x00, 0x00, 0x00, 0x0F});

        if (escenaActual == (Escena)ESCENA_MENU_PRINCIPAL)
        {

            for (int i = 0; i < MENU_PRINCIPAL_LADRILLOS_FILAS; i++)
            {
                for (int j = 0; j < MENU_PRINCIPAL_LADRILLOS_COLUMNAS; j++)
                {
                    if (ladrillosMenuPrincipal[i][j].activo)
                    {
                        DibujarLadrillo(&ladrillosMenuPrincipal[i][j]);
                    }
                }
            }

            DibujarPelota(&pelotaMenuPrincipal);
            DibujarPaleta(&jugadorPaletaMenuPrincipal);
            DibujarPaleta(&cpuPaletaMenuPrincipal);

            for (int i = 0; i < 6; i++)
            {
                DibujarObstaculo(&obstaculoMenuPrincipal[i]);
            }

            DrawText(juegoTitulo, mitadJuegoAncho - MeasureText(juegoTitulo, 160) / 2, 290, 160, WHITE);

            if (GuiButton(botonJugarRect, "Jugar") || IsKeyPressed(KEY_SPACE) || IsKeyPressed(KEY_ENTER))
            {
                TraceLog(LOG_INFO, "JUGAR");
                CambiarEscena(ESCENA_JUEGO);
                GenerarLadrillos((Ladrillo *)ladrillosJuego, 3, 5, 16.0f, ladrillosColoresJuego, 48.0f);
                GuiSetStyle((GuiControl)DEFAULT, (GuiDefaultProperty)TEXT_SIZE, 70);
                nivelActual = 1;
                puntosJugador = 0;
                haEmpezadoJuego = false;
                pausaJuego = false;
                strcpy(puntosTexto, "0");
                strcpy(nivelTexto, "Nivel 1");
            }
            if (GuiButton(botonLeaderboardRect, "Leaderboard") || IsKeyPressed(KEY_L))
            {
                TraceLog(LOG_INFO, "LEADERBOARD");
            }
            if (GuiButton(botonSalirRect, "Salir") || IsKeyPressed(KEY_ESCAPE))
            {
                TraceLog(LOG_INFO, "SALIR");
                break;
            }
        }
        else if (escenaActual == (Escena)ESCENA_JUEGO)
        {
            for (int i = 0; i < 3; i++)
            {
                for (int j = 0; j < 5; j++)
                {
                    if (ladrillosJuego[i][j].activo)
                    {
                        DibujarLadrillo(&ladrillosJuego[i][j]);
                    }
                }
            }

            DibujarPelota(&pelotaJuego);
            DibujarPaleta(&jugadorPaleta);

            if (!haEmpezadoJuego)
            {
                if (GuiButton((Rectangle){mitadJuegoAncho - anchoBotonPausa / 2, mitadJuegoAlto - altoBotonPausa / 2, anchoBotonPausa, altoBotonPausa}, "Empezar") || IsKeyPressed(KEY_SPACE) || IsKeyPressed(KEY_ENTER))
                {
                    haEmpezadoJuego = true;
                    pelotaJuego.velocidad = (Vector2){(float)GetRandomValue(200, 300), (float)GetRandomValue(300, 420)};
                    if (rand() % 2 == 0)
                    {
                        CambiarDireccionPelota(&pelotaJuego, (Direccion)HORIZONTAL);
                    }
                }
            }
            else if (pausaJuego)
            {
                if (GuiButton(botonReanudarRect, "Reanudar") || IsKeyPressed(KEY_SPACE) || IsKeyPressed(KEY_ENTER))
                {
                    TraceLog(LOG_INFO, "REANUDAR");
                    pausaJuego = false;
                }
                if (GuiButton(botonSalirJuegoRect, "Salir") || IsKeyPressed(KEY_ESCAPE) && !acabaDePausar)
                {
                    TraceLog(LOG_INFO, "SALIR");
                    CambiarEscena(ESCENA_MENU_PRINCIPAL);
                    GuiSetStyle((GuiControl)DEFAULT, (GuiDefaultProperty)TEXT_SIZE, 50);
                }
            }

            DrawText(puntosTexto, juegoAncho - MeasureText(puntosTexto, 48) - 10, 10, 48, RAYWHITE);
            DrawText(nivelTexto, mitadJuegoAncho - MeasureText(nivelTexto, 64) / 2, juegoAlto - 96, 64, RAYWHITE);
        }

        EndTextureMode();

        //===============> Dibujo

        BeginDrawing();
        ClearBackground(BLACK);
        DrawTexturePro(juegoRenderTextura.texture, (Rectangle){0, 0, (float)juegoAncho, (float)-juegoAlto}, (Rectangle){(ventanaAncho - juegoAncho * juegoEscala) / 2, (ventanaAlto - juegoAlto * juegoEscala) / 2, juegoAncho * juegoEscala, juegoAlto * juegoEscala}, (Vector2){0, 0}, 0, WHITE);
        DrawFPS(10, 10);
        /*
        // DrawText(TextFormat("Mouse: (%.0f, %.0f)", mouseVirtual.x, mouseVirtual.y), 10, 10, 20, WHITE);
        if (escenaActual == Escena::ESCENA_MENU_PRINCIPAL)
        {
            // DrawText("MENU PRINCIPAL", ventanaAncho - MeasureText("MENU PRINCIPAL", 20) - 10, 10, 20, WHITE);
        }
        else if (escenaActual == Escena::ESCENA_JUEGO)
        {
            // DrawText(TextFormat("Puntos: %u", puntosJugador), ventanaAncho / 2 - MeasureText("Puntos: 100", 36) / 2, ventanaAlto - 48, 36, WHITE);
            // DrawText("JUEGO", ventanaAncho - MeasureText("JUEGO", 20) - 10, 10, 20, WHITE);
        }
        */
        EndDrawing();

        //===============> Lógica después de dibujar

        if (escenaActual == (Escena)ESCENA_JUEGO)
        {
            if (haEmpezadoJuego)
            {
                if (!pausaJuego && IsKeyPressed(KEY_ESCAPE))
                {
                    pausaJuego = true;
                    acabaDePausar = true;
                }
                else if (pausaJuego && IsKeyReleased(KEY_ESCAPE))
                {
                    acabaDePausar = false;
                }
            }
        }
    }

    //===============> Cierre

    UnloadRenderTexture(juegoRenderTextura);
    UnloadSound(colisionWav);
    CloseAudioDevice();
    CloseWindow();
    return 0;
}

//==============================> Funciones

void CambiarEscena(Escena nueva_escena)
{
    escenaActual = nueva_escena;
}

void GenerarLadrillos(Ladrillo *ladrillos, int filas, int columnas, float separacion, const Color *colores, float desplazamiento_vertical)
{
    for (int fila = 0; fila < filas; fila++)
    {
        for (int columna = 0; columna < columnas; columna++)
        {
            Vector2 tamaño = (Vector2){(juegoAncho - separacion * (columnas + 1)) / columnas, 30.0f};
            Vector2 posicion = (Vector2){columna * (tamaño.x + separacion) + separacion, fila * (tamaño.y + separacion) + separacion + desplazamiento_vertical};
            *(ladrillos + fila * columnas + columna) = (Ladrillo){posicion, tamaño, 0.5f, 4, colores[fila], true, 100 * (filas - fila)};
        }
    }
}
