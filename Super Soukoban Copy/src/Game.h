#pragma once
#include "Globals.h"
#include "Scene.h"

enum class GameState { MAIN_MENU, PLAYING, SETTINGS, CREDITS, START, SELECTION, TRANSITION, INTRO };

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
    AppStatus BeginPlay(int stage);
    AppStatus BeginPlayMenu();
    AppStatus BeginPlaySelection();
    void FinishPlay();

    float time;
    float a = 0;

    bool wonOrLost = false;
    int stage = 1;

    void Intro();

    AppStatus LoadResources();
    void UnloadResources();

    GameState state;
    Scene* scene;
    const Texture2D* img_menu;
    const Texture2D* img_selection;
    const Texture2D* img_initial;
    const Texture2D* img_hand;

    const Texture2D* img_intro;
    const Texture2D* img_intro2;
    const Texture2D* img_intro3;
    const Texture2D* img_intro4;
    const Texture2D* img_intro5;
    const Texture2D* img_intro6;
    const Texture2D* img_intro7;
    const Texture2D* img_intro8;
    const Texture2D* img_intro9;
    const Texture2D* img_intro10;
    const Texture2D* img_intro11;
    const Texture2D* img_intro12;
    const Texture2D* img_intro13;
    const Texture2D* img_intro14;
    const Texture2D* img_intro15;
    const Texture2D* img_intro16;
    const Texture2D* img_intro17;
    const Texture2D* img_intro18;
    const Texture2D* img_intro19;
    const Texture2D* img_intro20;
    const Texture2D* img_intro21;
    const Texture2D* img_intro22;
    const Texture2D* img_intro23;
    const Texture2D* img_intro24;

    void Transition();

    //To work with original game units and then scale the result
    RenderTexture2D target;
    Rectangle src, dst;
};