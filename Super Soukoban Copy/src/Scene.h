#pragma once
#include <raylib.h>
#include "Player.h"
#include "TileMap.h"
#include "Object.h"
#include "Text.h"

enum class DebugMode { OFF, SPRITES_AND_HITBOXES, ONLY_HITBOXES, SIZE };

class Scene
{
public:
    Scene();
    ~Scene();

    AppStatus Init(int stage);
    void Update();
    void Render();
    void Release();

    Player* player;

private:
    AppStatus LoadLevel(int stage);

    int CurrentStage = 0;

    void CheckCollisions();
    void ClearLevel();
    void RenderObjects() const;
    void RenderObjectsDebug(const Color& col) const;

    void RenderGUI() const;

    TileMap* level;
    TileMap* Auxlevel;
    std::vector<Object*> objects;

    Camera2D camera;
    DebugMode debug;

    Text *font;
};