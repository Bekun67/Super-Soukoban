
#include "Player.h"
#include "Sprite.h"
#include "TileMap.h"
#include "Globals.h"
#include <raymath.h>

Player::Player(const Point& p, State s, Look view) :
	Entity(p, PLAYER_PHYSICAL_WIDTH, PLAYER_PHYSICAL_HEIGHT, PLAYER_FRAME_SIZE, PLAYER_FRAME_SIZE)
{
	state = s;
	look = view;
	map = nullptr;
	mapAux = nullptr;

	score = 0;
}
Player::~Player()
{
}
AppStatus Player::Initialise()
{
	int i;
	const int n = PLAYER_FRAME_SIZE;

	ResourceManager& data = ResourceManager::Instance();
	if (data.LoadTexture(Resource::IMG_PLAYER, "images/soukobanguy.png") != AppStatus::OK)
	{
		return AppStatus::ERROR;
	}

	render = new Sprite(data.GetTexture(Resource::IMG_PLAYER));
	if (render == nullptr)
	{
		LOG("Failed to allocate memory for player sprite");
		return AppStatus::ERROR;
	}

	Sprite* sprite = dynamic_cast<Sprite*>(render);
	sprite->SetNumberAnimations((int)PlayerAnim::NUM_ANIMATIONS);

	sprite->SetAnimationDelay((int)PlayerAnim::IDLE_RIGHT, ANIM_DELAY);
	sprite->AddKeyFrame((int)PlayerAnim::IDLE_RIGHT, { 0, n * 5, n, n });
	sprite->SetAnimationDelay((int)PlayerAnim::IDLE_LEFT, ANIM_DELAY);
	sprite->AddKeyFrame((int)PlayerAnim::IDLE_LEFT, { 0, n * 5, -n, n });
	sprite->SetAnimationDelay((int)PlayerAnim::IDLE_UP, ANIM_DELAY);
	sprite->AddKeyFrame((int)PlayerAnim::IDLE_UP, { 0,  n * 4, n, n });
	sprite->SetAnimationDelay((int)PlayerAnim::IDLE_DOWN, ANIM_DELAY);
	sprite->AddKeyFrame((int)PlayerAnim::IDLE_DOWN, { 0, n * 3, n, n });

	sprite->SetAnimationDelay((int)PlayerAnim::WALKING_RIGHT, ANIM_DELAY);
	for (i = 0; i < 9; ++i)
		sprite->AddKeyFrame((int)PlayerAnim::WALKING_RIGHT, { (float)i * n, n * 5,  n, n });
	sprite->SetAnimationDelay((int)PlayerAnim::WALKING_LEFT, ANIM_DELAY);
	for (i = 0; i < 9; ++i)
		sprite->AddKeyFrame((int)PlayerAnim::WALKING_LEFT, { (float)i * n, n * 5,  -n, n });

	sprite->SetAnimationDelay((int)PlayerAnim::WALKING_UP, ANIM_DELAY);
	for (i = 0; i < 9; ++i)
		sprite->AddKeyFrame((int)PlayerAnim::WALKING_UP, { (float)i * n, n * 4,  n, n });
	sprite->SetAnimationDelay((int)PlayerAnim::WALKING_DOWN, ANIM_DELAY);
	for (i = 0; i < 9; ++i)
		sprite->AddKeyFrame((int)PlayerAnim::WALKING_DOWN, { (float)i * n, n * 3,  n, n });

	sprite->SetAnimationDelay((int)PlayerAnim::PUSHING_LEFT, ANIM_DELAY);
	for (i = 0; i < 7; ++i)
		sprite->AddKeyFrame((int)PlayerAnim::PUSHING_LEFT, { (float)i * n, 0, -n, n });
	sprite->SetAnimationDelay((int)PlayerAnim::PUSHING_RIGHT, ANIM_DELAY);
	for (i = 0; i < 7; ++i)
		sprite->AddKeyFrame((int)PlayerAnim::PUSHING_RIGHT, { (float)i * n, 0,  n, n });

	sprite->SetAnimationDelay((int)PlayerAnim::PUSHING_UP, ANIM_DELAY);
	for (i = 0; i < 7; ++i)
		sprite->AddKeyFrame((int)PlayerAnim::PUSHING_UP, { (float)i * n, n,  n, n });
	sprite->SetAnimationDelay((int)PlayerAnim::PUSHING_DOWN, ANIM_DELAY);
	for (i = 0; i < 7; ++i)
		sprite->AddKeyFrame((int)PlayerAnim::PUSHING_DOWN, { (float)i * n, n * 2,  n, n });

	sprite->SetAnimation((int)PlayerAnim::IDLE_RIGHT);

	return AppStatus::OK;
}
void Player::InitScore()
{
	score = 0;
}
void Player::IncrScore(int n)
{
	score += n;
}
int Player::GetScore()
{
	return score;
}
void Player::SetTileMap(TileMap* tilemap)
{
	map = tilemap;
}
void Player::SetAuxTileMap(TileMap* tilemap)
{
	mapAux = tilemap;
}
bool Player::IsLookingRight() const
{
	return look == Look::RIGHT;
}
bool Player::IsLookingLeft() const
{
	return look == Look::LEFT;
}
bool Player::IsLookingUp() const
{
	return look == Look::UP;
}
bool Player::IsLookingDown() const
{
	return look == Look::DOWN;
}
bool Player::IsMoving() const
{
	return state == State::WALKING;
}
void Player::SetAnimation(int id)
{
	Sprite* sprite = dynamic_cast<Sprite*>(render);
	sprite->SetAnimation(id);
}
PlayerAnim Player::GetAnimation()
{
	Sprite* sprite = dynamic_cast<Sprite*>(render);
	return (PlayerAnim)sprite->GetAnimation();
}
void Player::Stop()
{
	dir = { 0,0 };
	state = State::IDLE;
	if (IsLookingRight())	    SetAnimation((int)PlayerAnim::IDLE_RIGHT);
	else if (IsLookingLeft())	SetAnimation((int)PlayerAnim::IDLE_LEFT);
	else if (IsLookingUp())  	SetAnimation((int)PlayerAnim::IDLE_UP);
	else if (IsLookingDown())	SetAnimation((int)PlayerAnim::IDLE_DOWN);
}
void Player::StartWalkingLeft()
{
	state = State::WALKING;
	look = Look::LEFT;
	SetAnimation((int)PlayerAnim::WALKING_LEFT);
}
void Player::StartWalkingRight()
{
	state = State::WALKING;
	look = Look::RIGHT;
	SetAnimation((int)PlayerAnim::WALKING_RIGHT);
}
void Player::StartWalkingUp()
{
	state = State::WALKING;
	look = Look::UP;
	SetAnimation((int)PlayerAnim::WALKING_UP);
}
void Player::StartWalkingDown()
{
	state = State::WALKING;
	look = Look::DOWN;
	SetAnimation((int)PlayerAnim::WALKING_DOWN);
}
void Player::StartPushingLeft()
{
	state = State::PUSHING;
	look = Look::LEFT;
	SetAnimation((int)PlayerAnim::PUSHING_LEFT);
}
void Player::StartPushingRight()
{
	state = State::PUSHING;
	look = Look::RIGHT;
	SetAnimation((int)PlayerAnim::PUSHING_RIGHT);
}
void Player::StartPushingUp()
{
	state = State::PUSHING;
	look = Look::UP;
	SetAnimation((int)PlayerAnim::PUSHING_UP);
}
void Player::StartPushingDown()
{
	state = State::PUSHING;
	look = Look::DOWN;
	SetAnimation((int)PlayerAnim::PUSHING_DOWN);
}
void Player::ChangeAnimRight()
{
	look = Look::RIGHT;
	switch (state)
	{
	case State::IDLE:	 SetAnimation((int)PlayerAnim::IDLE_RIGHT);    break;
	case State::WALKING: SetAnimation((int)PlayerAnim::WALKING_RIGHT); break;
	}
}
void Player::ChangeAnimLeft()
{
	look = Look::LEFT;
	switch (state)
	{
	case State::IDLE:	 SetAnimation((int)PlayerAnim::IDLE_LEFT);    break;
	case State::WALKING: SetAnimation((int)PlayerAnim::WALKING_LEFT); break;
	}
}
void Player::ChangeAnimUp()
{
	look = Look::UP;
	switch (state)
	{
	case State::IDLE:	 SetAnimation((int)PlayerAnim::IDLE_UP);    break;
	case State::WALKING: SetAnimation((int)PlayerAnim::WALKING_UP); break;
	}
}
void Player::ChangeAnimDown()
{
	look = Look::DOWN;
	switch (state)
	{
	case State::IDLE:	 SetAnimation((int)PlayerAnim::IDLE_DOWN);    break;
	case State::WALKING: SetAnimation((int)PlayerAnim::WALKING_DOWN); break;
	}
}
void Player::Update()
{
	//Player doesn't use the "Entity::Update() { pos += dir; }" default behaviour.
	//Instead, uses an independent behaviour for each axis.
	MoveX();
	MoveY();

	Sprite* sprite = dynamic_cast<Sprite*>(render);
	sprite->Update();
}

void Player::MoveLeft()
{
}

void Player::MoveX()
{
	AABB box;
	int prev_x = pos.x;

	if (IsKeyDown(KEY_LEFT) && !IsKeyDown(KEY_RIGHT) && !IsKeyDown(KEY_UP) && !IsKeyDown(KEY_DOWN))
	{
		pos.x += -PLAYER_SPEED;
		if (state == State::IDLE)
		{
			StartWalkingLeft();
		}
		else
		{
			if (IsLookingRight() or IsLookingDown() or IsLookingUp()) ChangeAnimLeft();
		}

		box = GetHitbox();
		if (map->TestCollisionWallLeft(box))
		{
			pos.x = prev_x;
			if (state == State::WALKING) Stop();
		}
		if (map->TestBoxLeft(box))
		{
			StartPushingLeft();
			pos.x = prev_x;
			if (state == State::WALKING) Stop();
			if (map->MoveBoxLeft(box))
			{
				int x1 = (pos.x / TILE_SIZE) - 1;
				int x2 = (pos.x / TILE_SIZE) - 2;

				Tile nextTile = map->GetTileIndex(x1, pos.y / TILE_SIZE);
				map->ChangeTile(x2, pos.y / TILE_SIZE, nextTile);

				Tile OldTile = mapAux->GetOldTileIndex(x1, pos.y / TILE_SIZE);
				map->ChangeTile(x1, pos.y / TILE_SIZE, OldTile);

				Tile Destination = mapAux->GetOldTileIndex(x2, pos.y / TILE_SIZE);
				if (Destination == Tile::FLOORRED)
				{
					map->ChangeTile(x2, pos.y / TILE_SIZE, Tile::BOXU);
					map->ChangeTile(x1, pos.y / TILE_SIZE, OldTile);
				}

				if (OldTile == Tile::FLOORRED)
				{
					nextTile = Tile::BOXU;
					map->ChangeTile(x2, pos.y / TILE_SIZE, nextTile);
					map->CheckWin(WINCOUNT_LVL1);
				}
			}
		}
		if (state == State::PUSHING) Stop();
	}
	else if (IsKeyDown(KEY_RIGHT) && !IsKeyDown(KEY_UP) && !IsKeyDown(KEY_DOWN))
	{
		pos.x += PLAYER_SPEED;
		if (state == State::IDLE) StartWalkingRight();
		else
		{
			if (IsLookingLeft() or IsLookingDown() or IsLookingUp()) ChangeAnimRight();
		}

		box = GetHitbox();
		if (map->TestCollisionWallRight(box))
		{
			pos.x = prev_x;
			if (state == State::WALKING) Stop();
		}
		if (map->TestBoxRight(box))
		{
			StartPushingRight();
			pos.x = prev_x;
			if (state == State::WALKING) Stop();
			if (map->MoveBoxRight(box))
			{
				int x1 = (pos.x / TILE_SIZE) + 1;
				int x2 = (pos.x / TILE_SIZE) + 2;

				Tile nextTile = map->GetTileIndex(x1, pos.y / TILE_SIZE);
				map->ChangeTile(x2, pos.y / TILE_SIZE, nextTile);

				Tile OldTile = mapAux->GetOldTileIndex(x1, pos.y / TILE_SIZE);
				map->ChangeTile(x1, pos.y / TILE_SIZE, OldTile);

				Tile Destination = mapAux->GetOldTileIndex(x2, pos.y / TILE_SIZE);
				if (Destination == Tile::FLOORRED)
				{
					map->ChangeTile(x2, pos.y / TILE_SIZE, Tile::BOXU);
					map->ChangeTile(x1, pos.y / TILE_SIZE, OldTile);
					map->CheckWin(WINCOUNT_LVL1);
				}
			}
		}
		if (state == State::PUSHING) Stop();
	}
	else
	{
		if (state == State::WALKING) Stop();
	}
}
void Player::MoveY()
{
	AABB box;
	int prev_y = pos.y;

	if (IsKeyDown(KEY_UP) && !IsKeyDown(KEY_DOWN) && !IsKeyDown(KEY_RIGHT) && !IsKeyDown(KEY_LEFT))
	{
		pos.y += -PLAYER_SPEED;
		if (state == State::IDLE) StartWalkingUp();
		else
		{
			if (IsLookingRight() or IsLookingDown() or IsLookingLeft()) ChangeAnimUp();
		}

		box = GetHitbox();
		if (map->TestCollisionWallUp(box))
		{
			pos.y = prev_y;
			if (state == State::WALKING) Stop();
		}
		if (map->TestBoxUp(box))
		{
			StartPushingUp();
			pos.y = prev_y;
			if (state == State::WALKING) Stop();
			if (map->MoveBoxUp(box))
			{
				int y1 = (pos.y / TILE_SIZE) - 1;
				int y2 = (pos.y / TILE_SIZE) - 2;

				Tile nextTile = map->GetTileIndex(pos.x / TILE_SIZE, y1);
				map->ChangeTile(pos.x / TILE_SIZE, y2, nextTile);

				Tile OldTile = mapAux->GetOldTileIndex(pos.x / TILE_SIZE, y1);
				map->ChangeTile(pos.x / TILE_SIZE, y1, OldTile);

				Tile Destination = mapAux->GetOldTileIndex(pos.x / TILE_SIZE, y2);
				if (Destination == Tile::FLOORRED)
				{
					map->ChangeTile(pos.x / TILE_SIZE, y2, Tile::BOXU);
					map->ChangeTile(pos.x / TILE_SIZE, y1, OldTile);
					map->CheckWin(WINCOUNT_LVL1);
				}
			}
		}
		if (state == State::PUSHING) Stop();

	}
	else if (IsKeyDown(KEY_DOWN) && !IsKeyDown(KEY_RIGHT) && !IsKeyDown(KEY_LEFT))
	{
		pos.y += PLAYER_SPEED;
		if (state == State::IDLE) StartWalkingDown();
		else
		{
			if (IsLookingRight() or IsLookingLeft() or IsLookingUp()) ChangeAnimDown();
		}

		box = GetHitbox();
		if (map->TestCollisionWallDown(box))
		{
			pos.y = prev_y;
			if (state == State::WALKING) Stop();
		}
		if (map->TestBoxDown(box))
		{
			StartPushingDown();
			pos.y = prev_y;
			if (state == State::WALKING) Stop();
			if (map->MoveBoxDown(box))
			{
				int y1 = (pos.y / TILE_SIZE) + 1;
				int y2 = (pos.y / TILE_SIZE) + 2;

				Tile nextTile = map->GetTileIndex(pos.x / TILE_SIZE, y1);
				map->ChangeTile(pos.x / TILE_SIZE, y2, nextTile);

				Tile OldTile = mapAux->GetOldTileIndex(pos.x / TILE_SIZE, y1);
				map->ChangeTile(pos.x / TILE_SIZE, y1, OldTile);

				Tile Destination = mapAux->GetOldTileIndex(pos.x / TILE_SIZE, y2);
				if (Destination == Tile::FLOORRED)
				{
					map->ChangeTile(pos.x / TILE_SIZE, y2, Tile::BOXU);
					map->ChangeTile(pos.x / TILE_SIZE, y1, OldTile);
					map->CheckWin(WINCOUNT_LVL1);
				}
			}
		}
		if (state == State::PUSHING) Stop();
	}
	else
	{
		if (state == State::WALKING) Stop();
	}
}
void Player::DrawDebug(const Color& col) const
{
	Entity::DrawHitbox(pos.x, pos.y, width, height, col);

	DrawText(TextFormat("Position: (%d,%d)\nSize: %dx%d\nFrame: %dx%d", pos.x, pos.y, width, height, frame_width, frame_height), 18 * 16, 0, 8, LIGHTGRAY);
	DrawPixel(pos.x, pos.y, WHITE);
}
void Player::Release()
{
	ResourceManager& data = ResourceManager::Instance();
	data.ReleaseTexture(Resource::IMG_PLAYER);

	render->Release();
}