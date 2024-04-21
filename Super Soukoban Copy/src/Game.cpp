#include "Game.h"
#include "Globals.h"
#include "ResourceManager.h"
#include <stdio.h>

Music soundMusic[10];
Sound soundEffect[10];

Game::Game()
{
    state = GameState::START;

    scene = nullptr;
    img_menu = nullptr;

    time = GetTime();

    target = {};
    src = {};
    dst = {};
}
Game::~Game()
{
    if (scene != nullptr)
    {
        scene->Release();
        delete scene;
        scene = nullptr;
    }
}
AppStatus Game::Initialise(float scale)
{
    float w, h;
    w = WINDOW_WIDTH * scale;
    h = WINDOW_HEIGHT * scale;

    //Initialise window
    InitWindow((int)w, (int)h, "Super Soukoban");


    //Render texture initialisation, used to hold the rendering result so we can easily resize it
    target = LoadRenderTexture(WINDOW_WIDTH, WINDOW_HEIGHT);

    if (target.id == 0)
    {
        LOG("Failed to create render texture");
        return AppStatus::ERROR;
    }
    SetTextureFilter(target.texture, TEXTURE_FILTER_POINT);
    src = { 0, 0, WINDOW_WIDTH, -WINDOW_HEIGHT };
    dst = { 0, 0, w, h };

    //Load resources
    if (LoadResources() != AppStatus::OK)
    {
        LOG("Failed to load resources");
        return AppStatus::ERROR;
    }

    //Set the target frame rate for the application
    SetTargetFPS(60);
    //Disable the escape key to quit functionality
    SetExitKey(0);

    return AppStatus::OK;
}
AppStatus Game::LoadResources()
{
    ResourceManager& data = ResourceManager::Instance();

    if (data.LoadTexture(Resource::IMG_MENU, "images/menu.png") != AppStatus::OK)
    {
        return AppStatus::ERROR;
    }
    img_menu = data.GetTexture(Resource::IMG_MENU);

    if (data.LoadTexture(Resource::IMG_INITIAL, "images/InitialScreen.png") != AppStatus::OK)
    {
        return AppStatus::ERROR;
    }
    img_initial = data.GetTexture(Resource::IMG_INITIAL);

    return AppStatus::OK;
}
AppStatus Game::BeginPlayMenu()
{
    scene = new Scene();
     soundMusic[1] = LoadMusicStream("sound/music/TitleScreen.ogg");
     PlayMusicStream(soundMusic[1]);
    if (scene == nullptr)
    {
        LOG("Failed to allocate memory for Scene");
        return AppStatus::ERROR;
    }
    if (scene->Init() != AppStatus::OK)
    {
        LOG("Failed to initialise Scene");
        return AppStatus::ERROR;
    }

    return AppStatus::OK;
}
AppStatus Game::BeginPlay()
{
    scene = new Scene();
    soundMusic[0] = LoadMusicStream("sound/music/PhysicalLabor.ogg");
    PlayMusicStream(soundMusic[0]);
    if (scene == nullptr)
    {
        LOG("Failed to allocate memory for Scene");
        return AppStatus::ERROR;
    }
    if (scene->Init() != AppStatus::OK)
    {
        LOG("Failed to initialise Scene");
        return AppStatus::ERROR;
    }

    return AppStatus::OK;
}
void Game::FinishPlay()
{
    scene->Release();
    delete scene;
    scene = nullptr;
}
AppStatus Game::Update()
{
    //Check if user attempts to close the window, either by clicking the close button or by pressing Alt+F4
    if (WindowShouldClose()) return AppStatus::QUIT;
    UpdateMusicStream(soundMusic[0]);
    UpdateMusicStream(soundMusic[1]);
    UpdateMusicStream(soundMusic[2]);
    UpdateMusicStream(soundMusic[3]);
    time = GetTime();
    switch (state)
    {

    case GameState::START:

        DrawTexture(*img_initial, 0, 0, WHITE);
        if (time == 4)
        {
            BeginPlayMenu();
            Transition();
            state = GameState::MAIN_MENU;
        }
        if (IsKeyDown(KEY_Z))
        {
            BeginPlayMenu();
            Transition();
            state = GameState::MAIN_MENU;
        }

    case GameState::MAIN_MENU:
        wonOrLost = false;

        if (IsKeyPressed(KEY_ESCAPE)) return AppStatus::QUIT;

        if (IsKeyPressed(KEY_SPACE) or (IsKeyPressed(KEY_R) and state == GameState::PLAYING))
        {
            Transition();
            StopMusicStream(soundMusic[0]);
            StopMusicStream(soundMusic[2]);
            StopMusicStream(soundMusic[3]);
            soundEffect[0] = LoadSound("sound/SFX/Transition.wav");
            PlaySound(soundEffect[0]);
            StopMusicStream(soundMusic[1]);
            if (BeginPlay() != AppStatus::OK) return AppStatus::ERROR;
            state = GameState::PLAYING;
        }
        break;

    case GameState::PLAYING:
        if (IsKeyPressed(KEY_ESCAPE))
        {
            Transition();
            PlaySound(soundEffect[0]);
            BeginPlayMenu();
            FinishPlay();
            StopMusicStream(soundMusic[0]);
            StopMusicStream(soundMusic[2]);
            StopMusicStream(soundMusic[3]);

            if (BeginPlayMenu() != AppStatus::OK) return AppStatus::ERROR;
            state = GameState::MAIN_MENU;
        }
        if (scene->player->lost and !wonOrLost)
        {
            StopMusicStream(soundMusic[0]);
            soundMusic[3] = LoadMusicStream("sound/music/Failure.ogg");
            PlayMusicStream(soundMusic[3]);
            wonOrLost = true;

            if (IsKeyPressed(KEY_R))
            {
                StopMusicStream(soundMusic[2]);
                StopMusicStream(soundMusic[3]);
                PlayMusicStream(soundMusic[0]);
            }
        }
        if (scene->player->won and !wonOrLost)
        {
            StopMusicStream(soundMusic[0]);
            soundMusic[2] = LoadMusicStream("sound/music/StageComplete.ogg");
            PlayMusicStream(soundMusic[2]);
            wonOrLost = true;
        }

        if (scene->player->won or scene->player->lost)
        {
            if (IsKeyPressed(KEY_R))
            {
                Transition();
                StopMusicStream(soundMusic[2]);
                StopMusicStream(soundMusic[3]);

                PlaySound(soundEffect[0]);
                BeginPlayMenu();
                FinishPlay();
                StopMusicStream(soundMusic[0]);
                StopMusicStream(soundMusic[2]);
                StopMusicStream(soundMusic[3]);

                if (BeginPlayMenu() != AppStatus::OK) return AppStatus::ERROR;
                state = GameState::MAIN_MENU;
            }
        }


        else
        {
            //Game logic
            scene->Update();
        }
        break;
    }
    return AppStatus::OK;
}

void Game::Transition()
{
    BeginTextureMode(target);
    a = 0;
    while (a <= 1)
    {
        DrawRectangle(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT, Fade(BLACK, a));
        a += 0.2;
    }
    while (a >= 0)
    {
        DrawRectangle(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT, Fade(BLACK, a));
        a -= 0.2;
    }
    a = 0;
    EndTextureMode();
}

void Game::Render()
{
    //Draw everything in the render texture, note this will not be rendered on screen, yet
    BeginTextureMode(target);
    ClearBackground(BLACK);

    switch (state)
    {
    case GameState::START:
        DrawTexture(*img_initial, 0, 0, WHITE);
        break;

    case GameState::MAIN_MENU:
        DrawTexture(*img_menu, 0, 0, WHITE);
        break;

    case GameState::PLAYING:
        scene->Render();
        break;
    }

    EndTextureMode();

    //Draw render texture to screen, properly scaled
    BeginDrawing();
    DrawTexturePro(target.texture, src, dst, { 0, 0 }, 0.0f, WHITE);
    EndDrawing();
}
void Game::Cleanup()
{
    UnloadResources();
    CloseWindow();
}
void Game::UnloadResources()
{
    ResourceManager& data = ResourceManager::Instance();
    data.ReleaseTexture(Resource::IMG_MENU);

    UnloadRenderTexture(target);
}