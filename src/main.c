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

#define ARCHIVO_PUNTUACIONES "puntuaciones.dat"

//==============================> Tipos de datos

typedef unsigned char Byte;

//==============================> Enumeraciones

typedef enum
{
    ESCENA_MENU_PRINCIPAL = 0,
    ESCENA_JUEGO,
    ESCENA_LEADERBOARD
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
void AgregarPuntuacion(const char *archivo, const char *nombre, unsigned short tamaño, unsigned int puntos);
unsigned short CargarPuntuaciones(const char *archivo, char nombres[16][13], unsigned int *puntos);
void OrdenaPuntuaciones(char nombres[16][13], unsigned int *puntos, unsigned short cantidad);

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
    Sound poderWav = LoadSound("poder.wav");
    float volumenOffset = 0.0f;

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

    GuiSetStyle((GuiControl)TEXTBOX, (GuiControlProperty)TEXT_COLOR_NORMAL, ColorToInt(RAYWHITE));
    GuiSetStyle((GuiControl)TEXTBOX, (GuiControlProperty)BASE_COLOR_NORMAL, ColorToInt(DARKGRAY));
    GuiSetStyle((GuiControl)TEXTBOX, (GuiControlProperty)BORDER_COLOR_NORMAL, ColorToInt(RAYWHITE));

    GuiSetStyle((GuiControl)TEXTBOX, (GuiControlProperty)TEXT_COLOR_FOCUSED, ColorToInt(RAYWHITE));
    GuiSetStyle((GuiControl)TEXTBOX, (GuiControlProperty)BASE_COLOR_FOCUSED, ColorToInt(GRAY));
    GuiSetStyle((GuiControl)TEXTBOX, (GuiControlProperty)BORDER_COLOR_FOCUSED, ColorToInt(RAYWHITE));

    GuiSetStyle((GuiControl)TEXTBOX, (GuiControlProperty)TEXT_COLOR_PRESSED, ColorToInt(DARKGRAY));
    GuiSetStyle((GuiControl)TEXTBOX, (GuiControlProperty)BASE_COLOR_PRESSED, ColorToInt(LIGHTGRAY));
    GuiSetStyle((GuiControl)TEXTBOX, (GuiControlProperty)BORDER_COLOR_PRESSED, ColorToInt(DARKGRAY));

    // GuiSetStyle((GuiControl)TEXTBOX, (GuiControlProperty)TEXT_SIZE, 50);
    GuiSetStyle((GuiControl)TEXTBOX, (GuiControlProperty)TEXT_PADDING, 16);

    GuiSetIconScale(3);

    //===============> Variables locales a la función principal

#ifdef DEBUG
    bool modoAutomatico = false;
    float modoAutomaticoTextoTransparencia = 1.0f;
    float modoAutomaticoTextoTransparenciaMult = 1.0f;
    bool muestraDatosDebug = false;
#endif

    RenderTexture2D juegoRenderTextura = LoadRenderTexture(juegoAncho, juegoAlto);
    SetTextureFilter(juegoRenderTextura.texture, (TextureFilter)TEXTURE_FILTER_BILINEAR);

    //=====> Fondo

    float fondoCeldaTamaño = MAX(juegoAncho, juegoAlto) / 24.0f;
    Image fondoImagen = GenImageChecked(juegoAncho, juegoAlto + fondoCeldaTamaño * 3, fondoCeldaTamaño, fondoCeldaTamaño, (Color){0x18, 0x18, 0x18, 0xFF}, (Color){0x30, 0x30, 0x30, 0xFF});
    Texture2D fondoTextura = LoadTextureFromImage(fondoImagen);
    UnloadImage(fondoImagen);
    float fondoDesplazamientoVertical = 0.0f;
    Color fondoModuloColor = Fade(WHITE, 0.5f);

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
    float obstaculoMenuPrincipal1Interpolacion = 0.0f;
    float obstaculoMenuPrincipal1InterpolacionMult = 0.4f;
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
    bool perdioJuego = false;
    bool ganoJuego = false;
    char nombreJugador[13] = {0};

    char puntuacionesNombres[16][13] = {0};
    unsigned int puntuacionesPuntos[16] = {0};
    unsigned short puntuacionesCantidad = 0;

    //=====> Botones

    const int anchoBotonPausa = 480;
    const int altoBotonPausa = 120;
    const int margenBotonPausa = 160;
    const int separacionBotonPausa = 20;

    const Rectangle botonReanudarRect = {mitadJuegoAncho - anchoBotonPausa / 2, mitadJuegoAlto - altoBotonPausa - separacionBotonPausa / 2, anchoBotonPausa, altoBotonPausa};
    const Rectangle botonSalirJuegoRect = {mitadJuegoAncho - anchoBotonPausa / 2, mitadJuegoAlto + separacionBotonPausa / 2, anchoBotonPausa, altoBotonPausa};

    const Rectangle botonGuardarPuntosRect = {mitadJuegoAncho - anchoBotonPausa / 2, 470.0f, anchoBotonPausa, altoBotonPausa};
    const Rectangle botonReiniciarOverRect = {mitadJuegoAncho - anchoBotonPausa / 2, 480.0f + margenBotonPausa, anchoBotonPausa, altoBotonPausa};
    const Rectangle botonSalirOverRect = {mitadJuegoAncho - anchoBotonPausa / 2, 480.0f + margenBotonPausa * 2, anchoBotonPausa, altoBotonPausa};

    //=====> Pelota

    Pelota pelotaJuego = (Pelota){VECTOR2_CERO, VECTOR2_CERO, VECTOR2_UNO, 20.0f, (Color){0xC8, 0xC8, 0xC8, 0xFF}};

    //=====> Paletas

    Paleta jugadorPaleta = (Paleta){(Vector2){mitadJuegoAncho, juegoAlto - 140.0f}, (Vector2){20.0f, 20.0f}, 0.0f, 4, 2.0f, (Color){186, 0xFF, 0xFF, 0xFF}};
    Paleta cpuPaleta = (Paleta){(Vector2){mitadJuegoAncho, 375.0f}, (Vector2){100.0f, 20.0f}, 200.0f, 4, 2.0f, (Color){0xFF, 186, 186, 0xFF}};

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

    //=====> Obstáculos

    Obstaculo obstaculoNivel2 = (Obstaculo){(Vector2){mitadJuegoAncho - 75.0f, mitadJuegoAlto - 25.0f}, (Vector2){150.0f, 50.0f}, BLACK, WHITE, 2.0f, 0.0f};

    Obstaculo obstaculosNivel3[2];
    obstaculosNivel3[0] = (Obstaculo){(Vector2){200.0f, 575.0f}, (Vector2){50.0f, 150.0f}, BLACK, WHITE, 2.0f, 0.0f};
    obstaculosNivel3[1] = (Obstaculo){(Vector2){juegoAncho - 450.0f, 500.0f}, (Vector2){100.0f, 100.0f}, BLACK, WHITE, 2.0f, 0.0f};

    Obstaculo obstaculosNivel4[3];
    obstaculosNivel4[0] = (Obstaculo){(Vector2){144.0f, 525.0f}, (Vector2){100.0f, 100.0f}, BLACK, WHITE, 2.0f, 0.0f};
    obstaculosNivel4[1] = (Obstaculo){(Vector2){mitadJuegoAncho - 30.0f, 675.0f}, (Vector2){60.0f, 250.0f}, BLACK, WHITE, 2.0f, 0.0f};
    obstaculosNivel4[2] = (Obstaculo){(Vector2){juegoAncho - 325.0f, 400.0f}, (Vector2){200.0f, 50.0f}, BLACK, WHITE, 2.0f, 0.0f};
    float obstaculo2Nivel4Interpolacion = 0.0f;
    float obstaculo2Nivel4InterpolacionMult = 0.3f;

    Obstaculo obstaculosNivel5[3];
    obstaculosNivel5[0] = (Obstaculo){(Vector2){-350.0f, 500.0f}, (Vector2){200.0f, 30.0f}, BLACK, WHITE, 2.0f, 0.0f};
    obstaculosNivel5[1] = (Obstaculo){(Vector2){mitadJuegoAncho - 37.5f, mitadJuegoAlto + 100.0f}, (Vector2){75.0f, 75.0f}, BLACK, WHITE, 2.0f, 0.0f};
    obstaculosNivel5[2] = (Obstaculo){(Vector2){juegoAncho + 350.0f, 800.0f}, (Vector2){200.0f, 30.0f}, BLACK, WHITE, 2.0f, 0.0f};
    float obstaculo0Nivel5Interpolacion = 0.0f;
    float obstaculo2Nivel5Interpolacion = 0.0f;

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

        fondoDesplazamientoVertical += 50.0f * GetFrameTime();
        if (fondoDesplazamientoVertical >= fondoCeldaTamaño * 2.0f)
        {
            fondoDesplazamientoVertical -= fondoCeldaTamaño * 2.0f;
        }

        if (IsKeyPressed(KEY_ENTER) && (IsKeyDown(KEY_LEFT_ALT) || IsKeyDown(KEY_RIGHT_ALT)) || IsKeyPressed(KEY_F12))
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

#ifdef DEBUG
        if ((IsKeyPressed(KEY_D) && (IsKeyDown(KEY_LEFT_CONTROL)) || IsKeyDown(KEY_F11)))
        {
            muestraDatosDebug = !muestraDatosDebug;
        }
#endif

        if (IsKeyPressed(KEY_EQUAL))
        {
            SetMasterVolume(MIN(GetMasterVolume() + 0.1f, 1.0f));
            volumenOffset = 100.0f;
        }
        else if (IsKeyPressed(KEY_MINUS))
        {
            SetMasterVolume(MAX(GetMasterVolume() - 0.1f, 0.0f));
            volumenOffset = 100.0f;
        }
        volumenOffset = MAX(0.0f, volumenOffset - GetFrameTime() * 75.0f);

        if (escenaActual == (Escena)ESCENA_MENU_PRINCIPAL)
        {
            ActualizarPelota(&pelotaMenuPrincipal);
            ActualizarCPUPaleta(&cpuPaletaMenuPrincipal, pelotaMenuPrincipal.posicion);
            ActualizarCPUPaleta(&jugadorPaletaMenuPrincipal, pelotaMenuPrincipal.posicion);
            obstaculoMenuPrincipal1Interpolacion += obstaculoMenuPrincipal1InterpolacionMult * GetFrameTime();
            if (obstaculoMenuPrincipal1Interpolacion >= 1.0f)
            {
                obstaculoMenuPrincipal1Interpolacion = 1.0f;
                obstaculoMenuPrincipal1InterpolacionMult *= -1.0f;
            }
            else if (obstaculoMenuPrincipal1Interpolacion <= 0.0f)
            {
                obstaculoMenuPrincipal1Interpolacion = 0.0f;
                obstaculoMenuPrincipal1InterpolacionMult *= -1.0f;
            }
            MoverObstaculo(&obstaculoMenuPrincipal[1], (Vector2){574.0f, 524.0f}, (Vector2){574.0f, 824.0f}, obstaculoMenuPrincipal1Interpolacion);
        }
        else if (escenaActual == (Escena)ESCENA_JUEGO)
        {
            if (!pausaJuego && !perdioJuego && !ganoJuego)
            {
                if (haEmpezadoJuego)
                {
                    ActualizarPelota(&pelotaJuego);
                    if (nivelActual == 5)
                    {
                        ActualizarCPUPaleta(&cpuPaleta, pelotaJuego.posicion);
                    }
                }
                ActualizarPaleta(&jugadorPaleta);
                if (nivelActual == 4)
                {
                    obstaculo2Nivel4Interpolacion += obstaculo2Nivel4InterpolacionMult * GetFrameTime();
                    if (obstaculo2Nivel4Interpolacion >= 1.0f)
                    {
                        obstaculo2Nivel4Interpolacion = 1.0f;
                        obstaculo2Nivel4InterpolacionMult *= -1.0f;
                    }
                    else if (obstaculo2Nivel4Interpolacion <= 0.0f)
                    {
                        obstaculo2Nivel4Interpolacion = 0.0f;
                        obstaculo2Nivel4InterpolacionMult *= -1.0f;
                    }
                    MoverObstaculo(&obstaculosNivel4[2], (Vector2){juegoAncho - 325.0f, 500.0f}, (Vector2){juegoAncho - 450.0f, 500.0f}, obstaculo2Nivel4Interpolacion);
                }
                if (nivelActual == 5)
                {
                    obstaculo0Nivel5Interpolacion += 0.1f * GetFrameTime();
                    if (obstaculo0Nivel5Interpolacion >= 1.0f)
                    {
                        obstaculo0Nivel5Interpolacion = 0.0f;
                    }
                    MoverObstaculo(&obstaculosNivel5[0], (Vector2){-550.0f, 500.0f}, (Vector2){juegoAncho + 350.0f, 500.0f}, obstaculo0Nivel5Interpolacion);
                    obstaculo2Nivel5Interpolacion += 0.1f * GetFrameTime();
                    if (obstaculo2Nivel5Interpolacion >= 1.0f)
                    {
                        obstaculo2Nivel5Interpolacion = 0.0f;
                    }
                    MoverObstaculo(&obstaculosNivel5[2], (Vector2){juegoAncho + 350.0f, 800.0f}, (Vector2){-550.0f, 800.0f}, obstaculo2Nivel5Interpolacion);
                }
            }
#ifdef DEBUG
            if (IsKeyPressed(KEY_TAB))
            {
                modoAutomatico = !modoAutomatico;
            }
            if (IsKeyPressed(KEY_KP_SUBTRACT))
            {
                ladrillosRestantes = 0;
            }
            if (IsKeyPressed(KEY_KP_ADD))
            {
                vidasJugador++;
                if (vidasJugador > 7)
                {
                    vidasJugador = 7;
                }
                else
                {
                    PlaySound(poderWav);
                }
            }
            if (modoAutomatico)
            {
                modoAutomaticoTextoTransparencia += modoAutomaticoTextoTransparenciaMult * GetFrameTime();
                if (modoAutomaticoTextoTransparencia >= 1.0f)
                {
                    modoAutomaticoTextoTransparencia = 1.0f;
                    modoAutomaticoTextoTransparenciaMult = -1.0f;
                }
                if (modoAutomaticoTextoTransparencia <= 0.0f)
                {
                    modoAutomaticoTextoTransparencia = 0.0f;
                    modoAutomaticoTextoTransparenciaMult = 1.0f;
                }
                if (IsKeyPressed(KEY_LEFT))
                {
                    jugadorPaleta.posicion.x -= CalcularMovimiento(jugadorPaleta.velocidad, 2.0f);
                }
                else if (IsKeyPressed(KEY_RIGHT))
                {
                    jugadorPaleta.posicion.x += CalcularMovimiento(jugadorPaleta.velocidad, 2.0f);
                }
                else
                {
                    ActualizarCPUPaleta(&jugadorPaleta, pelotaJuego.posicion);
                }
            }
#endif
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
                    if (!IsSoundPlaying(colisionWav))
                    {
                        PlaySound(colisionWav);
                    }
                }
                ActualizarObstaculo(&obstaculoMenuPrincipal[i]);
            }

            if (pelotaMenuPrincipal.posicion.x - pelotaMenuPrincipal.radio <= 0.0f || pelotaMenuPrincipal.posicion.x + pelotaMenuPrincipal.radio >= juegoAncho)
            {
                CambiarDireccionPelota(&pelotaMenuPrincipal, (Direccion)HORIZONTAL);
                MoverPelota(&pelotaMenuPrincipal, (Direccion)HORIZONTAL);
            }
            if (pelotaMenuPrincipal.posicion.y - pelotaMenuPrincipal.radio <= 0.0f)
            {
                CambiarDireccionPelota(&pelotaMenuPrincipal, (Direccion)VERTICAL);
                MoverPelota(&pelotaMenuPrincipal, (Direccion)VERTICAL);
            }
            else if (pelotaMenuPrincipal.posicion.y + pelotaMenuPrincipal.radio >= juegoAlto)
            {
                pelotaMenuPrincipal.posicion = (Vector2){(float)GetRandomValue(0, juegoAncho - 20.0f), 275.0f};
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

            // Colisión de la pelota con la paleta enemiga
            if (nivelActual == 5)
            {
                if (pelotaJuego.velocidad.y < 0)
                {
                    if (CheckCollisionCircleRec(pelotaJuegoVirtualPosicion, pelotaJuego.radio, ObtenerRectanguloPaleta(&cpuPaleta)))
                    {
                        CambiarDireccionColisionRectPelota(&pelotaJuego, ObtenerRectanguloPaleta(&cpuPaleta));
                        PlaySound(colisionWav);
                    }
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

            // Colisiones de la pelota con los obstáculos estáticos
            switch (nivelActual)
            {
            case 2:
                if (CheckCollisionCircleRec(pelotaJuegoVirtualPosicion, pelotaJuego.radio, ObtenerRectanguloObstaculo(&obstaculoNivel2)))
                {
                    CambiarDireccionColisionRectPelota(&pelotaJuego, ObtenerRectanguloObstaculo(&obstaculoNivel2));
                    obstaculoNivel2.modulo = 1.0f;
                    if (!IsSoundPlaying(colisionWav))
                    {
                        PlaySound(colisionWav);
                    }
                }
                ActualizarObstaculo(&obstaculoNivel2);
                break;
            case 3:
                for (int i = 0; i < 2; i++)
                {
                    if (CheckCollisionCircleRec(pelotaJuegoVirtualPosicion, pelotaJuego.radio, ObtenerRectanguloObstaculo(&obstaculosNivel3[i])))
                    {
                        CambiarDireccionColisionRectPelota(&pelotaJuego, ObtenerRectanguloObstaculo(&obstaculosNivel3[i]));
                        obstaculosNivel3[i].modulo = 1.0f;
                        if (!IsSoundPlaying(colisionWav))
                        {
                            PlaySound(colisionWav);
                        }
                    }
                    ActualizarObstaculo(&obstaculosNivel3[i]);
                }
                break;
            case 4:
                for (int i = 0; i < 3; i++)
                {
                    if (CheckCollisionCircleRec(pelotaJuegoVirtualPosicion, pelotaJuego.radio, ObtenerRectanguloObstaculo(&obstaculosNivel4[i])))
                    {
                        CambiarDireccionColisionRectPelota(&pelotaJuego, ObtenerRectanguloObstaculo(&obstaculosNivel4[i]));
                        obstaculosNivel4[i].modulo = 1.0f;
                        if (!IsSoundPlaying(colisionWav))
                        {
                            PlaySound(colisionWav);
                        }
                    }
                    ActualizarObstaculo(&obstaculosNivel4[i]);
                }
                break;
            case 5:
                for (int i = 0; i < 3; i++)
                {
                    if (CheckCollisionCircleRec(pelotaJuegoVirtualPosicion, pelotaJuego.radio, ObtenerRectanguloObstaculo(&obstaculosNivel5[i])))
                    {
                        CambiarDireccionColisionRectPelota(&pelotaJuego, ObtenerRectanguloObstaculo(&obstaculosNivel5[i]));
                        obstaculosNivel5[i].modulo = 1.0f;
                        if (!IsSoundPlaying(colisionWav))
                        {
                            PlaySound(colisionWav);
                        }
                    }
                    ActualizarObstaculo(&obstaculosNivel5[i]);
                }
                break;
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
            else if (pelotaJuego.posicion.y + pelotaJuego.radio >= juegoAlto && !perdioJuego && !ganoJuego)
            {
                PlaySound(explosionWav);
                vidasJugador--;
                if (vidasJugador > 0)
                {
                    haEmpezadoJuego = false;
                    vaResetearJuego = true;
                }
                else
                {
                    perdioJuego = true;
                }
            }
        }

        //===============> Renderización

        BeginTextureMode(juegoRenderTextura);

        ClearBackground(COLOR_EERIE_BLACK);

        DrawTextureEx(fondoTextura, (Vector2){0.0f, -fondoDesplazamientoVertical}, 0.0f, 1.0f, Fade(fondoModuloColor, 1.0f));

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

            // DrawRectangle(botonJugarRect.x - 16.0f, botonJugarRect.y - 16.0f, botonSalirRect.width + 32.0f, botonSalirJuegoRect.y - botonSalirJuegoRect.height - botonJugarRect.y + 32.0f, (Color){0xFF, 0x12, 0x12, 0x7F});

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
                puntuacionesCantidad = CargarPuntuaciones(ARCHIVO_PUNTUACIONES, puntuacionesNombres, puntuacionesPuntos);
                OrdenaPuntuaciones(puntuacionesNombres, puntuacionesPuntos, puntuacionesCantidad);
                CambiarEscena((Escena)ESCENA_LEADERBOARD);
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

            switch (nivelActual)
            {
            case 2:
                DibujarObstaculo(&obstaculoNivel2);
                break;
            case 3:
                for (int i = 0; i < 2; i++)
                {
                    DibujarObstaculo(&obstaculosNivel3[i]);
                }
                break;
            case 4:
                for (int i = 0; i < 3; i++)
                {
                    DibujarObstaculo(&obstaculosNivel4[i]);
                }
                break;
            case 5:
                DibujarPaleta(&cpuPaleta);
                for (int i = 0; i < 3; i++)
                {
                    DibujarObstaculo(&obstaculosNivel5[i]);
                }
                break;
            }

            // Corazones
            for (short i = 0; i < vidasJugador; i++)
            {
                DrawTextureEx(corazonTextura, (Vector2){8.0f + 64.0f * i, 8.0f}, 0.0f, 6.0f, WHITE);
            }

            DrawText(puntosTexto, juegoAncho - MeasureText(puntosTexto, 48) - 10, 10, 48, RAYWHITE);
            DrawText(nivelTexto, mitadJuegoAncho - MeasureText(nivelTexto, 64) / 2, juegoAlto - 84, 64, RAYWHITE);

#ifdef DEBUG
            if (modoAutomatico)
            {
                DrawText("MODO AUTOMÁTICO", mitadJuegoAncho - MeasureText("MODO AUTOMÁTICO", 48) / 2, mitadJuegoAlto + 64, 48, Fade(RAYWHITE, modoAutomaticoTextoTransparencia));
            }
#endif

            if (!haEmpezadoJuego)
            {
                if (GuiButton((Rectangle){mitadJuegoAncho - anchoBotonPausa / 2, mitadJuegoAlto - altoBotonPausa / 2, anchoBotonPausa, altoBotonPausa}, "Empezar") || IsKeyPressed(KEY_SPACE) || IsKeyPressed(KEY_ENTER))
                {
                    PlaySound(seleccionWav);
                    pelotaJuego.velocidad.x = 350.0f + 25.0f * nivelActual;
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
                DrawRectangle(0, 0, juegoAncho, juegoAlto, (Color){0x00, 0x00, 0x00, 0x7F});
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
            else if (perdioJuego)
            {
                DrawRectangle(0, 0, juegoAncho, juegoAlto, (Color){0x00, 0x00, 0x00, 0x7F});
                DrawText("GAME OVER", mitadJuegoAncho - MeasureText("GAME OVER", 160) / 2, mitadJuegoAlto - 340, 160, WHITE);
                if (puntosJugador > 0)
                {
                    GuiSetStyle((GuiControl)DEFAULT, (GuiDefaultProperty)TEXT_SIZE, 55);
                    if (GuiTextBox(botonGuardarPuntosRect, nombreJugador, 12, true))
                    {
                        if (strcmp(nombreJugador, "") != 0)
                        {
                            AgregarPuntuacion(ARCHIVO_PUNTUACIONES, nombreJugador, 12, puntosJugador);
                            TraceLog(LOG_INFO, "PUNTUACIÓN GUARDADA");
                            PlaySound(seleccionWav);
                            vaRegresarMenuPrincipal = true;
                            perdioJuego = false;
                        }
                    }
                    GuiSetStyle((GuiControl)DEFAULT, (GuiDefaultProperty)TEXT_SIZE, 70);
                    DrawText(TextFormat("Puntos: %u", puntosJugador), mitadJuegoAncho - MeasureText(TextFormat("Puntos: %u", puntosJugador), 64) / 2, mitadJuegoAlto - 170, 64, WHITE);
                }
                if (GuiButton(botonReiniciarOverRect, "Reiniciar"))
                {
                    TraceLog(LOG_INFO, "REINICIAR");
                    PlaySound(seleccionWav);
                    vaIniciarJuego = true;
                    perdioJuego = false;
                }
                if (GuiButton(botonSalirOverRect, "Salir") || IsKeyPressed(KEY_ESCAPE) && !acabaDePausar)
                {
                    TraceLog(LOG_INFO, "SALIR");
                    PlaySound(seleccionWav);
                    vaRegresarMenuPrincipal = true;
                    perdioJuego = false;
                }
            }
            else if (ganoJuego)
            {
                DrawRectangle(0, 0, juegoAncho, juegoAlto, (Color){0x00, 0x00, 0x00, 0x7F});
                DrawText("¡FELICIDADES!", mitadJuegoAncho - MeasureText("¡FELICIDADES!", 160) / 2, 200, 160, WHITE);
                GuiSetStyle((GuiControl)DEFAULT, (GuiDefaultProperty)TEXT_SIZE, 55);
                if (GuiTextBox(botonGuardarPuntosRect, nombreJugador, 12, true))
                {
                    if (strcmp(nombreJugador, "") != 0)
                    {
                        AgregarPuntuacion(ARCHIVO_PUNTUACIONES, nombreJugador, 12, puntosJugador);
                        TraceLog(LOG_INFO, "PUNTUACIÓN GUARDADA");
                        PlaySound(seleccionWav);
                        vaRegresarMenuPrincipal = true;
                        perdioJuego = false;
                    }
                }
                DrawText(TextFormat("Puntos: %u", puntosJugador), mitadJuegoAncho - MeasureText(TextFormat("Puntos: %u", puntosJugador), 64) / 2, mitadJuegoAlto - 170, 64, WHITE);
                if (GuiButton(botonLeaderboardRect, "Reiniciar"))
                {
                    PlaySound(seleccionWav);
                    vaIniciarJuego = true;
                    ganoJuego = false;
                }
                if (GuiButton(botonSalirRect, "Salir") || IsKeyPressed(KEY_ESCAPE) && !acabaDePausar)
                {
                    TraceLog(LOG_INFO, "SALIR");
                    PlaySound(seleccionWav);
                    vaRegresarMenuPrincipal = true;
                    ganoJuego = false;
                }
                GuiSetStyle((GuiControl)DEFAULT, (GuiDefaultProperty)TEXT_SIZE, 70);
            }
        }
        else if (escenaActual == (Escena)ESCENA_LEADERBOARD)
        {
            DrawRectangle(16, 16, juegoAncho - 32, juegoAlto - 32, (Color){0x00, 0x00, 0x00, 0x7F});
            if (puntuacionesCantidad == 0)
            {
                DrawText("No hay puntuaciones", mitadJuegoAncho - MeasureText("No hay puntuaciones", 64) / 2, mitadJuegoAlto - 32, 64, WHITE);
            }
            else
            {
                DrawText("Leaderboard", mitadJuegoAncho - MeasureText("Leaderboard", 64) / 2, 32, 64, WHITE);
                DrawLineEx((Vector2){64, 116}, (Vector2){juegoAncho - 64, 116}, 4.0f, WHITE);
                for (int i = 0; i < puntuacionesCantidad; i++)
                {
                    DrawText(TextFormat("%s: %u", puntuacionesNombres[i], puntuacionesPuntos[i]), mitadJuegoAncho - MeasureText(TextFormat("%s: %u", puntuacionesNombres[i], puntuacionesPuntos[i]), 48) / 2, 144 + 64 * i, 48, WHITE);
                }
            }
            if (GuiButton((Rectangle){juegoAncho - anchoBotonMenuPrincipal - 32, juegoAlto - altoBotonMenuPrincipal - 32, anchoBotonMenuPrincipal, altoBotonMenuPrincipal}, "REGRESAR") || IsKeyPressed(KEY_ESCAPE))
            {
                TraceLog(LOG_INFO, "REGRESAR A MENU PRINCIPAL");
                PlaySound(seleccionWav);
                vaRegresarMenuPrincipal = true;
            }
        }

        EndTextureMode();

        //===============> Dibujo

        BeginDrawing();
        ClearBackground((Color){0x08, 0x08, 0x08, 0xFF});
        DrawTexturePro(juegoRenderTextura.texture, (Rectangle){0, 0, (float)juegoAncho, (float)-juegoAlto}, (Rectangle){(ventanaAncho - juegoAncho * juegoEscala) / 2, (ventanaAlto - juegoAlto * juegoEscala) / 2, juegoAncho * juegoEscala, juegoAlto * juegoEscala}, (Vector2){0, 0}, 0, WHITE);
#ifdef DEBUG
        if (muestraDatosDebug)
        {
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
        }
#endif
        DrawText(TextFormat("Volumen: %d%%", (int)(GetMasterVolume() * 100.0f)), ventanaAncho - 225, ventanaAlto - MIN(volumenOffset, 60.0f) + 20.0f, 30, WHITE);
        EndDrawing();

        //===============> Lógica después de dibujar

        if (escenaActual == (Escena)ESCENA_JUEGO)
        {
            if (haEmpezadoJuego && !perdioJuego)
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
            fondoModuloColor = Fade(WHITE, 0.5f);
            pelotaMenuPrincipal.posicion.x = (float)GetRandomValue(0, juegoAncho - 20.0f);
            pelotaMenuPrincipal.posicion.y = 275.0f;
            pelotaMenuPrincipal.velocidad.x = 300.0f;
            pelotaMenuPrincipal.velocidad.y = (float)GetRandomValue(200, 350);
            vaRegresarMenuPrincipal = false;
        }
        if (ladrillosRestantes == 0 && !perdioJuego && !ganoJuego)
        {
            nivelActual++;
            if (nivelActual <= 5)
            {
                vidasJugador++;
#ifdef DEBUG
                if (vidasJugador > 7)
                {
                    vidasJugador = 7;
                }
#endif
                puntosJugador += 1000 * (nivelActual - 1);
                PlaySound(poderWav);
                sprintf(nivelTexto, "Nivel %u", nivelActual);
                sprintf(puntosTexto, "%u", puntosJugador);
                ladrillosFilas = ladrillosFilasNiveles[nivelActual - 1];
                ladrillosColumnas = ladrillosColumnasNiveles[nivelActual - 1];
                ladrillosRestantes = ladrillosFilas * ladrillosColumnas;
                switch (nivelActual)
                {
                case 2:
                    ladrillosNivelActual = (Ladrillo *)ladrillosNivel2;
                    GenerarLadrillos((Ladrillo *)ladrillosNivel2, ladrillosFilasNiveles[1], ladrillosColumnasNiveles[1], 11.0f, (Color[4]){COLOR_APPLE_GREEN, COLOR_PIGMENT_GREEN, COLOR_CERULEAN, COLOR_PALATINATE_BLUE}, 56.0f);
                    fondoModuloColor = ColorBrightness(COLOR_PIGMENT_GREEN, 0.5f);
                    break;
                case 3:
                    ladrillosNivelActual = (Ladrillo *)ladrillosNivel3;
                    GenerarLadrillos((Ladrillo *)ladrillosNivel3, ladrillosFilasNiveles[2], ladrillosColumnasNiveles[2], 10.0f, (Color[5]){COLOR_COPPER, COLOR_APPLE_GREEN, COLOR_PIGMENT_GREEN, COLOR_CERULEAN, COLOR_PALATINATE_BLUE}, 64.0f);
                    fondoModuloColor = ColorBrightness(COLOR_COPPER, 0.5f);
                    break;
                case 4:
                    ladrillosNivelActual = (Ladrillo *)ladrillosNivel4;
                    GenerarLadrillos((Ladrillo *)ladrillosNivel4, ladrillosFilasNiveles[3], ladrillosColumnasNiveles[3], 9.0f, (Color[6]){COLOR_COCOA_BROWN, COLOR_COPPER, COLOR_APPLE_GREEN, COLOR_PIGMENT_GREEN, COLOR_CERULEAN, COLOR_PALATINATE_BLUE}, 72.0f);
                    fondoModuloColor = ColorBrightness(COLOR_COCOA_BROWN, 0.5f);
                    break;
                case 5:
                    ladrillosNivelActual = (Ladrillo *)ladrillosNivel5;
                    GenerarLadrillos((Ladrillo *)ladrillosNivel5, ladrillosFilasNiveles[4], ladrillosColumnasNiveles[4], 8.0f, (Color[7]){COLOR_BITTERSWEET_SHIMMER, COLOR_COCOA_BROWN, COLOR_COPPER, COLOR_APPLE_GREEN, COLOR_PIGMENT_GREEN, COLOR_CERULEAN, COLOR_PALATINATE_BLUE}, 80.0f);
                    fondoModuloColor = ColorBrightness(COLOR_BITTERSWEET_SHIMMER, 0.5f);
                    break;
                }
                jugadorPaleta.tamaño.x = 300.0f - 30.0f * (nivelActual - 1);
                jugadorPaleta.velocidad = 600.0f + 75.0f * (nivelActual - 1);
                vaResetearJuego = true;
                haEmpezadoJuego = false;
            }
            else
            {
                puntosJugador += 10000;
                sprintf(puntosTexto, "%u", puntosJugador);
                ganoJuego = true;
            }
        }
        if (vaIniciarJuego)
        {
            CambiarEscena(ESCENA_JUEGO);
            GuiSetStyle((GuiControl)DEFAULT, (GuiDefaultProperty)TEXT_SIZE, 70);
            fondoModuloColor = ColorBrightness(COLOR_CERULEAN, 0.5f);
            ladrillosFilas = ladrillosFilasNiveles[0];
            ladrillosColumnas = ladrillosColumnasNiveles[0];
            jugadorPaleta.tamaño.x = 300.0f;
            jugadorPaleta.velocidad = 600.0f;
            jugadorPaleta.posicion.x = mitadJuegoAncho - jugadorPaleta.tamaño.x / 2;
            nivelActual = 1;
            puntosJugador = 0;
            haEmpezadoJuego = false;
            pausaJuego = false;
            vidasJugador = 3;
            perdioJuego = false;
            ganoJuego = false;
            strcpy(puntosTexto, "0");
            strcpy(nivelTexto, "Nivel 1");
            ladrillosRestantes = ladrillosFilas * ladrillosColumnas;
            ladrillosNivelActual = (Ladrillo *)ladrillosNivel1;
            GenerarLadrillos((Ladrillo *)ladrillosNivel1, ladrillosFilasNiveles[0], ladrillosColumnasNiveles[0], 12.0f, (Color[3]){COLOR_PIGMENT_GREEN, COLOR_CERULEAN, COLOR_PALATINATE_BLUE}, 48.0f);
#ifdef DEBUG
            modoAutomatico = false;
            modoAutomaticoTextoTransparencia = 0.0f;
            modoAutomaticoTextoTransparenciaMult = 1.0f;
#endif
            vaResetearJuego = true;
            vaIniciarJuego = false;
        }
        if (vaResetearJuego)
        {
            pelotaJuego.posicion.x = (float)GetRandomValue(pelotaJuego.radio * 2, juegoAncho - pelotaJuego.radio * 4);
            pelotaJuego.posicion.y = 300.0f + nivelActual * 24.0f;
            vaResetearJuego = false;
        }
    }

    //===============> Cierre

    UnloadRenderTexture(juegoRenderTextura);
    UnloadTexture(corazonTextura);
    UnloadTexture(fondoTextura);
    UnloadSound(colisionWav);
    UnloadSound(explosionWav);
    UnloadSound(seleccionWav);
    UnloadSound(poderWav);
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

void AgregarPuntuacion(const char *archivo, const char *nombre, unsigned short tamaño, unsigned int puntos)
{
    FILE *archivo_puntuaciones = fopen(archivo, "ab");
    fwrite(nombre, sizeof(char), tamaño, archivo_puntuaciones);
    fwrite(&puntos, sizeof(unsigned int), 1, archivo_puntuaciones);
    fclose(archivo_puntuaciones);
}

unsigned short CargarPuntuaciones(const char *archivo, char nombres[16][13], unsigned int *puntos)
{
    FILE *archivo_puntuaciones = fopen(archivo, "rb");
    unsigned short cantidad_leida = 0;
    if (archivo_puntuaciones != NULL)
    {
        for (unsigned short i = 0; i < 16; i++)
        {
            if (fread(nombres[i], sizeof(char), 12, archivo_puntuaciones) == 12 &&
                fread(&puntos[i], sizeof(unsigned int), 1, archivo_puntuaciones) == 1)
            {
                cantidad_leida++;
            }
            else
            {
                break;
            }
        }
        fclose(archivo_puntuaciones);
    }
    return cantidad_leida;
}

void OrdenaPuntuaciones(char nombres[16][13], unsigned int *puntos, unsigned short cantidad)
{
    for (unsigned short i = 0; i < cantidad; i++)
    {
        for (unsigned short j = i + 1; j < cantidad; j++)
        {
            if (puntos[j] > puntos[i])
            {
                unsigned int puntos_temp = puntos[i];
                puntos[i] = puntos[j];
                puntos[j] = puntos_temp;
                char nombre_temp[13];
                strcpy(nombre_temp, nombres[i]);
                strcpy(nombres[i], nombres[j]);
                strcpy(nombres[j], nombre_temp);
            }
        }
    }
}
