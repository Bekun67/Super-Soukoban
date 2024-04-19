#include "Scene.h"
#include <stdio.h>
#include "Globals.h"

Scene::Scene()
{
	player = nullptr;
	level = nullptr;
	Auxlevel = nullptr;

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
AppStatus Scene::Init()
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
	if (LoadLevel(1) != AppStatus::OK)
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

	ClearLevel();

	size = LEVEL_WIDTH * LEVEL_HEIGHT;
	if (stage == 1)
	{
		player->level = 1;
		player->steps = 0;
		player->lost = false;

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
	else
	{
		//Error level doesn't exist or incorrect level number
		LOG("Failed to load level, stage %d doesn't exist", stage);
		return AppStatus::ERROR;
	}

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
				obj = new Object(pos, ObjectType::APPLE);
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
				pos.y = y * TILE_SIZE + TILE_SIZE - 1;
				obj = new Object(pos, ObjectType::APPLE);
				objects.push_back(obj);
				mapAux[i] = 0;
			}
			++i;
		}
	}
	pos.x = 4 * TILE_SIZE;
	pos.y = 3 * TILE_SIZE - 1;
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
		LoadLevel(1);
	}
	//else if (IsKeyPressed(KEY_TWO))	LoadLevel(2);

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
	//Temporal approach
	if (player->level == 1)
	{
		DrawText(TextFormat("STAGE", player->GetScore()), 10, 10, 8, YELLOW);
		DrawText(TextFormat("01", player->GetScore()), 58, 10, 8, LIGHTGRAY);

		DrawText(TextFormat("STEP", player->steps), 10, 25, 8, YELLOW);

		if (player->steps < 10)
		{
			DrawText(TextFormat("000%i", player->steps), 45, 25, 8, LIGHTGRAY);
		}
		else if (player->steps < 100)
		{
			DrawText(TextFormat("00%i", player->steps), 45, 25, 8, LIGHTGRAY);
		}
		else if (player->steps < 1000)
		{
			DrawText(TextFormat("0%i", player->steps), 45, 25, 8, LIGHTGRAY);
		}
		DrawText(TextFormat("LIMIT", player->steps), 10, 35, 8, YELLOW);
		DrawText(TextFormat("0120", player->steps), 45, 35, 8, LIGHTGRAY);
	}
}