/*******************************************************************************************
*
*   raylib game template
*
*   <Game title>
*   <Game description>
*
*   This game has been created using raylib (www.raylib.com)
*   raylib is licensed under an unmodified zlib/libpng license (View raylib.h for details)
*
*   Copyright (c) 2021 Ramon Santamaria (@raysan5)
*
********************************************************************************************/
#include "math.h"
#include "raylib.h"
#include "screens.h" // NOTE: Declares global (extern) variables and screens functions

Rectangle tUp = {53, 338, 25, 30};
Rectangle tLeft = {20, 370, 30, 25};
Rectangle tRight = {77, 370, 30, 25};
Rectangle tDown = {53, 400, 25, 30};
Rectangle tFire = {432, 338, 68, 100};

#if defined(PLATFORM_WEB)
#include <emscripten/emscripten.h>
#endif

// shake stuff.
static float shakeAmount = 0.0f;
static float shakeProgress = 0.0f;
static void UpdateShake(float delta);

//----------------------------------------------------------------------------------
// Shared Variables Definition (global)
// NOTE: Those variables are shared between modules through screens.h
//----------------------------------------------------------------------------------
GameScreen currentScreen = 0;
Font font = {0};
Music music = {0};
Sound fxCoin = {0};
int scale = 1;
const int screenWidth = 512;
const int screenHeight = 448;

//----------------------------------------------------------------------------------
// Local Variables Definition (local to this module)
//----------------------------------------------------------------------------------
static Camera2D camera = {0};

// Required variables to manage screen transitions (fade-in, fade-out)
static float transAlpha = 0.0f;
static bool onTransition = false;
static bool transFadeOut = false;
static int transFromScreen = -1;
static int transToScreen = -1;

//----------------------------------------------------------------------------------
// Local Functions Declaration
//----------------------------------------------------------------------------------
static void ChangeToScreen(int screen); // Change to screen, no transition effect

static void TransitionToScreen(int screen); // Request transition to next screen
static void UpdateTransition(void);         // Update transition effect
static void DrawTransition(void);           // Draw transition effect (full-screen rectangle)
static void UpdateDrawFrame(void);          // Update and draw one frame

//----------------------------------------------------------------------------------
// Main entry point
//----------------------------------------------------------------------------------
int main(void)
{
    // Initialization
    //---------------------------------------------------------
    InitWindow(screenWidth * scale, screenHeight * scale, "raylib game template");

    InitAudioDevice(); // Initialize audio device

    // Load global data (assets that must be available in all screens, i.e. font)
    font = GetFontDefault(); // LoadFont("resources/mecha.png");
    //music = LoadMusicStream("resources/ambient.ogg");
    fxCoin = LoadSound("resources/coin.wav");

    SetMusicVolume(music, 1.0f);
    //PlayMusicStream(music);

    // Setup and init first screen
    currentScreen = LOGO;
    InitLogoScreen();

    // this generates all the NPCs and stuff that the scene system will use as the game runs.
    InitSceneSystem();

    camera.zoom = scale;
    camera.offset.x = 0;
    camera.offset.y = 0;

#if defined(PLATFORM_WEB)
    emscripten_set_main_loop(UpdateDrawFrame, 60, 1);
#else
    SetTargetFPS(60); // Set our game to run at 60 frames-per-second
    //--------------------------------------------------------------------------------------

    // Main game loop
    while (!WindowShouldClose()) // Detect window close button or ESC key
    {
        UpdateDrawFrame();
    }
#endif

    // De-Initialization
    //--------------------------------------------------------------------------------------
    // Unload current screen data before closing
    switch (currentScreen)
    {
    case LOGO:
        UnloadLogoScreen();
        break;
    case TITLE:
        UnloadTitleScreen();
        break;
    case GAMEPLAY:
        UnloadGameplayScreen();
        break;
    case ENDING:
        UnloadEndingScreen();
        break;
    default:
        break;
    }

    // Unload global data loaded
    UnloadFont(font);
    UnloadMusicStream(music);
    UnloadSound(fxCoin);

    CloseAudioDevice(); // Close audio context

    CloseWindow(); // Close window and OpenGL context
    //--------------------------------------------------------------------------------------

    return 0;
}

//----------------------------------------------------------------------------------
// Module specific Functions Definition
//----------------------------------------------------------------------------------
// Change to next screen, no transition
static void ChangeToScreen(int screen)
{
    // Unload current screen
    switch (currentScreen)
    {
    case LOGO:
        UnloadLogoScreen();
        break;
    case TITLE:
        UnloadTitleScreen();
        break;
    case GAMEPLAY:
        UnloadGameplayScreen();
        break;
    case ENDING:
        UnloadEndingScreen();
        break;
    default:
        break;
    }

    // Init next screen
    switch (screen)
    {
    case LOGO:
        InitLogoScreen();
        break;
    case TITLE:
        InitTitleScreen();
        break;
    case GAMEPLAY:
        InitGameplayScreen();
        break;
    case ENDING:
        InitEndingScreen();
        break;
    default:
        break;
    }

    currentScreen = screen;
}

// Request transition to next screen
static void TransitionToScreen(int screen)
{
    onTransition = true;
    transFadeOut = false;
    transFromScreen = currentScreen;
    transToScreen = screen;
    transAlpha = 0.0f;
}

// Update transition effect (fade-in, fade-out)
static void UpdateTransition(void)
{
    if (!transFadeOut)
    {
        transAlpha += 0.05f;

        // NOTE: Due to float internal representation, condition jumps on 1.0f instead of 1.05f
        // For that reason we compare against 1.01f, to avoid last frame loading stop
        if (transAlpha > 1.01f)
        {
            transAlpha = 1.0f;

            // Unload current screen
            switch (transFromScreen)
            {
            case LOGO:
                UnloadLogoScreen();
                break;
            case TITLE:
                UnloadTitleScreen();
                break;
            case OPTIONS:
                UnloadOptionsScreen();
                break;
            case GAMEPLAY:
                UnloadGameplayScreen();
                break;
            case ENDING:
                UnloadEndingScreen();
                break;
            default:
                break;
            }

            // Load next screen
            switch (transToScreen)
            {
            case LOGO:
                InitLogoScreen();
                break;
            case TITLE:
                InitTitleScreen();
                break;
            case GAMEPLAY:
                InitGameplayScreen();
                break;
            case ENDING:
                InitEndingScreen();
                break;
            default:
                break;
            }

            currentScreen = transToScreen;

            // Activate fade out effect to next loaded screen
            transFadeOut = true;
        }
    }
    else // Transition fade out logic
    {
        transAlpha -= 0.02f;

        if (transAlpha < -0.01f)
        {
            transAlpha = 0.0f;
            transFadeOut = false;
            onTransition = false;
            transFromScreen = -1;
            transToScreen = -1;
        }
    }
}

// Draw transition effect (full-screen rectangle)
static void DrawTransition(void)
{
    DrawRectangle(0, 0, screenWidth, screenHeight, Fade(BLACK, transAlpha));
}

// Update and draw game frame
static void UpdateDrawFrame(void)
{
    // Update
    //----------------------------------------------------------------------------------
    UpdateMusicStream(music); // NOTE: Music keeps playing between screens

    float delta = GetFrameTime();

    UpdateShake(delta);

    if (!onTransition)
    {
        switch (currentScreen)
        {
        case LOGO:
        {
            UpdateLogoScreen();

            if (FinishLogoScreen())
                TransitionToScreen(TITLE);
        }
        break;
        case TITLE:
        {
            UpdateTitleScreen();

            if (FinishTitleScreen() == 1)
                TransitionToScreen(OPTIONS);
            else if (FinishTitleScreen() == 2)
                TransitionToScreen(GAMEPLAY);
        }
        break;
        case OPTIONS:
        {
            UpdateOptionsScreen();

            if (FinishOptionsScreen())
                TransitionToScreen(TITLE);
        }
        break;
        case GAMEPLAY:
        {
            UpdateGameplayScreen();

            if (FinishGameplayScreen() == 1)
                TransitionToScreen(ENDING);
            //else if (FinishGameplayScreen() == 2) TransitionToScreen(TITLE);
        }
        break;
        case ENDING:
        {
            UpdateEndingScreen();

            if (FinishEndingScreen() == 1)
                TransitionToScreen(TITLE);
        }
        break;
        default:
            break;
        }
    }
    else
        UpdateTransition(); // Update transition (fade-in, fade-out)
    //----------------------------------------------------------------------------------

    // Draw
    //----------------------------------------------------------------------------------
    BeginDrawing();
    {
        BeginMode2D(camera);
        {
            ClearBackground(RAYWHITE);

            switch (currentScreen)
            {
            case LOGO:
                DrawLogoScreen();
                break;
            case TITLE:
                DrawTitleScreen();
                break;
            case OPTIONS:
                DrawOptionsScreen();
                break;
            case GAMEPLAY:
                DrawGameplayScreen();
                break;
            case ENDING:
                DrawEndingScreen();
                break;
            default:
                break;
            }

            // Draw full screen rectangle in front of everything
            if (onTransition)
                DrawTransition();

            //DrawFPS(screenWidth - 30, 10);
        }
        EndMode2D();
    }

    // Vector2 mouse = GetMousePosition();
    // DrawCircle(mouse.x, mouse.y, 10.0f, RED);
    EndDrawing();
    //----------------------------------------------------------------------------------
}

void UpdateShake(float delta)
{
    shakeAmount -= shakeAmount * delta * 2;
    shakeProgress += delta * 100;


    if (shakeAmount > 0.1f)
    {
        camera.offset.x = sinf(shakeProgress) * shakeAmount;
    }
    else
    {
        shakeAmount = 0;
        camera.offset.x = 0;
    }
}

void Shake(float amount)
{
    shakeAmount = amount;
    shakeProgress = 0.0f;
}