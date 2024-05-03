#pragma once
#include <raylib.h>
#include "Sprite.h"
#include "Point.h"
#include "AABB.h"
#include "Globals.h"
#include <unordered_map>

enum class Tile {

	// -1: area covered by an entity
	EMPTY = -1,
	//  0: air tile
	AIR = 0,

	BOX = 1,
	BRICK = 2,
	BRICKB = 3,
	BOXU = 4,
	FLOOR = 5,
	OUT = 6,
	FLOORRED = 7,
	COMPLETION = 8,

	// id >= 100: entities' initial locations
	PLAYER = 100,

	STATIC_FIRST = FLOOR,
	STATIC_LAST = FLOORRED,
	SOLID_FIRST = BOX,
	SOLID_LAST = BOXU,
	SPECIAL_FIRST = FLOORRED,
	SPECIAL_LAST = FLOORRED,

	//Intervals
	ENTITY_FIRST = PLAYER,
	ENTITY_LAST = PLAYER
};

class TileMap
{
public:
	TileMap();
	~TileMap();

	const Texture2D* img_box;

	AppStatus Initialise();
	AppStatus Load(int data[], int w, int h);
	void Update();
	void Render();
	void Release();
	bool CheckWin(int winCount);

	Tile GetTileIndex(int x, int y) const;
	Tile GetOldTileIndex(int x, int y) const;
	void ChangeTile(int x, int y, Tile t) const;

	//Test for collisions with walls
	bool TestCollisionWallLeft(const AABB& box) const;
	bool TestCollisionWallRight(const AABB& box) const;
	bool TestCollisionWallUp(const AABB& box) const;
	bool TestCollisionWallDown(const AABB& box) const;

	bool TestBoxLeft(const AABB& box) const;
	bool TestBoxRight(const AABB& box) const;
	bool TestBoxUp(const AABB& box) const;
	bool TestBoxDown(const AABB& box) const;

	bool MoveBoxRight(const AABB& box) const;
	bool MoveBoxLeft(const AABB& box) const;
	bool MoveBoxUp(const AABB& box) const;
	bool MoveBoxDown(const AABB& box) const;

	void DrawBox(int x, int y) const;
	void DrawWin() const;
	void DrawLose() const;


private:
	void InitTileDictionary();

	bool IsTileSolid(Tile tile) const;
	bool IsTileBox(Tile tile) const;
	bool IsTileGoal(Tile tile) const;

	bool CollisionX(const Point& p, int distance) const;
	bool CollisionY(const Point& p, int distance) const;

	bool CollisionBoxX(const Point& p, int distance) const;
	bool CollisionBoxY(const Point& p, int distance) const;

	bool BoxRight(const Point& p, int distance) const;
	bool BoxLeft(const Point& p, int distance) const;
	bool BoxUp(const Point& p, int distance) const;
	bool BoxDown(const Point& p, int distance) const;

	//Tile map
	Tile* map;
	Tile* mapAux;

	//Size of the tile map
	int size, width, height;

	//Dictionary of tile frames
	std::unordered_map<int, Rectangle> dict_rect;

	//Tile sheet
	const Texture2D* img_tiles;
	const Texture2D* img_hud;
	const Texture2D* img_win;
	const Texture2D* img_lose;
};
