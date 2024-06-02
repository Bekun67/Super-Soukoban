#include "Game.h"
#include "MemLeaks.h"
#include "Globals.h"

int main()
{
    ReportMemoryLeaks();

    Game* game;
    AppStatus status;
    int main_return = EXIT_SUCCESS;

    game = new Game();
    status = game->Initialise(GAME_SCALE_FACTOR);
    InitAudioDevice();
    if (!IsWindowFullscreen())
    {
        int monitor = GetCurrentMonitor();
        SetWindowSize(GetMonitorWidth(monitor), GetMonitorHeight(monitor));
        ToggleFullscreen();
    }
    else
    {
        ToggleFullscreen();
        SetWindowSize(WINDOW_WIDTH, WINDOW_HEIGHT);
    }
    LOG("Application start");
    if (status != AppStatus::OK)
    {
        LOG("Failed to initialise game");
        main_return = EXIT_FAILURE;
    }

    while (status == AppStatus::OK)
    {
        status = game->Update();
        if (status != AppStatus::OK)
        {
            if (status == AppStatus::ERROR)      main_return = EXIT_FAILURE;
            else if (status == AppStatus::QUIT)  main_return = EXIT_SUCCESS;
            break;
        }
        game->Render();
    }

    LOG("Application finish");
    game->Cleanup();
    CloseAudioDevice();
    LOG("Bye :3");
    delete game;

    return main_return;
}