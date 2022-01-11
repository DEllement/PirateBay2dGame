#include "MovementSystem.h"


MovementSystem::MovementSystem()
{
	addComponentType<Position>();
	addComponentType<Motion>();
}

void MovementSystem::initialize()
{
	_positionsMapper.init(*world);
	_motionsMapper.init(*world);
}

void MovementSystem::processEntity(artemis::Entity &e)
{
	auto p = _positionsMapper.get(e);
	auto m = _motionsMapper.get(e);

	//m->rotation = p->getRotation();
	if (m->rotation == 0)
		m->directionY = -1;
	else if (m->rotation == 90)
		m->directionX = -1;
	else if (m->rotation == 180)
		m->directionY = 1;
	else if (m->rotation == 270)
		m->directionX = 1;

	if (m->isMoving){
		p->setLocation(
			p->getX() + m->directionX*m->velocity,
			p->getY() + m->directionY*m->velocity
			);
	}
}

bool MovementSystem::checkProcessing()
{
	return true;
}