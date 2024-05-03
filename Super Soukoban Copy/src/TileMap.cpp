#include "TileMap.h"
#include "Globals.h"
#include "ResourceManager.h"
#include <cstring>

Music soundMusicWoL[10];

TileMap::TileMap()
{
	map = nullptr;
	mapAux = nullptr;
	width = 0;
	height = 0;
	img_tiles = nullptr;
	img_hud = nullptr;
	img_box = nullptr;
	img_win = nullptr;
	img_lose = nullptr;
	img_stage = nullptr;
	img_step = nullptr;
	img_limit = nullptr;

	InitTileDictionary();
}
TileMap::~TileMap()
{
	if (map != nullptr)
	{
		delete[] map;
		map = nullptr;
	}
	if (mapAux != nullptr)
	{
		delete[] mapAux;
		mapAux = nullptr;
	}
}
void TileMap::InitTileDictionary()
{
	const int n = TILE_SIZE;

	dict_rect[(int)Tile::BOX] = { 0,  0, n, n };

	dict_rect[(int)Tile::BRICK] = { n,  0, n, n };

	dict_rect[(int)Tile::BRICKB] = { n * 2, 0, n, n };

	dict_rect[(int)Tile::OUT] = { n, n, n, n };

	dict_rect[(int)Tile::FLOOR] = { 0, n, n, n };

	dict_rect[(int)Tile::BOXU] = { n * 3, 0, n, n };

	dict_rect[(int)Tile::FLOORRED] = { n * 2, n, n, n };
}
AppStatus TileMap::Initialise()
{
	ResourceManager& data = ResourceManager::Instance();

	if (data.LoadTexture(Resource::IMG_TILES, "images/tileset.png") != AppStatus::OK)
	{
		return AppStatus::ERROR;
	}
	img_tiles = data.GetTexture(Resource::IMG_TILES);

	if (data.LoadTexture(Resource::IMG_HUD, "images/hud.png") != AppStatus::OK)
	{
		return AppStatus::ERROR;
	}
	img_hud = data.GetTexture(Resource::IMG_HUD);

	if (data.LoadTexture(Resource::IMG_STEP, "images/step.png") != AppStatus::OK)
	{
		return AppStatus::ERROR;
	}
	img_step = data.GetTexture(Resource::IMG_STEP);
	if (data.LoadTexture(Resource::IMG_LIMIT, "images/limit.png") != AppStatus::OK)
	{
		return AppStatus::ERROR;
	}
	img_limit = data.GetTexture(Resource::IMG_LIMIT);
	if (data.LoadTexture(Resource::IMG_STAGE, "images/stage.png") != AppStatus::OK)
	{
		return AppStatus::ERROR;
	}
	img_stage = data.GetTexture(Resource::IMG_STAGE);

	if (data.LoadTexture(Resource::IMG_BOX, "images/box.png") != AppStatus::OK)
	{
		return AppStatus::ERROR;
	}
	img_box = data.GetTexture(Resource::IMG_BOX);

	if (data.LoadTexture(Resource::IMG_WIN, "images/win.png") != AppStatus::OK)
	{
		return AppStatus::ERROR;
	}
	img_win = data.GetTexture(Resource::IMG_WIN);

	if (data.LoadTexture(Resource::IMG_LOSE, "images/lose.png") != AppStatus::OK)
	{
		return AppStatus::ERROR;
	}
	img_lose = data.GetTexture(Resource::IMG_LOSE);

	return AppStatus::OK;
}
AppStatus TileMap::Load(int data[], int w, int h)
{
	size = w * h;
	width = w;
	height = h;

	if (map != nullptr)	delete[] map;

	map = new Tile[size];
	if (map == nullptr)
	{
		LOG("Failed to allocate memory for tile map");
		return AppStatus::ERROR;
	}
	memcpy(map, data, size * sizeof(int));

	if (mapAux != nullptr)	delete[] mapAux;

	mapAux = new Tile[size];
	if (mapAux == nullptr)
	{
		LOG("Failed to allocate memory for tile map");
		return AppStatus::ERROR;
	}
	memcpy(mapAux, data, size * sizeof(int));

	return AppStatus::OK;
}
void TileMap::Update()
{

}

bool TileMap::CheckWin(int winCount)
{
	Tile tile;
	int count = 0;
	for (int i = 0; i < height; ++i)
	{
		for (int j = 0; j < width; ++j)
		{
			tile = map[i * width + j];
			if (tile == Tile::BOXU)
			{
				count++;
			}
		}
	}
	if (count == winCount)
	{
		//winCondition activa
		return true;
	}
	else
	{
		return false;
	}
}

Tile TileMap::GetTileIndex(int x, int y) const
{
	int idx = x + y * width;
	if (idx < 0 || idx >= size)
	{
		LOG("Error: Index out of bounds. Tile map dimensions: %dx%d. Given index: (%d, %d)", width, height, x, y)
			return Tile::AIR;
	}
	return map[x + y * width];
}
Tile TileMap::GetOldTileIndex(int x, int y) const
{
	int idx = x + y * width;
	if (idx < 0 || idx >= size)
	{
		LOG("Error: Index out of bounds. Tile map dimensions: %dx%d. Given index: (%d, %d)", width, height, x, y)
			return Tile::AIR;
	}
	return mapAux[x + y * width];
}
void TileMap::ChangeTile(int x, int y, Tile t) const
{
	int idx = x + y * width;
	if (idx < 0 || idx >= size)
	{
		LOG("Error: Index out of bounds. Tile map dimensions: %dx%d. Given index: (%d, %d)", width, height, x, y)
	}
	map[x + y * width] = t;
}
bool TileMap::IsTileSolid(Tile tile) const
{
	return (Tile::SOLID_FIRST <= tile && tile <= Tile::SOLID_LAST);
}
bool TileMap::IsTileBox(Tile tile) const
{
	return (tile == Tile::BOX or tile == Tile::BOXU);
}
bool TileMap::IsTileGoal(Tile tile) const
{
	return (tile == Tile::FLOORRED);
}


bool TileMap::TestCollisionWallLeft(const AABB& box) const
{
	return CollisionX(box.pos, box.height);
}
bool TileMap::MoveBoxLeft(const AABB& box) const
{
	return BoxLeft(box.pos, box.height);
}
bool TileMap::TestBoxLeft(const AABB& box) const
{
	return CollisionBoxX(box.pos, box.height);
}


bool TileMap::TestCollisionWallRight(const AABB& box) const
{
	return CollisionX(box.pos + Point(box.width - 1, 0), box.height);
}
bool TileMap::MoveBoxRight(const AABB& box) const
{
	return BoxRight(box.pos, box.height);
}
bool TileMap::TestBoxRight(const AABB& box) const
{
	return CollisionBoxX(box.pos + Point(box.width - 1, 0), box.height);
}


bool TileMap::TestCollisionWallUp(const AABB& box) const
{
	if (CollisionY(box.pos + Point(box.height - 1, 0), box.width) == true or CollisionY(box.pos, box.width) == true)
	{
		return true;
	}
	else
	{
		return false;
	}
}
bool TileMap::MoveBoxUp(const AABB& box) const
{
	return BoxUp(box.pos, box.height);
}
bool TileMap::TestBoxUp(const AABB& box) const
{
	if (CollisionBoxY(box.pos + Point(box.height - 1, 0), box.width) == true or CollisionBoxY(box.pos, box.width) == true)
	{
		return true;
	}
	else
	{
		return false;
	}
}

bool TileMap::TestCollisionWallDown(const AABB& box) const
{
	if (CollisionY(box.pos + Point(box.height - 1, 0), box.width) == true or CollisionY(box.pos, box.width) == true)
	{
		return true;
	}
	else
	{
		return false;
	}
}
bool TileMap::MoveBoxDown(const AABB& box) const
{
	return BoxDown(box.pos, box.height);
}
bool TileMap::TestBoxDown(const AABB& box) const
{
	if (CollisionBoxY(box.pos + Point(box.height - 1, 0), box.width) == true or CollisionBoxY(box.pos, box.width) == true)
	{
		return true;
	}
	else
	{
		return false;
	}
}
bool TileMap::CollisionX(const Point& p, int distance) const
{
	int x, y, y0, y1;

	//Calculate the tile coordinates and the range of tiles to check for collision
	x = p.x / TILE_SIZE;
	y0 = p.y / TILE_SIZE;
	y1 = (p.y + distance - 1) / TILE_SIZE;

	//Iterate over the tiles within the vertical range
	for (y = y0; y <= y1; ++y)
	{
		//One solid tile is sufficient
		if (IsTileSolid(GetTileIndex(x, y)))
			return true;
	}
	return false;
}
bool TileMap::CollisionBoxX(const Point& p, int distance) const
{
	int x, y, y0, y1;

	//Calculate the tile coordinates and the range of tiles to check for collision
	x = p.x / TILE_SIZE;
	y0 = p.y / TILE_SIZE;
	y1 = (p.y + distance - 1) / TILE_SIZE;

	//Iterate over the tiles within the vertical range
	for (y = y0; y <= y1; ++y)
	{
		//One solid tile is sufficient
		if (IsTileBox(GetTileIndex(x, y)))
			return true;
	}
	return false;
}
bool TileMap::CollisionY(const Point& p, int distance) const
{
	int x, y, y0, y1;

	//Calculate the tile coordinates and the range of tiles to check for collision
	x = p.x / TILE_SIZE;
	y0 = p.y / TILE_SIZE;
	y1 = (p.y + distance - 1) / TILE_SIZE;

	//Iterate over the tiles within the vertical range
	for (y = y0; y <= y1; ++y)
	{
		//One solid tile is sufficient
		if (IsTileSolid(GetTileIndex(x, y)))
			return true;
	}
	return false;
}
bool TileMap::CollisionBoxY(const Point& p, int distance) const
{
	int x, y, y0, y1;

	//Calculate the tile coordinates and the range of tiles to check for collision
	x = p.x / TILE_SIZE;
	y0 = p.y / TILE_SIZE;
	y1 = (p.y + distance - 1) / TILE_SIZE;

	//Iterate over the tiles within the vertical range
	for (y = y0; y <= y1; ++y)
	{
		//One solid tile is sufficient
		if (IsTileBox(GetTileIndex(x, y)))
			return true;
	}
	return false;
}
bool TileMap::BoxRight(const Point& p, int distance) const
{
	int x, y, x1;

	//Calculate the tile coordinates and the range of tiles to check for collision
	x = p.x / TILE_SIZE;
	y = p.y / TILE_SIZE;
	x1 = p.x / TILE_SIZE + 2;

	if (IsTileSolid(GetTileIndex(x1, y)))
	{
		return false;
	}
	else
	{
		return true;
	}
}
bool TileMap::BoxLeft(const Point& p, int distance) const
{
	int x, y, x1;

	//Calculate the tile coordinates and the range of tiles to check for collision
	x = p.x / TILE_SIZE;
	y = p.y / TILE_SIZE;
	x1 = p.x / TILE_SIZE - 1;

	if (IsTileSolid(GetTileIndex(x1, y)))
	{
		return false;
	}
	else
	{
		return true;
	}
}
bool TileMap::BoxUp(const Point& p, int distance) const
{
	int x, y, y1;

	//Calculate the tile coordinates and the range of tiles to check for collision
	x = p.x / TILE_SIZE;
	y = p.y / TILE_SIZE;
	y1 = p.y / TILE_SIZE - 1;

	if (IsTileSolid(GetTileIndex(x, y1)))
	{
		return false;
	}
	else
	{
		return true;
	}
}
bool TileMap::BoxDown(const Point& p, int distance) const
{
	int x, y, y1;

	//Calculate the tile coordinates and the range of tiles to check for collision
	x = p.x / TILE_SIZE;
	y = p.y / TILE_SIZE;
	y1 = p.y / TILE_SIZE + 2;

	if (IsTileSolid(GetTileIndex(x, y1)))
	{
		return false;
	}
	else
	{
		return true;
	}
}
void TileMap::Render()
{
	Tile tile;
	Rectangle rc;
	Vector2 pos;

	for (int i = 0; i < height; ++i)
	{
		for (int j = 0; j < width; ++j)
		{
			tile = map[i * width + j];
			if (tile != Tile::AIR)
			{
				pos.x = (float)j * TILE_SIZE;
				pos.y = (float)i * TILE_SIZE;

				if (tile != Tile::COMPLETION)
				{
					rc = dict_rect[(int)tile];
					DrawTextureRec(*img_tiles, rc, pos, WHITE);
				}
			}
		}
	}
	DrawTexture(*img_hud, 5, 7, WHITE);
	DrawTexture(*img_limit, 10, 34, WHITE);
	DrawTexture(*img_step, 45, 34, WHITE);
	DrawTexture(*img_stage, 10, 10, WHITE);


	//DrawTexture(*map->img_box, 5, 7, WHITE);
	//DrawBox();
}

void TileMap::DrawBox(int x, int y) const
{
	DrawTexture(*img_box, x, y, WHITE);
}

void TileMap::DrawWin() const
{
	DrawTexture(*img_win, 0, 0, WHITE);
}

void TileMap::DrawLose() const
{
	DrawTexture(*img_lose, 0, 0, WHITE);
}

void TileMap::Release()
{
	ResourceManager& data = ResourceManager::Instance();
	data.ReleaseTexture(Resource::IMG_TILES);
	data.ReleaseTexture(Resource::IMG_HUD);
	data.ReleaseTexture(Resource::IMG_BOX);
	data.ReleaseTexture(Resource::IMG_WIN);
	data.ReleaseTexture(Resource::IMG_LOSE);
	data.ReleaseTexture(Resource::IMG_STAGE);
	data.ReleaseTexture(Resource::IMG_STEP);
	data.ReleaseTexture(Resource::IMG_LIMIT);

	dict_rect.clear();
}