#pragma once
#include "Artemis\Artemis.h"
class Motion :
	public artemis::Component
{
public:
	Motion();
	~Motion();

	float rotation;
	float directionX;
	float directionY;
	bool isMoving;
	float velocity;

	Motion* clone();
};

