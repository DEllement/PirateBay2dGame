#pragma once

#include "artemis/Artemis.h"

#include "Constants.h"

class EnemyData : public artemis::Component
{
public:
	EnemyData();
	~EnemyData();

	bool isAlive;
	bool canMove;
	bool canEatTile;
	bool canDestroyShip;
	bool toBeRemoved;

	float lastSpannedDelta;
	
	Constants::EnemyType enemyType;
};

