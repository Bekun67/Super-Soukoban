#pragma once
#include "Entity.h"
#include "TileMap.h"

//Representation model size: 32x32
#define PLAYER_FRAME_SIZE		32

//Logical model size: 12x28
#define PLAYER_PHYSICAL_WIDTH	32
#define PLAYER_PHYSICAL_HEIGHT	32

//Horizontal speed and vertical speed while falling down
#define PLAYER_SPEED			32

//Logic states
enum class State { IDLE, WALKING, PUSHING };
enum class Look { RIGHT, LEFT, UP, DOWN };

//Rendering states
enum class PlayerAnim {
	IDLE_LEFT, IDLE_RIGHT,
	IDLE_UP, IDLE_DOWN,
	WALKING_LEFT, WALKING_RIGHT,
	WALKING_UP, WALKING_DOWN,
	PUSHING_LEFT, PUSHING_RIGHT,
	PUSHING_UP, PUSHING_DOWN,

	NUM_ANIMATIONS
};

class Player: public Entity
{
public:
	Player(const Point& p, State s, Look view);
	~Player();
	
	AppStatus Initialise();
	void SetTileMap(TileMap* tilemap);

	void InitScore();
	void IncrScore(int n);
	int GetScore();

	void Update();
	void DrawDebug(const Color& col) const;
	void Release();

private:
	bool IsLookingRight() const;
	bool IsLookingLeft() const;
	bool IsLookingUp() const;
	bool IsLookingDown() const;

	void MoveLeft();

	bool IsMoving() const;

	//Player mechanics
	void MoveX();
	void MoveY();

	//Animation management
	void SetAnimation(int id);
	PlayerAnim GetAnimation();
	void Stop();

	void StartWalkingLeft();
	void StartWalkingRight();
	void StartWalkingUp();
	void StartWalkingDown();

	void ChangeAnimRight();
	void ChangeAnimLeft();
	void ChangeAnimUp();
	void ChangeAnimDown();


	State state;
	Look look;

	TileMap *map;

	int score;
};