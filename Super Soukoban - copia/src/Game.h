#pragma once
#include "Globals.h"
#include "Scene.h"

enum class GameState { MAIN_MENU, PLAYING, SETTINGS, CREDITS, START };

class Game
{
public:
    Game();
    ~Game();

    AppStatus Initialise(float scale);
    AppStatus Update();
    void Render();
    void Cleanup();

private:
    AppStatus BeginPlay();
    AppStatus BeginPlayMenu();
    void FinishPlay();

    int time;
    float a = 0;

    bool wonOrLost = false;

    AppStatus LoadResources();
    void UnloadResources();

    GameState state;
    Scene* scene;
    const Texture2D* img_menu;
    const Texture2D* img_initial;

    void Transition();

    //To work with original game units and then scale the result
    RenderTexture2D target;
    Rectangle src, dst;
};