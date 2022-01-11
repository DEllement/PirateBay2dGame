#include "Motion.h"


Motion::Motion():
rotation(0),
directionX(0),
directionY(0),
isMoving(false),
velocity(1)
{
}


Motion::~Motion()
{
}

Motion* Motion::clone(){

	Motion* motion = new Motion();
	motion->rotation = this->rotation;
	motion->directionX = 0;
	motion->directionY = 0;
	motion->isMoving = this->isMoving;
	motion->velocity = this->velocity;

	return motion;
}
