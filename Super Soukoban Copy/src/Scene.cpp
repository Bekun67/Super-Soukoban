#include "Scene.h"
#include <stdio.h>
#include "Globals.h"

Scene::Scene()
{
	player = nullptr;
	level = nullptr;
	Auxlevel = nullptr;

	font = nullptr;

	camera.target = { 0, 0 };				//Center of the screen
	camera.offset = { 0, MARGIN_GUI_Y };	//Offset from the target (center of the screen)
	camera.rotation = 0.0f;					//No rotation
	camera.zoom = 1.0f;						//Default zoom

	debug = DebugMode::OFF;
}
Scene::~Scene()
{
	if (player != nullptr)
	{
		player->Release();
		delete player;
		player = nullptr;
	}
	if (level != nullptr)
	{
		level->Release();
		delete level;
		level = nullptr;
	}
	if (font != nullptr)
	{
		delete font;
		font = nullptr;
	}
	if (Auxlevel != nullptr)
	{
		Auxlevel->Release();
		delete Auxlevel;
		Auxlevel = nullptr;
	}
	for (Entity* obj : objects)
	{
		delete obj;
	}
	objects.clear();
}
AppStatus Scene::Init(int stage)
{
	//Create player
	player = new Player({ 0,0 }, State::IDLE, Look::RIGHT);
	if (player == nullptr)
	{
		LOG("Failed to allocate memory for Player");
		return AppStatus::ERROR;
	}
	//Initialise player
	if (player->Initialise() != AppStatus::OK)
	{
		LOG("Failed to initialise Player");
		return AppStatus::ERROR;
	}

	//Create level 
	level = new TileMap();
	if (level == nullptr)
	{
		LOG("Failed to allocate memory for Level");
		return AppStatus::ERROR;
	}
	Auxlevel = new TileMap();
	if (Auxlevel == nullptr)
	{
		LOG("Failed to allocate memory for Level");
		return AppStatus::ERROR;
	}
	font = new Text();
	if (font == nullptr)
	{
		LOG("Failed to allocate memory for font 1");
		return AppStatus::ERROR;
	}
	if (font->Initialise(Resource::IMG_FONT, "images/font8x8.png", (char)0, 8) != AppStatus::OK)
	{
		LOG("Failed to initialise Level");
		return AppStatus::ERROR;
	}
	//Initialise level
	if (level->Initialise() != AppStatus::OK)
	{
		LOG("Failed to initialise Level");
		return AppStatus::ERROR;
	}
	if (Auxlevel->Initialise() != AppStatus::OK)
	{
		LOG("Failed to initialise Level");
		return AppStatus::ERROR;
	}
	//Load level
	if (LoadLevel(stage) != AppStatus::OK)
	{
		LOG("Failed to load Level 1");
		return AppStatus::ERROR;
	}
	//Assign the tile map reference to the player to check collisions while navigating
	player->SetTileMap(level);
	player->SetAuxTileMap(Auxlevel);

	return AppStatus::OK;
}

AppStatus Scene::LoadLevel(int stage)
{
	int size;
	int x, y, i;
	Tile tile;
	Point pos;
	int* map = nullptr;
	int* mapAux = nullptr;
	Object* obj;

	CurrentStage = stage;

	ClearLevel();

	size = LEVEL_WIDTH * LEVEL_HEIGHT;
	if (stage == 1)
	{
		player->level = 1;
		player->wincount = WINCOUNT_LVL1;
		player->steps = 0;
		player->lost = false;
		player->won = false;

		pos.x = 4 * TILE_SIZE;
		pos.y = 3 * TILE_SIZE - 1;

		map = new int[size] {
			6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6,
				6, 6, 2, 3, 3, 3, 3, 3, 2, 6, 6,
				6, 6, 2, 5, 5, 7, 7, 7, 2, 6, 6,
				6, 6, 2, 5, 5, 5, 3, 3, 3, 2, 6,
				6, 2, 3, 3, 5, 5, 5, 5, 5, 2, 6,
				6, 2, 5, 5, 5, 2, 1, 3, 5, 2, 6,
				6, 2, 5, 1, 1, 2, 5, 5, 5, 2, 6,
				6, 2, 5, 5, 5, 2, 3, 3, 3, 3, 6,
				6, 3, 3, 3, 3, 3, 6, 6, 6, 6, 6,
				6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6
		};
		mapAux = new int[size] {
			6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6,
				6, 6, 2, 3, 3, 3, 3, 3, 2, 6, 6,
				6, 6, 2, 5, 5, 7, 7, 7, 2, 6, 6,
				6, 6, 2, 5, 5, 5, 3, 3, 3, 2, 6,
				6, 2, 3, 3, 5, 5, 5, 5, 5, 2, 6,
				6, 2, 5, 5, 5, 2, 5, 3, 5, 2, 6,
				6, 2, 5, 5, 5, 2, 5, 5, 5, 2, 6,
				6, 2, 5, 5, 5, 2, 3, 3, 3, 3, 6,
				6, 3, 3, 3, 3, 3, 6, 6, 6, 6, 6,
				6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6
		};
		player->InitScore();

	}

	if (stage == 2)
	{
		player->level = 2;
		player->wincount = WINCOUNT_LVL2;
		player->steps = 0;
		player->lost = false;
		player->won = false;

		pos.x = 2 * TILE_SIZE;
		pos.y = 3 * TILE_SIZE - 1;

		map = new int[size] {
				6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6,
				6, 2, 3, 3, 3, 2, 6, 6, 6, 6, 6,
				6, 2, 5, 5, 5, 2, 6, 6, 6, 6, 6,
				6, 2, 5, 1, 1, 2, 6, 2, 3, 2, 6,
				6, 2, 5, 1, 5, 2, 6, 2, 7, 2, 6,
				6, 3, 2, 2, 5, 3, 3, 3, 7, 2, 6,
				6, 6, 2, 3, 5, 5, 5, 5, 7, 2, 6,
				6, 6, 2, 5, 5, 5, 2, 5, 5, 2, 6,
				6, 6, 2, 5, 5, 5, 2, 3, 3, 3, 6,
				6, 6, 3, 3, 3, 3, 3, 6, 6, 6, 6,
		};

		mapAux = new int[size] {
				6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6,
				6, 2, 3, 3, 3, 2, 6, 6, 6, 6, 6,
				6, 2, 5, 5, 5, 2, 6, 6, 6, 6, 6,
				6, 2, 5, 5, 5, 2, 6, 2, 3, 2, 6,
				6, 2, 5, 5, 5, 2, 6, 2, 7, 2, 6,
				6, 3, 2, 2, 5, 3, 3, 3, 7, 2, 6,
				6, 6, 2, 3, 5, 5, 5, 5, 7, 2, 6,
				6, 6, 2, 5, 5, 5, 2, 5, 5, 2, 6,
				6, 6, 2, 5, 5, 5, 2, 3, 3, 3, 6,
				6, 6, 3, 3, 3, 3, 3, 6, 6, 6, 6,
		};
		player->InitScore();
	}

	if (stage == 3)
	{
		player->level = 3;
		player->wincount = WINCOUNT_LVL3;
		player->steps = 0;
		player->lost = false;
		player->won = false;

		pos.x = 4 * TILE_SIZE;
		pos.y = 4 * TILE_SIZE - 1;

		map = new int[size] {
			6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6,
				6, 6, 6, 6, 2, 3, 3, 2, 6, 6, 6,
				6, 6, 6, 2, 3, 5, 5, 2, 6, 6, 6,
				6, 6, 6, 2, 5, 1, 5, 2, 6, 6, 6,
				6, 6, 6, 2, 2, 1, 5, 3, 2, 6, 6,
				6, 6, 6, 2, 3, 5, 1, 5, 2, 6, 6,
				6, 6, 6, 2, 7, 1, 5, 5, 2, 6, 6,
				6, 6, 6, 2, 7, 7, 4, 7, 2, 6, 6,
				6, 6, 6, 3, 3, 3, 3, 3, 3, 6, 6,
				6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6,
		};

		mapAux = new int[size] {
			6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6,
				6, 6, 6, 6, 2, 3, 3, 2, 6, 6, 6,
				6, 6, 6, 2, 3, 5, 5, 2, 6, 6, 6,
				6, 6, 6, 2, 5, 5, 5, 2, 6, 6, 6,
				6, 6, 6, 2, 2, 5, 5, 3, 2, 6, 6,
				6, 6, 6, 2, 3, 5, 5, 5, 2, 6, 6,
				6, 6, 6, 2, 7, 5, 5, 5, 2, 6, 6,
				6, 6, 6, 2, 7, 7, 7, 7, 2, 6, 6,
				6, 6, 6, 3, 3, 3, 3, 3, 3, 6, 6,
				6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6,
		};
		player->InitScore();
	}

	if (stage == 4)
	{
		player->level = 4;
		player->wincount = WINCOUNT_LVL4;
		player->steps = 0;
		player->lost = false;
		player->won = false;

		pos.x = 3 * TILE_SIZE;
		pos.y = 5 * TILE_SIZE - 1;

		map = new int[size] {
			6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6,
				6, 6, 2, 3, 3, 3, 3, 3, 2, 6, 6,
				6, 6, 2, 5, 5, 5, 5, 5, 3, 3, 2,
				6, 2, 3, 1, 3, 3, 3, 5, 5, 5, 2,
				6, 2, 5, 5, 5, 1, 5, 5, 1, 5, 2,
				6, 2, 5, 7, 7, 2, 5, 1, 5, 2, 3,
				6, 3, 2, 7, 7, 2, 5, 5, 5, 2, 6,
				6, 6, 3, 3, 3, 3, 3, 3, 3, 3, 6,
				6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6,
				6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6,
		};

		mapAux = new int[size] {
			6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6,
				6, 6, 2, 3, 3, 3, 3, 3, 2, 6, 6,
				6, 6, 2, 5, 5, 5, 5, 5, 3, 3, 2,
				6, 2, 3, 5, 3, 3, 3, 5, 5, 5, 2,
				6, 2, 5, 5, 5, 5, 5, 5, 5, 5, 2,
				6, 2, 5, 7, 7, 2, 5, 5, 5, 2, 3,
				6, 3, 2, 7, 7, 2, 5, 5, 5, 2, 6,
				6, 6, 3, 3, 3, 3, 3, 3, 3, 3, 6,
				6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6,
				6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6,
		};
		player->InitScore();
	}

	if (stage == 5)
	{
		player->level = 5;
		player->wincount = WINCOUNT_LVL5;
		player->steps = 0;
		player->lost = false;
		player->won = false;

		pos.x = 3 * TILE_SIZE;
		pos.y = 3 * TILE_SIZE - 1;

		map = new int[size] {
			6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6,
				6, 6, 2, 3, 3, 2, 6, 6, 6, 6, 6,
				6, 6, 2, 5, 5, 3, 3, 2, 6, 6, 6,
				6, 6, 2, 5, 1, 5, 5, 2, 6, 6, 6,
				6, 2, 3, 2, 5, 2, 5, 3, 2, 6, 6,
				6, 2, 7, 3, 5, 3, 5, 5, 2, 6, 6,
				6, 2, 7, 1, 5, 5, 3, 5, 2, 6, 6,
				6, 2, 7, 5, 5, 5, 1, 5, 2, 6, 6,
				6, 3, 3, 3, 3, 3, 3, 3, 3, 6, 6,
				6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6,
		};

		mapAux = new int[size] {
			6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6,
				6, 6, 2, 3, 3, 2, 6, 6, 6, 6, 6,
				6, 6, 2, 5, 5, 3, 3, 2, 6, 6, 6,
				6, 6, 2, 5, 5, 5, 5, 2, 6, 6, 6,
				6, 2, 3, 2, 5, 2, 5, 3, 2, 6, 6,
				6, 2, 7, 3, 5, 3, 5, 5, 2, 6, 6,
				6, 2, 7, 5, 5, 5, 3, 5, 2, 6, 6,
				6, 2, 7, 5, 5, 5, 5, 5, 2, 6, 6,
				6, 3, 3, 3, 3, 3, 3, 3, 3, 6, 6,
				6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6,
		};
		player->InitScore();
	}

	if (stage == 6)
	{
		player->level = 6;
		player->wincount = WINCOUNT_LVL6;
		player->steps = 0;
		player->lost = false;
		player->won = false;

		pos.x = 9 * TILE_SIZE;
		pos.y = 5 * TILE_SIZE - 1;

		map = new int[size] {
			6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6,
				6, 6, 6, 6, 2, 3, 3, 3, 3, 2, 6,
				6, 6, 2, 3, 3, 5, 5, 5, 5, 2, 6,
				6, 2, 3, 7, 5, 1, 3, 3, 5, 3, 2,
				6, 2, 7, 7, 1, 5, 1, 5, 5, 5, 2,
				6, 2, 7, 7, 5, 1, 5, 1, 5, 2, 3,
				6, 3, 3, 3, 3, 3, 2, 5, 5, 2, 6,
				6, 6, 6, 6, 6, 6, 3, 3, 3, 3, 6,
				6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6,
				6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6,
				6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6,
		};

		mapAux = new int[size] {
			6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6,
				6, 6, 6, 6, 2, 3, 3, 3, 3, 2, 6,
				6, 6, 2, 3, 3, 5, 5, 5, 5, 2, 6,
				6, 2, 3, 7, 5, 5, 3, 3, 5, 3, 2,
				6, 2, 7, 7, 5, 5, 5, 5, 5, 5, 2,
				6, 2, 7, 7, 5, 5, 5, 5, 5, 2, 3,
				6, 3, 3, 3, 3, 3, 2, 5, 5, 2, 6,
				6, 6, 6, 6, 6, 6, 3, 3, 3, 3, 6,
				6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6,
				6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6,
				6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6,
		};
		player->InitScore();
	}

	if (stage == 7)
	{
		player->level = 7;
		player->wincount = WINCOUNT_LVL7;
		player->steps = 0;
		player->lost = false;
		player->won = false;

		pos.x = 2 * TILE_SIZE;
		pos.y = 5 * TILE_SIZE - 1;

		map = new int[size] {
			6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6,
				6, 6, 6, 2, 3, 3, 3, 3, 2, 6, 6,
				6, 6, 6, 2, 5, 5, 5, 5, 2, 6, 6,
				6, 2, 3, 3, 1, 1, 1, 5, 2, 6, 6,
				6, 2, 5, 5, 1, 7, 7, 5, 2, 6, 6,
				6, 2, 5, 1, 7, 7, 7, 2, 3, 6, 6,
				6, 3, 3, 3, 2, 5, 5, 2, 6, 6, 6,
				6, 6, 6, 6, 3, 3, 3, 3, 6, 6, 6,
				6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6,
				6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6,
		};

		mapAux = new int[size] {
			6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6,
				6, 6, 6, 2, 3, 3, 3, 3, 2, 6, 6,
				6, 6, 6, 2, 5, 5, 5, 5, 2, 6, 6,
				6, 2, 3, 3, 5, 5, 5, 5, 2, 6, 6,
				6, 2, 5, 5, 5, 7, 7, 5, 2, 6, 6,
				6, 2, 5, 5, 7, 7, 7, 2, 3, 6, 6,
				6, 3, 3, 3, 2, 5, 5, 2, 6, 6, 6,
				6, 6, 6, 6, 3, 3, 3, 3, 6, 6, 6,
				6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6,
				6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6,
		};
		player->InitScore();
	}

	if (stage == 8)
	{
		player->level = 8;
		player->wincount = WINCOUNT_LVL8;
		player->steps = 0;
		player->lost = false;
		player->won = false;

		pos.x = 7 * TILE_SIZE;
		pos.y = 8 * TILE_SIZE - 1;

		map = new int[size] {
			6, 6, 2, 3, 3, 3, 2, 3, 3, 2, 6,
				6, 6, 2, 5, 5, 5, 3, 7, 5, 2, 6,
				6, 2, 3, 5, 5, 1, 7, 7, 7, 2, 6,
				6, 2, 5, 5, 1, 5, 2, 4, 7, 2, 6,
				2, 3, 5, 3, 3, 1, 3, 5, 3, 2, 6,
				2, 5, 5, 5, 1, 5, 5, 1, 5, 2, 6,
				2, 5, 5, 5, 2, 5, 5, 5, 5, 2, 6,
				3, 3, 3, 3, 3, 3, 2, 5, 5, 2, 6,
				6, 6, 6, 6, 6, 6, 3, 3, 3, 3, 6,
				6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6,
		};

		mapAux = new int[size] {
			6, 6, 2, 3, 3, 3, 2, 3, 3, 2, 6,
				6, 6, 2, 5, 5, 5, 3, 7, 5, 2, 6,
				6, 2, 3, 5, 5, 5, 7, 7, 7, 2, 6,
				6, 2, 5, 5, 5, 5, 2, 7, 7, 2, 6,
				2, 3, 5, 3, 3, 5, 3, 5, 3, 2, 6,
				2, 5, 5, 5, 5, 5, 5, 5, 5, 2, 6,
				2, 5, 5, 5, 2, 5, 5, 5, 5, 2, 6,
				3, 3, 3, 3, 3, 3, 2, 5, 5, 2, 6,
				6, 6, 6, 6, 6, 6, 3, 3, 3, 3, 6,
				6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6,
		};
		player->InitScore();
	}

	//else
	//{
	//	//Error level doesn't exist or incorrect level number
	//	LOG("Failed to load level, stage %d doesn't exist", stage);
	//	return AppStatus::ERROR;
	//}

	//Entities and objects
	i = 0;
	for (y = 0; y < LEVEL_HEIGHT; ++y)
	{
		for (x = 0; x < LEVEL_WIDTH; ++x)
		{
			tile = (Tile)map[i];
			if (tile == Tile::EMPTY)
			{
				map[i] = 0;
			}
			else if (tile == Tile::PLAYER)
			{
				pos.x = x * TILE_SIZE;
				pos.y = y * TILE_SIZE + TILE_SIZE - 1;
				player->SetPos(pos);
				map[i] = 0;
			}
			else if (tile == Tile::COMPLETION)
			{
				pos.x = x * TILE_SIZE;
				pos.y = y * TILE_SIZE + TILE_SIZE - 1;
				objects.push_back(obj);
				map[i] = 0;
			}
			++i;
		}
	}

	i = 0;
	for (y = 0; y < LEVEL_HEIGHT; ++y)
	{
		for (x = 0; x < LEVEL_WIDTH; ++x)
		{
			tile = (Tile)mapAux[i];
			if (tile == Tile::EMPTY)
			{
				mapAux[i] = 0;
			}
			else if (tile == Tile::PLAYER)
			{
				pos.x = x * TILE_SIZE;
				pos.y = y * TILE_SIZE + TILE_SIZE - 1;
				mapAux[i] = 0;
			}
			else if (tile == Tile::COMPLETION)
			{
				pos.x = x * TILE_SIZE;
				pos.y = y * TILE_SIZE + TILE_SIZE - 1;;
				objects.push_back(obj);
				mapAux[i] = 0;
			}
			++i;
		}
	}
	player->SetPos(pos);
	//Tile map
	level->Load(map, LEVEL_WIDTH, LEVEL_HEIGHT);
	Auxlevel->Load(mapAux, LEVEL_WIDTH, LEVEL_HEIGHT);


	return AppStatus::OK;
}
void Scene::Update()
{
	Point p1, p2;
	AABB box;

	//Switch between the different debug modes: off, on (sprites & hitboxes), on (hitboxes) 
	if (IsKeyPressed(KEY_F1))
	{
		debug = (DebugMode)(((int)debug + 1) % (int)DebugMode::SIZE);
	}
	//Debug levels instantly
	if (IsKeyPressed(KEY_R))
	{
		LoadLevel(CurrentStage);
	}

	level->Update();
	player->Update();
	CheckCollisions();
}
void Scene::Render()
{
	BeginMode2D(camera);

	level->Render();
	if (debug == DebugMode::OFF || debug == DebugMode::SPRITES_AND_HITBOXES)
	{
		RenderObjects();
		player->Draw();
	}
	if (debug == DebugMode::SPRITES_AND_HITBOXES || debug == DebugMode::ONLY_HITBOXES)
	{
		RenderObjectsDebug(YELLOW);
		player->DrawDebug(GREEN);
	}

	EndMode2D();

	RenderGUI();
}
void Scene::Release()
{
	level->Release();
	Auxlevel->Release();
	player->Release();
	ClearLevel();
}
void Scene::CheckCollisions()
{
	AABB player_box, obj_box;

	player_box = player->GetHitbox();
	auto it = objects.begin();
	while (it != objects.end())
	{
		obj_box = (*it)->GetHitbox();
		if (player_box.TestAABB(obj_box))
		{
			player->IncrScore((*it)->Points());

			//Delete the object
			delete* it;
			//Erase the object from the vector and get the iterator to the next valid element
			it = objects.erase(it);
		}
		else
		{
			//Move to the next object
			++it;
		}
	}
}
void Scene::ClearLevel()
{
	for (Object* obj : objects)
	{
		delete obj;
	}
	objects.clear();
}
void Scene::RenderObjects() const
{
	for (Object* obj : objects)
	{
		obj->Draw();
	}
}
void Scene::RenderObjectsDebug(const Color& col) const
{
	for (Object* obj : objects)
	{
		obj->DrawDebug(col);
	}
}
void Scene::RenderGUI() const
{
	if (player->PushingLeft)
	{
		level->DrawBox(player->x - TILE_SIZE, player->y - TILE_SIZE + 1);
	}
	if (player->PushingRight)
	{
		level->DrawBox(player->x + TILE_SIZE, player->y - TILE_SIZE + 1);
	}
	if (player->PushingUp)
	{
		level->DrawBox(player->x, player->y - TILE_SIZE * 2);
	}
	if (player->PushingDown)
	{
		level->DrawBox(player->x, player->y);
	}
	if (player->won)
	{
		level->DrawWin();
	}
	if (player->lost)
	{
		level->DrawLose();
	}

	font->Draw(10, 24, TextFormat("*/+'", player->steps), WHITE);
	font->Draw(10, 10, TextFormat("*/()+", player->GetScore()), WHITE);
	font->Draw(10, 34, TextFormat(",.-./", player->steps), WHITE);

	if (player->steps < 10)
	{
		font->Draw(52, 24, TextFormat("000%i", player->steps), WHITE);
	}
	else if (player->steps < 100)
	{
		font->Draw(52, 24, TextFormat("00%i", player->steps), WHITE);
	}
	else if (player->steps < 1000)
	{
		font->Draw(52, 24, TextFormat("0%i", player->steps), WHITE);
	}
	if (player->level == 1)
	{
		font->Draw(68, 10, TextFormat("01", player->GetScore()), WHITE);
		font->Draw(52, 34, TextFormat("0090", player->steps), WHITE);
	}
	if (player->level == 2)
	{
		font->Draw(68, 10, TextFormat("02", player->GetScore()), WHITE);
		font->Draw(52, 34, TextFormat("0120", player->steps), WHITE);
	}
	if (player->level == 3)
	{
		font->Draw(68, 10, TextFormat("03", player->GetScore()), WHITE);
		font->Draw(52, 34, TextFormat("0050", player->steps), WHITE);
	}
	if (player->level == 4)
	{
		font->Draw(68, 10, TextFormat("04", player->GetScore()), WHITE);
		font->Draw(52, 34, TextFormat("0140", player->steps), WHITE);
	}
	if (player->level == 5)
	{
		font->Draw(68, 10, TextFormat("05", player->GetScore()), WHITE);
		font->Draw(52, 34, TextFormat("0065", player->steps), WHITE);
	}
	if (player->level == 6)
	{
		font->Draw(68, 10, TextFormat("06", player->GetScore()), WHITE);
		font->Draw(52, 34, TextFormat("0080", player->steps), WHITE);
	}
	if (player->level == 7)
	{
		font->Draw(68, 10, TextFormat("07", player->GetScore()), WHITE);
		font->Draw(52, 34, TextFormat("0080", player->steps), WHITE);
	}
	if (player->level == 8)
	{
		font->Draw(68, 10, TextFormat("08", player->GetScore()), WHITE);
		font->Draw(52, 34, TextFormat("0160", player->steps), WHITE);
	}
}