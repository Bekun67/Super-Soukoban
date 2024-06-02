#pragma once

#include "raylib.h"
#include "Globals.h"
#include <unordered_map>
#include <string>

enum class Resource {
    IMG_MENU,
    IMG_PLAYER,
    IMG_TILES,
    IMG_ITEMS,
    IMG_HUD,
    IMG_BOX,
    IMG_WIN,
    IMG_LOSE,
    IMG_INITIAL,
    IMG_FONT,
    IMG_SELECTION,
    IMG_HAND,
    IMG_INTRO1,
    IMG_INTRO2,
    IMG_INTRO3,
    IMG_INTRO4,
    IMG_INTRO5,
    IMG_INTRO6,
    IMG_INTRO7,
    IMG_INTRO8,
    IMG_INTRO9,
    IMG_INTRO10,
    IMG_INTRO11,
    IMG_INTRO12,
    IMG_INTRO13,
    IMG_INTRO14,
    IMG_INTRO15,
    IMG_INTRO16,
    IMG_INTRO17,
    IMG_INTRO18,
    IMG_INTRO19,
    IMG_INTRO20,
    IMG_INTRO21,
    IMG_INTRO22,
    IMG_INTRO23,
    IMG_INTRO24,
};

class ResourceManager {
public:
    //Singleton instance retrieval
    static ResourceManager& Instance()
    {
        static ResourceManager instance; //Guaranteed to be initialized only once
        return instance;
    }

    //Load and unload texture
    AppStatus LoadTexture(Resource id, const std::string& file_path);
    void ReleaseTexture(Resource id);

    //Get texture by key
    const Texture2D* GetTexture(Resource id) const;

    //Release resources
    void Release();

    //Ensure Singleton cannot be copied or assigned
    ResourceManager(const ResourceManager&) = delete;
    ResourceManager& operator=(const ResourceManager&) = delete;

private:
    //Private constructor to prevent instantiation
    ResourceManager();
    //Destructor
    ~ResourceManager();

    //Dictionary to store loaded textures
    std::unordered_map<Resource, Texture2D> textures;
};