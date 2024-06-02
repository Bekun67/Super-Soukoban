#pragma once
#include "Entity.h"
#include "TileMap.h"

//Representation model size: 32x32
#define PLAYER_FRAME_SIZE		32

#define PLAYER_PHYSICAL_WIDTH	32
#define PLAYER_PHYSICAL_HEIGHT	32

//Speed
#define PLAYER_SPEED			2

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

class Player : public Entity
{
public:
	Player(const Point& p, State s, Look view);
	~Player();

	AppStatus Initialise();
	void SetTileMap(TileMap* tilemap);
	void SetAuxTileMap(TileMap* tilemap);

	void InitScore();
	void IncrScore(int n);
	int GetScore();

	void Update();
	void DrawDebug(const Color& col) const;
	void Release();

	int steps = 0;
	int level = 0;
	bool lost = false;
	bool won = false;
	int wincount = 0;

	bool PushingUp = false;
	bool PushingDown = false;
	bool PushingRight = false;
	bool PushingLeft = false;

	int x, x1, x2, y, y1, y2;

	Tile nextTile, Destination;

private:
	bool IsLookingRight() const;
	bool IsLookingLeft() const;
	bool IsLookingUp() const;
	bool IsLookingDown() const;

	bool Moving = false;

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

	int maxMove;

	void StartPushingLeft();
	void StartPushingRight();
	void StartPushingUp();
	void StartPushingDown();

	void ChangeAnimRight();
	void ChangeAnimLeft();
	void ChangeAnimUp();
	void ChangeAnimDown();

	void WaitForInput();
	bool CanMove;

	void CheckSteps();

	//float timer;
	//float lifetime;

	//void StarTimer(float *t, float *l);
	//void UpdateTimer(float *t);
	//bool TimerDone(float *t);

	State state;
	Look look;

	TileMap* map;
	TileMap* mapAux;

	int score;
};