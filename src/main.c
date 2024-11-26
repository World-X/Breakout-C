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

void CambiarPantallaCompleta(int ventana_ancho, int ventana_alto);
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
    int ventanaAnchoAnterior = ventanaAncho;
    int ventanaAltoAnterior = ventanaAlto;
    const char *juegoTitulo = "Breakout";
    InitWindow(ventanaAncho, ventanaAlto, juegoTitulo);
    SetTargetFPS(60);

    //===============> Inicialización de audio

    InitAudioDevice();
    SetMasterVolume(0.3f);
    Sound colisionWav = LoadSound("colision.wav");
    Sound explosionWav = LoadSound("explosion.wav");
    Sound seleccionWav = LoadSound("seleccion.wav");

    //===============> Configuración de raylib

    SetTraceLogLevel((TraceLogLevel)LOG_ALL);
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

    bool vaIniciarJuego = false;
    bool vaRegresarMenuPrincipal = false;

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

    Pelota pelotaMenuPrincipal = (Pelota){VECTOR2_CERO, VECTOR2_CERO, VECTOR2_UNO, 20.0f, (Color){0xC8, 0xC8, 0xC8, 0xFF}};
    pelotaMenuPrincipal.posicion.x = (float)GetRandomValue(0, juegoAncho - 20.0f);
    pelotaMenuPrincipal.posicion.y = 275.0f;
    pelotaMenuPrincipal.velocidad.x = 300.0f;
    pelotaMenuPrincipal.velocidad.y = (float)GetRandomValue(200, 350);
    if (rand() % 2 == 0)
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
    obstaculoMenuPrincipal[0] = (Obstaculo){(Vector2){306.0f, 720.0f}, (Vector2){96.0f, 96.0f}, BLACK, WHITE, 2.0f, 0.0f};
    obstaculoMenuPrincipal[1] = (Obstaculo){(Vector2){574.0f, 524.0f}, (Vector2){128.0f, 64.0f}, BLACK, WHITE, 2.0f, 0.0f};
    obstaculoMenuPrincipal[2] = (Obstaculo){(Vector2){1487.0f, 433.0f}, (Vector2){96.0f, 96.0f}, BLACK, WHITE, 2.0f, 0.0f};
    obstaculoMenuPrincipal[3] = (Obstaculo){(Vector2){173.0f, 389.0f}, (Vector2){64.0f, 64.0f}, BLACK, WHITE, 2.0f, 0.0f};
    obstaculoMenuPrincipal[4] = (Obstaculo){(Vector2){1599.0f, 691.0f}, (Vector2){96.0f, 96.0f}, BLACK, WHITE, 2.0f, 0.0f};
    obstaculoMenuPrincipal[5] = (Obstaculo){(Vector2){1329.0f, 664.0f}, (Vector2){64.0f, 128.0f}, BLACK, WHITE, 2.0f, 0.0f};

    //==========> Juego

    //=====> Variables genéricas

    Byte nivelActual = 1;
    short vidasJugador = 3;
    unsigned int puntosJugador = 0;
    bool pausaJuego = false;
    bool haEmpezadoJuego = false;
    char puntosTexto[10] = {0};
    char nivelTexto[10] = {0};
    bool acabaDePausar = false;
    bool vaResetearJuego = false;

    //=====> Botones

    const int anchoBotonPausa = 480;
    const int altoBotonPausa = 120;
    const int margenBotonPausa = 160;
    const int separacionBotonPausa = 20;

    const Rectangle botonReanudarRect = {mitadJuegoAncho - anchoBotonPausa / 2, mitadJuegoAlto - altoBotonPausa - separacionBotonPausa / 2, anchoBotonPausa, altoBotonPausa};
    const Rectangle botonSalirJuegoRect = {mitadJuegoAncho - anchoBotonPausa / 2, mitadJuegoAlto + separacionBotonPausa / 2, anchoBotonPausa, altoBotonPausa};

    //=====> Pelota

    Pelota pelotaJuego = (Pelota){VECTOR2_CERO, VECTOR2_CERO, VECTOR2_UNO, 20.0f, (Color){0xC8, 0xC8, 0xC8, 0xFF}};

    //=====> Paletas

    Paleta jugadorPaleta = (Paleta){(Vector2){mitadJuegoAncho, juegoAlto - 140.0f}, (Vector2){20.0f, 20.0f}, 0.0f, 4, 2.0f, (Color){186, 0xFF, 0xFF, 0xFF}};

    //=====> Ladrillos

    Byte ladrillosFilas = 0;
    Byte ladrillosColumnas = 0;
    Byte ladrillosFilasNiveles[5] = {3, 4, 5, 6, 7};
    Byte ladrillosColumnasNiveles[5] = {5, 5, 6, 10, 15};
    Ladrillo ladrillosNivel1[ladrillosFilasNiveles[0]][ladrillosColumnasNiveles[0]];
    Ladrillo ladrillosNivel2[ladrillosFilasNiveles[1]][ladrillosColumnasNiveles[1]];
    Ladrillo ladrillosNivel3[ladrillosFilasNiveles[2]][ladrillosColumnasNiveles[2]];
    Ladrillo ladrillosNivel4[ladrillosFilasNiveles[3]][ladrillosColumnasNiveles[3]];
    Ladrillo ladrillosNivel5[ladrillosFilasNiveles[4]][ladrillosColumnasNiveles[4]];
    Ladrillo *ladrillosNivelActual = (Ladrillo *)ladrillosNivel1;
    short ladrillosRestantes = -1;

    //=====> Texturas

    Texture corazonTextura = LoadTexture("heart.png");
    SetTextureFilter(corazonTextura, (TextureFilter)TEXTURE_FILTER_POINT);

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

        if (IsKeyPressed(KEY_ENTER) && (IsKeyDown(KEY_LEFT_ALT) || IsKeyDown(KEY_RIGHT_ALT)) || IsKeyPressed(KEY_F))
        {
            if (!IsWindowFullscreen())
            {
                ventanaAnchoAnterior = ventanaAncho;
                ventanaAltoAnterior = ventanaAlto;
            }
            CambiarPantallaCompleta(ventanaAnchoAnterior, ventanaAltoAnterior);
            ventanaAncho = GetScreenWidth();
            ventanaAlto = GetScreenHeight();
        }

        if (escenaActual == (Escena)ESCENA_MENU_PRINCIPAL)
        {
            ActualizarPelota(&pelotaMenuPrincipal);
            ActualizarCPUPaleta(&cpuPaletaMenuPrincipal, pelotaMenuPrincipal.posicion);
            ActualizarCPUPaleta(&jugadorPaletaMenuPrincipal, pelotaMenuPrincipal.posicion);
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
            Vector2 pelotaMenuPrincipalVirtualPosicion = {pelotaMenuPrincipal.posicion.x + CalcularMovimiento(pelotaMenuPrincipal.velocidad.x, pelotaMenuPrincipal.aceleracion.x), pelotaMenuPrincipal.posicion.y + CalcularMovimiento(pelotaMenuPrincipal.velocidad.y, pelotaMenuPrincipal.aceleracion.y)};

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
                    obstaculoMenuPrincipal[i].modulo = 1.0f;
                    PlaySound(colisionWav);
                }
                if (obstaculoMenuPrincipal[i].modulo > 0.0f)
                {
                    obstaculoMenuPrincipal[i].modulo = MAX(0.0f, obstaculoMenuPrincipal[i].modulo - 2.0f * GetFrameTime());
                }
            }

            if (pelotaMenuPrincipal.posicion.x - pelotaMenuPrincipal.radio <= 0.0f || pelotaMenuPrincipal.posicion.x + pelotaMenuPrincipal.radio >= juegoAncho)
            {
                CambiarDireccionPelota(&pelotaMenuPrincipal, (Direccion)HORIZONTAL);
                MoverPelota(&pelotaMenuPrincipal, (Direccion)HORIZONTAL);
            }
            if (pelotaMenuPrincipal.posicion.y - pelotaMenuPrincipal.radio <= 0.0f || pelotaMenuPrincipal.posicion.y + pelotaMenuPrincipal.radio >= juegoAlto)
            {
                CambiarDireccionPelota(&pelotaMenuPrincipal, (Direccion)VERTICAL);
                MoverPelota(&pelotaMenuPrincipal, (Direccion)VERTICAL);
            }
        }
        else if (escenaActual == (Escena)ESCENA_JUEGO)
        {
            Vector2 pelotaJuegoVirtualPosicion = {pelotaJuego.posicion.x + CalcularMovimiento(pelotaJuego.velocidad.x, pelotaJuego.aceleracion.x), pelotaJuego.posicion.y + CalcularMovimiento(pelotaJuego.velocidad.y, pelotaJuego.aceleracion.y)};

            // Colisión de la pelota con la paleta del jugador
            if (CheckCollisionCircleRec(pelotaJuegoVirtualPosicion, pelotaJuego.radio, ObtenerRectanguloPaleta(&jugadorPaleta)))
            {
                CambiarDireccionColisionRectPelota(&pelotaJuego, ObtenerRectanguloPaleta(&jugadorPaleta));
                PlaySound(colisionWav);
                // Cambia la aceleración de la pelota en base a la posición de la pelota en la paleta
                // Aceleración debe ser un valor entre 0 y 1
                float mitadPaleta = jugadorPaleta.posicion.x + jugadorPaleta.tamaño.x / 2;
                pelotaJuego.aceleracion.x = Clamp((abs(mitadPaleta - pelotaJuego.posicion.x) / (jugadorPaleta.tamaño.x / 2)) * 2.0f, 0.1f, 1.9f);
                pelotaJuego.aceleracion.y = Clamp(2.0f - pelotaJuego.aceleracion.x, 0.5f, 2.0f);
                if (pelotaJuego.posicion.x < mitadPaleta && pelotaJuego.velocidad.x > 0 || pelotaJuego.posicion.x > mitadPaleta && pelotaJuego.velocidad.x < 0)
                {
                    pelotaJuego.velocidad.x = -pelotaJuego.velocidad.x;
                }
            }

            // Colisiones de la pelota con los ladrillos
            for (int i = 0; i < ladrillosFilas; i++)
            {
                for (int j = 0; j < ladrillosColumnas; j++)
                {
                    if (ladrillosNivelActual[i * ladrillosColumnas + j].activo)
                    {
                        if (CheckCollisionCircleRec(pelotaJuegoVirtualPosicion, pelotaJuego.radio, ObtenerRectanguloLadrillo(&ladrillosNivelActual[i * ladrillosColumnas + j])))
                        {
                            ladrillosNivelActual[i * ladrillosColumnas + j].activo = false;
                            CambiarDireccionColisionRectPelota(&pelotaJuego, ObtenerRectanguloLadrillo(&ladrillosNivelActual[i * ladrillosColumnas + j]));
                            PlaySound(colisionWav);
                            puntosJugador += ladrillosNivelActual[i * ladrillosColumnas + j].puntos;
                            sprintf(puntosTexto, "%u", puntosJugador);
                            ladrillosRestantes--;
                        }
                    }
                }
            }

            if (pelotaJuego.posicion.x - pelotaJuego.radio <= 0.0f || pelotaJuego.posicion.x + pelotaJuego.radio >= juegoAncho)
            {
                CambiarDireccionPelota(&pelotaJuego, (Direccion)HORIZONTAL);
                MoverPelota(&pelotaJuego, (Direccion)HORIZONTAL);
            }
            if (pelotaJuego.posicion.y - pelotaJuego.radio <= 0.0f)
            {
                CambiarDireccionPelota(&pelotaJuego, (Direccion)VERTICAL);
                MoverPelota(&pelotaJuego, (Direccion)VERTICAL);
            }
            else if (pelotaJuego.posicion.y + pelotaJuego.radio >= juegoAlto)
            {
                PlaySound(explosionWav);
                vidasJugador--;
                haEmpezadoJuego = false;
                if (vidasJugador > 0)
                {
                    vaResetearJuego = true;
                }
                else
                {
                    vaRegresarMenuPrincipal = true;
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

            DrawRectangle(botonJugarRect.x - 16.0f, botonJugarRect.y - 16.0f, botonSalirRect.width + 32.0f, botonSalirJuegoRect.y - botonSalirJuegoRect.height - botonJugarRect.y + 32.0f, (Color){0xFF, 0x12, 0x12, 0x7F});

            if (GuiButton(botonJugarRect, "Jugar") || IsKeyPressed(KEY_SPACE) || IsKeyPressed(KEY_ENTER))
            {
                TraceLog(LOG_INFO, "JUGAR");
                PlaySound(seleccionWav);
                vaIniciarJuego = true;
            }
            if (GuiButton(botonLeaderboardRect, "Leaderboard") || IsKeyPressed(KEY_L))
            {
                TraceLog(LOG_INFO, "LEADERBOARD");
                PlaySound(seleccionWav);
            }
            if (GuiButton(botonSalirRect, "Salir") || IsKeyPressed(KEY_ESCAPE))
            {
                TraceLog(LOG_INFO, "SALIR");
                PlaySound(seleccionWav);
                break;
            }
        }
        else if (escenaActual == (Escena)ESCENA_JUEGO)
        {
            for (int i = 0; i < ladrillosFilas; i++)
            {
                for (int j = 0; j < ladrillosColumnas; j++)
                {
                    if (ladrillosNivelActual[i * ladrillosColumnas + j].activo)
                    {
                        DibujarLadrillo(&ladrillosNivelActual[i * ladrillosColumnas + j]);
                    }
                }
            }

            DibujarPelota(&pelotaJuego);
            DibujarPaleta(&jugadorPaleta);

            if (!haEmpezadoJuego)
            {
                if (GuiButton((Rectangle){mitadJuegoAncho - anchoBotonPausa / 2, mitadJuegoAlto - altoBotonPausa / 2, anchoBotonPausa, altoBotonPausa}, "Empezar") || IsKeyPressed(KEY_SPACE) || IsKeyPressed(KEY_ENTER))
                {
                    PlaySound(seleccionWav);
                    pelotaJuego.velocidad.x = 350.0f + 50.0f * nivelActual;
                    pelotaJuego.velocidad.y = pelotaJuego.velocidad.x;
                    pelotaJuego.aceleracion = VECTOR2_UNO;
                    if (rand() % 2 == 0)
                    {
                        CambiarDireccionPelota(&pelotaJuego, (Direccion)HORIZONTAL);
                    }
                    haEmpezadoJuego = true;
                }
            }
            else if (pausaJuego)
            {
                if (GuiButton(botonReanudarRect, "Reanudar") || IsKeyPressed(KEY_SPACE) || IsKeyPressed(KEY_ENTER))
                {
                    TraceLog(LOG_INFO, "REANUDAR");
                    PlaySound(seleccionWav);
                    pausaJuego = false;
                }
                if (GuiButton(botonSalirJuegoRect, "Salir") || IsKeyPressed(KEY_ESCAPE) && !acabaDePausar)
                {
                    TraceLog(LOG_INFO, "SALIR");
                    PlaySound(seleccionWav);
                    vaRegresarMenuPrincipal = true;
                }
            }

            // Corazones
            for (short i = 0; i < vidasJugador; i++)
            {
                DrawTextureEx(corazonTextura, (Vector2){8.0f + 64.0f * i, 8.0f}, 0.0f, 6.0f, WHITE);
            }

            DrawText(puntosTexto, juegoAncho - MeasureText(puntosTexto, 48) - 10, 10, 48, RAYWHITE);
            DrawText(nivelTexto, mitadJuegoAncho - MeasureText(nivelTexto, 64) / 2, juegoAlto - 84, 64, RAYWHITE);
        }

        EndTextureMode();

        //===============> Dibujo

        BeginDrawing();
        ClearBackground((Color){0x08, 0x08, 0x08, 0xFF});
        DrawTexturePro(juegoRenderTextura.texture, (Rectangle){0, 0, (float)juegoAncho, (float)-juegoAlto}, (Rectangle){(ventanaAncho - juegoAncho * juegoEscala) / 2, (ventanaAlto - juegoAlto * juegoEscala) / 2, juegoAncho * juegoEscala, juegoAlto * juegoEscala}, (Vector2){0, 0}, 0, WHITE);
#ifdef DEBUG
        DrawFPS(10, 10);
        DrawText(TextFormat("Mouse: (%.0f, %.0f)", mouseVirtual.x, mouseVirtual.y), 10, 40, 20, WHITE);
        if (escenaActual == (Escena)ESCENA_MENU_PRINCIPAL)
        {
            DrawText("MENU PRINCIPAL", 10, 70, 20, WHITE);
        }
        else if (escenaActual == (Escena)ESCENA_JUEGO)
        {
            DrawText("JUEGO", 10, 70, 20, WHITE);
            DrawText(TextFormat("Pelota Acel. (%.2f, %.2f)", pelotaJuego.aceleracion.x, pelotaJuego.aceleracion.y), 10, 100, 20, WHITE);
        }
#endif
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

        if (vaRegresarMenuPrincipal)
        {
            CambiarEscena(ESCENA_MENU_PRINCIPAL);
            GuiSetStyle((GuiControl)DEFAULT, (GuiDefaultProperty)TEXT_SIZE, 50);
            vaRegresarMenuPrincipal = false;
        }
        if (ladrillosRestantes == 0)
        {
            nivelActual++;
            vidasJugador++;
            sprintf(nivelTexto, "Nivel %u", nivelActual);
            ladrillosFilas = ladrillosFilasNiveles[nivelActual - 1];
            ladrillosColumnas = ladrillosColumnasNiveles[nivelActual - 1];
            ladrillosRestantes = ladrillosFilas * ladrillosColumnas;
            switch (nivelActual)
            {
            case 2:
                ladrillosNivelActual = (Ladrillo *)ladrillosNivel2;
                GenerarLadrillos((Ladrillo *)ladrillosNivel2, ladrillosFilasNiveles[1], ladrillosColumnasNiveles[1], 11.0f, (Color[4]){COLOR_APPLE_GREEN, COLOR_PIGMENT_GREEN, COLOR_CERULEAN, COLOR_PALATINATE_BLUE}, 56.0f);
                break;
            case 3:
                ladrillosNivelActual = (Ladrillo *)ladrillosNivel3;
                GenerarLadrillos((Ladrillo *)ladrillosNivel3, ladrillosFilasNiveles[2], ladrillosColumnasNiveles[2], 10.0f, (Color[5]){COLOR_COPPER, COLOR_APPLE_GREEN, COLOR_PIGMENT_GREEN, COLOR_CERULEAN, COLOR_PALATINATE_BLUE}, 64.0f);
                break;
            case 4:
                ladrillosNivelActual = (Ladrillo *)ladrillosNivel4;
                GenerarLadrillos((Ladrillo *)ladrillosNivel4, ladrillosFilasNiveles[3], ladrillosColumnasNiveles[3], 9.0f, (Color[6]){COLOR_COCOA_BROWN, COLOR_COPPER, COLOR_APPLE_GREEN, COLOR_PIGMENT_GREEN, COLOR_CERULEAN, COLOR_PALATINATE_BLUE}, 72.0f);
                break;
            case 5:
                ladrillosNivelActual = (Ladrillo *)ladrillosNivel5;
                GenerarLadrillos((Ladrillo *)ladrillosNivel5, ladrillosFilasNiveles[4], ladrillosColumnasNiveles[4], 8.0f, (Color[7]){COLOR_BITTERSWEET_SHIMMER, COLOR_COCOA_BROWN, COLOR_COPPER, COLOR_APPLE_GREEN, COLOR_PIGMENT_GREEN, COLOR_CERULEAN, COLOR_PALATINATE_BLUE}, 80.0f);
                break;
            }
            jugadorPaleta.tamaño.x = 300.0f - 40.0f * (nivelActual - 1);
            jugadorPaleta.velocidad = 600.0f + 60.0f * (nivelActual - 1);
            vaResetearJuego = true;
            haEmpezadoJuego = false;
        }
        if (vaIniciarJuego)
        {
            CambiarEscena(ESCENA_JUEGO);
            GuiSetStyle((GuiControl)DEFAULT, (GuiDefaultProperty)TEXT_SIZE, 70);
            ladrillosFilas = ladrillosFilasNiveles[0];
            ladrillosColumnas = ladrillosColumnasNiveles[0];
            jugadorPaleta.tamaño.x = 300.0f;
            jugadorPaleta.velocidad = 600.0f;
            nivelActual = 1;
            puntosJugador = 0;
            haEmpezadoJuego = false;
            pausaJuego = false;
#ifdef DEBUG
            vidasJugador = 12;
#else
            vidasJugador = 3;
#endif
            strcpy(puntosTexto, "0");
            strcpy(nivelTexto, "Nivel 1");
            ladrillosRestantes = ladrillosFilas * ladrillosColumnas;
            ladrillosNivelActual = (Ladrillo *)ladrillosNivel1;
            GenerarLadrillos((Ladrillo *)ladrillosNivel1, ladrillosFilasNiveles[0], ladrillosColumnasNiveles[0], 12.0f, (Color[3]){COLOR_PIGMENT_GREEN, COLOR_CERULEAN, COLOR_PALATINATE_BLUE}, 48.0f);
            vaResetearJuego = true;
            vaIniciarJuego = false;
        }
        if (vaResetearJuego)
        {
            pelotaJuego.posicion.x = (float)GetRandomValue(0, juegoAncho - pelotaJuego.radio * 2);
            pelotaJuego.posicion.y = 300.0f + nivelActual * 24.0f;
            vaResetearJuego = false;
        }
    }

    //===============> Cierre

    UnloadRenderTexture(juegoRenderTextura);
    UnloadTexture(corazonTextura);
    UnloadSound(colisionWav);
    UnloadSound(explosionWav);
    UnloadSound(seleccionWav);
    CloseAudioDevice();
    CloseWindow();
    return 0;
}

//==============================> Funciones

void CambiarEscena(Escena nueva_escena)
{
    escenaActual = nueva_escena;
}

void CambiarPantallaCompleta(int ventana_ancho, int ventana_alto)
{
    if (IsWindowFullscreen())
    {
        ToggleFullscreen();
        SetWindowSize(ventana_ancho, ventana_alto);
    }
    else
    {
        int monitor = GetCurrentMonitor();
        SetWindowSize(GetMonitorWidth(monitor), GetMonitorHeight(monitor));
        ToggleFullscreen();
    }
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
