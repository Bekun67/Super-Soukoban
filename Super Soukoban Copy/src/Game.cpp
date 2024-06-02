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
    img_selection = nullptr;

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

    if (data.LoadTexture(Resource::IMG_SELECTION, "images/selection.png") != AppStatus::OK)
    {
        return AppStatus::ERROR;
    }
    img_selection = data.GetTexture(Resource::IMG_SELECTION);

    if (data.LoadTexture(Resource::IMG_INITIAL, "images/InitialScreen.png") != AppStatus::OK)
    {
        return AppStatus::ERROR;
    }
    img_initial = data.GetTexture(Resource::IMG_INITIAL);

    if (data.LoadTexture(Resource::IMG_HAND, "images/mano.png") != AppStatus::OK)
    {
        return AppStatus::ERROR;
    }
    img_hand = data.GetTexture(Resource::IMG_HAND);

    //INTRO
    data.LoadTexture(Resource::IMG_INTRO1, "images/intro/girl1.png");
    img_intro = data.GetTexture(Resource::IMG_INTRO1);

    data.LoadTexture(Resource::IMG_INTRO2, "images/intro/girl1m.png");
    img_intro2 = data.GetTexture(Resource::IMG_INTRO2);

    data.LoadTexture(Resource::IMG_INTRO3, "images/intro/girl2.png");
    img_intro3 = data.GetTexture(Resource::IMG_INTRO3);

    data.LoadTexture(Resource::IMG_INTRO4, "images/intro/girl2m.png");
    img_intro4 = data.GetTexture(Resource::IMG_INTRO4);

    data.LoadTexture(Resource::IMG_INTRO5, "images/intro/girl3.png");
    img_intro5 = data.GetTexture(Resource::IMG_INTRO5);

    data.LoadTexture(Resource::IMG_INTRO6, "images/intro/girl3m.png");
    img_intro6 = data.GetTexture(Resource::IMG_INTRO6);

    data.LoadTexture(Resource::IMG_INTRO7, "images/intro/car1.png");
    img_intro7 = data.GetTexture(Resource::IMG_INTRO7);

    data.LoadTexture(Resource::IMG_INTRO8, "images/intro/car2.png");
    img_intro8 = data.GetTexture(Resource::IMG_INTRO8);

    data.LoadTexture(Resource::IMG_INTRO9, "images/intro/car3.png");
    img_intro9 = data.GetTexture(Resource::IMG_INTRO9);

    data.LoadTexture(Resource::IMG_INTRO10, "images/intro/car4.png");
    img_intro10 = data.GetTexture(Resource::IMG_INTRO10);

    data.LoadTexture(Resource::IMG_INTRO10, "images/intro/car4.png");
    img_intro10 = data.GetTexture(Resource::IMG_INTRO10);

    data.LoadTexture(Resource::IMG_INTRO11, "images/intro/car5.png");
    img_intro11 = data.GetTexture(Resource::IMG_INTRO11);

    data.LoadTexture(Resource::IMG_INTRO12, "images/intro/car6.png");
    img_intro12 = data.GetTexture(Resource::IMG_INTRO12);

    data.LoadTexture(Resource::IMG_INTRO13, "images/intro/car7.png");
    img_intro13 = data.GetTexture(Resource::IMG_INTRO13);

    data.LoadTexture(Resource::IMG_INTRO14, "images/intro/car8.png");
    img_intro14 = data.GetTexture(Resource::IMG_INTRO14);

    data.LoadTexture(Resource::IMG_INTRO15, "images/intro/car9.png");
    img_intro15 = data.GetTexture(Resource::IMG_INTRO15);

    data.LoadTexture(Resource::IMG_INTRO16, "images/intro/car10.png");
    img_intro16 = data.GetTexture(Resource::IMG_INTRO16);

    data.LoadTexture(Resource::IMG_INTRO17, "images/intro/car11.png");
    img_intro17 = data.GetTexture(Resource::IMG_INTRO17);

    data.LoadTexture(Resource::IMG_INTRO18, "images/intro/car12.png");
    img_intro18 = data.GetTexture(Resource::IMG_INTRO18);

    data.LoadTexture(Resource::IMG_INTRO19, "images/intro/car13.png");
    img_intro19 = data.GetTexture(Resource::IMG_INTRO19);

    data.LoadTexture(Resource::IMG_INTRO20, "images/intro/car14.png");
    img_intro20 = data.GetTexture(Resource::IMG_INTRO20);

    data.LoadTexture(Resource::IMG_INTRO21, "images/intro/car15.png");
    img_intro21 = data.GetTexture(Resource::IMG_INTRO21);

    data.LoadTexture(Resource::IMG_INTRO22, "images/intro/car16.png");
    img_intro22 = data.GetTexture(Resource::IMG_INTRO22);

    data.LoadTexture(Resource::IMG_INTRO23, "images/intro/car17.png");
    img_intro23 = data.GetTexture(Resource::IMG_INTRO23);

    data.LoadTexture(Resource::IMG_INTRO24, "images/intro/car18.png");
    img_intro24 = data.GetTexture(Resource::IMG_INTRO24);


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
    if (scene->Init(1) != AppStatus::OK)
    {
        LOG("Failed to initialise Scene");
        return AppStatus::ERROR;
    }

    return AppStatus::OK;
}
AppStatus Game::BeginPlaySelection()
{
    scene = new Scene();
    soundMusic[4] = LoadMusicStream("sound/music/StageSelect.ogg");
    PlayMusicStream(soundMusic[4]);
    if (scene == nullptr)
    {
        LOG("Failed to allocate memory for Scene");
        return AppStatus::ERROR;
    }
    if (scene->Init(1) != AppStatus::OK)
    {
        LOG("Failed to initialise Scene");
        return AppStatus::ERROR;
    }

    return AppStatus::OK;
}
AppStatus Game::BeginPlay(int stage)
{
    scene = new Scene();
    soundMusic[0] = LoadMusicStream("sound/music/PhysicalLabor.ogg");
    PlayMusicStream(soundMusic[0]);
    if (scene == nullptr)
    {
        LOG("Failed to allocate memory for Scene");
        return AppStatus::ERROR;
    }
    if (scene->Init(stage) != AppStatus::OK)
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
    UpdateMusicStream(soundMusic[4]);
    UpdateMusicStream(soundMusic[7]);

    time = GetTime();
    switch (state)
    {

    case GameState::START:

        DrawTexture(*img_initial, 0, 0, WHITE);
        if (time > 4)
        {
            Intro();
            ClearBackground(BLACK);
            state = GameState::INTRO;
        }
        if (IsKeyDown(KEY_Z))
        {
            BeginPlayMenu();
            Transition();
            state = GameState::MAIN_MENU;
        }

    case GameState::INTRO:
        if (time > 44)
        {
            StopMusicStream(soundMusic[7]);
            BeginPlayMenu();
            Transition();
            state = GameState::MAIN_MENU;
        }
        if (IsKeyDown(KEY_Z))
        {
            StopMusicStream(soundMusic[7]);
            BeginPlayMenu();
            Transition();
            state = GameState::MAIN_MENU;
        }

    case GameState::MAIN_MENU:
        wonOrLost = false;

        if (IsKeyPressed(KEY_ESCAPE)) return AppStatus::QUIT;

        if (((IsKeyPressed(KEY_SPACE) and state == GameState::MAIN_MENU) or (IsKeyPressed(KEY_R) and state == GameState::PLAYING)) and state != GameState::START)
        {
            Transition();
            StopMusicStream(soundMusic[0]);
            StopMusicStream(soundMusic[2]);
            StopMusicStream(soundMusic[3]);
            StopMusicStream(soundMusic[4]);
            soundEffect[0] = LoadSound("sound/SFX/Transition.wav");
            soundEffect[1] = LoadSound("sound/SFX/Ding.wav");
            soundEffect[2] = LoadSound("sound/SFX/Menu.wav");
            PlaySound(soundEffect[0]);
            StopMusicStream(soundMusic[1]);

            stage = 1;

            if (BeginPlaySelection() != AppStatus::OK) return AppStatus::ERROR;
            state = GameState::SELECTION;
        }
        break;

    case GameState::SELECTION:
        wonOrLost = false;

        if (IsKeyPressed(KEY_ESCAPE))
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
            StopMusicStream(soundMusic[4]);

            if (BeginPlayMenu() != AppStatus::OK) return AppStatus::ERROR;
            state = GameState::MAIN_MENU;
        }

        if (IsKeyPressed(KEY_SPACE) and state == GameState::SELECTION)
        {
            wonOrLost = false;
            StopMusicStream(soundMusic[2]);
            StopMusicStream(soundMusic[3]);
            StopMusicStream(soundMusic[0]);
            StopMusicStream(soundMusic[2]);
            StopMusicStream(soundMusic[3]);
            StopMusicStream(soundMusic[4]);

            state = GameState::TRANSITION;
        }

        if (state == GameState::SELECTION and IsKeyPressed(KEY_RIGHT) and stage != 8)
        {
            PlaySound(soundEffect[1]);
            stage++;
        }
        if (state == GameState::SELECTION and IsKeyPressed(KEY_LEFT) and stage != 1)
        {
            PlaySound(soundEffect[1]);
            stage--;
        }

    case GameState::PLAYING:
        if (IsKeyPressed(KEY_ESCAPE) and state != GameState::MAIN_MENU)
        {
            wonOrLost = false;

            Transition();
            PlaySound(soundEffect[2]);
            BeginPlaySelection();
            FinishPlay();
            StopMusicStream(soundMusic[0]);
            StopMusicStream(soundMusic[2]);
            StopMusicStream(soundMusic[3]);
            StopMusicStream(soundMusic[4]);

            if (BeginPlaySelection() != AppStatus::OK) return AppStatus::ERROR;
            state = GameState::SELECTION;
        }

        if (scene->player->lost and !wonOrLost and state == GameState::PLAYING)
        {
            StopMusicStream(soundMusic[0]);
            soundMusic[3] = LoadMusicStream("sound/music/Failure.ogg");
            PlayMusicStream(soundMusic[3]);
            wonOrLost = true;

            if (IsKeyPressed(KEY_R))
            {
                StopMusicStream(soundMusic[2]);
                StopMusicStream(soundMusic[3]);
                StopMusicStream(soundMusic[4]);
                PlayMusicStream(soundMusic[0]);
            }
        }
        if (scene->player->won and !wonOrLost and state == GameState::PLAYING)
        {
            StopMusicStream(soundMusic[0]);
            soundMusic[2] = LoadMusicStream("sound/music/StageComplete.ogg");
            PlayMusicStream(soundMusic[2]);
            wonOrLost = true;
        }

        if ((scene->player->won or scene->player->lost) and state == GameState::PLAYING)
        {
            if (IsKeyPressed(KEY_R))
            {
                wonOrLost = false;
                StopMusicStream(soundMusic[2]);
                StopMusicStream(soundMusic[3]);
                FinishPlay();
                StopMusicStream(soundMusic[0]);
                StopMusicStream(soundMusic[2]);
                StopMusicStream(soundMusic[3]);
                StopMusicStream(soundMusic[4]);

                state = GameState::TRANSITION;
            }
        }

        else
        {
            //Game logic
            scene->Update();
        }
        break;

    case GameState::TRANSITION:

        wonOrLost = false;
        Transition();
        StopMusicStream(soundMusic[0]);
        StopMusicStream(soundMusic[2]);
        StopMusicStream(soundMusic[3]);
        StopMusicStream(soundMusic[4]);
        soundEffect[0] = LoadSound("sound/SFX/Transition.wav");
        PlaySound(soundEffect[0]);
        StopMusicStream(soundMusic[1]);
        if (BeginPlay(stage) != AppStatus::OK) return AppStatus::ERROR;
        state = GameState::PLAYING;

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
        WaitTime(0.01);
    }
    while (a >= 0)
    {
        DrawRectangle(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT, Fade(BLACK, a));
        a -= 0.2;
        WaitTime(0.01);
    }
    a = 0;
    EndTextureMode();
    BeginDrawing();
    DrawTexturePro(target.texture, src, dst, { 0, 0 }, 0.0f, WHITE);
    EndDrawing();
}

void Game::Intro()
{
    soundMusic[7] = LoadMusicStream("sound/music/Opening.ogg");
    PlayMusicStream(soundMusic[7]);
}

void Game::Render()
{
    //Draw everything in the render texture, note this will not be rendered on screen, yet
    BeginTextureMode(target);
    ClearBackground(BLACK);

    switch (state)
    {
    case GameState::INTRO:
        if (time >= 5 and time <= 6)
        {
            DrawTexture(*img_intro, 0, 0, WHITE);
        }
        if (time > 6 and time <= 7)
        {
            DrawTexture(*img_intro2, 0, 0, WHITE);
        }
        if (time > 7 and time <= 8)
        {
            DrawTexture(*img_intro, 0, 0, WHITE);
        }
        if (time > 8 and time <= 9)
        {
            DrawTexture(*img_intro2, 0, 0, WHITE);
        }
        if (time > 9 and time <= 10)
        {
            DrawTexture(*img_intro3, 0, 0, WHITE);
        }
        if (time > 10 and time <= 11)
        {
            DrawTexture(*img_intro4, 0, 0, WHITE);
        }
        if (time > 11 and time <= 12)
        {
            DrawTexture(*img_intro3, 0, 0, WHITE);
        }
        if (time > 12 and time <= 13)
        {
            DrawTexture(*img_intro4, 0, 0, WHITE);
        }
        if (time > 13 and time <= 14)
        {
            DrawTexture(*img_intro5, 0, 0, WHITE);
        }
        if (time > 14 and time <= 15)
        {
            DrawTexture(*img_intro6, 0, 0, WHITE);
        }
        if (time > 15 and time <= 16)
        {
            DrawTexture(*img_intro5, 0, 0, WHITE);
        }
        if (time > 16 and time <= 17)
        {
            DrawTexture(*img_intro6, 0, 0, WHITE);
        }
        if (time > 17 and time <= 18)
        {
            DrawTexture(*img_intro7, 0, 0, WHITE);
        }
        if (time > 18 and time <= 18.2)
        {
            DrawTexture(*img_intro8, 0, 0, WHITE);
        }
        if (time > 18.2 and time <= 18.4)
        {
            DrawTexture(*img_intro9, 0, 0, WHITE);
        }
        if (time > 18.4 and time <= 19.4)
        {
            DrawTexture(*img_intro10, 0, 0, WHITE);
        }
        if (time > 19.4 and time <= 21)
        {
            DrawTexture(*img_intro11, 0, 0, WHITE);
        }
        if (time > 21 and time <= 22.7)
        {
            DrawTexture(*img_intro12, 0, 0, WHITE);
        }
        if (time > 22.7 and time <= 24.2)
        {
            DrawTexture(*img_intro13, 0, 0, WHITE);
        }
        if (time > 24.2 and time <= 25.4)
        {
            DrawTexture(*img_intro14, 0, 0, WHITE);
        }
        if (time > 25.4 and time <= 25.6)
        {
            DrawTexture(*img_intro15, 0, 0, WHITE);
        }
        if (time > 25.6 and time <= 25.8)
        {
            DrawTexture(*img_intro16, 0, 0, WHITE);
        }
        if (time > 25.8 and time <= 26.8)
        {
            DrawTexture(*img_intro17, 0, 0, WHITE);
        }
        if (time > 26.8 and time <= 28.4)
        {
            DrawTexture(*img_intro18, 0, 0, WHITE);
        }
        if (time > 28.4 and time <= 30.6)
        {
            DrawTexture(*img_intro19, 0, 0, WHITE);
        }
        if (time > 30.6 and time <= 32.6)
        {
            DrawTexture(*img_intro20, 0, 0, WHITE);
        }
        if (time > 32.6 and time <= 34)
        {
            DrawTexture(*img_intro21, 0, 0, WHITE);
        }
        if (time > 34 and time <= 35.2)
        {
            DrawTexture(*img_intro22, 0, 0, WHITE);
        }
        if (time > 35.2 and time <= 37.4)
        {
            DrawTexture(*img_intro23, 0, 0, WHITE);
        }
        if (time > 37.4 and time <= 41)
        {
            DrawTexture(*img_intro24, 0, 0, WHITE);
        }
        break;

    case GameState::START:
        DrawTexture(*img_initial, 0, 0, WHITE);
        break;

    case GameState::MAIN_MENU:
        DrawTexture(*img_menu, 0, 0, WHITE);
        break;

    case GameState::SELECTION:
        DrawTexture(*img_selection, 0, 0, WHITE);
        switch (stage)
        {
        case 1:
            DrawTexture(*img_hand, 45, 155, WHITE);
            break;
        case 2:
            DrawTexture(*img_hand, 67, 155, WHITE);
            break;
        case 3:
            DrawTexture(*img_hand, 89, 155, WHITE);
            break;
        case 4:
            DrawTexture(*img_hand, 111, 155, WHITE);
            break;
        case 5:
            DrawTexture(*img_hand, 133, 155, WHITE);
            break;
        case 6:
            DrawTexture(*img_hand, 155, 155, WHITE);
            break;
        case 7:
            DrawTexture(*img_hand, 177, 155, WHITE);
            break;
        case 8:
            DrawTexture(*img_hand, 199, 155, WHITE);
            break;
        default:
            break;
        }
        //DrawTexture(*img_menu, 10, 10, WHITE);
        break;

    case GameState::TRANSITION:
        DrawTexture(*img_selection, 0, 0, WHITE);
        switch (stage)
        {
        case 1:
            DrawTexture(*img_hand, 45, 155, WHITE);
            break;
        case 2:
            DrawTexture(*img_hand, 67, 155, WHITE);
            break;
        case 3:
            DrawTexture(*img_hand, 89, 155, WHITE);
            break;
        case 4:
            DrawTexture(*img_hand, 111, 155, WHITE);
            break;
        case 5:
            DrawTexture(*img_hand, 133, 155, WHITE);
            break;
        case 6:
            DrawTexture(*img_hand, 155, 155, WHITE);
            break;
        case 7:
            DrawTexture(*img_hand, 177, 155, WHITE);
            break;
        case 8:
            DrawTexture(*img_hand, 199, 155, WHITE);
            break;
        default:
            break; 
        }
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