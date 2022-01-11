#pragma once
#include "Artemis\artemis.h"
#include "Position.h"
#include "Motion.h"
#include "TrigLUT.h"

class MovementSystem : public artemis::EntityProcessingSystem
{

public:
	MovementSystem();
	~MovementSystem() {}

	virtual void initialize() override;
	virtual void processEntity(artemis::Entity &e) override;
	virtual bool checkProcessing() override;
private:
	artemis::ComponentMapper<Position> _positionsMapper;
	artemis::ComponentMapper<Motion>  _motionsMapper;

};

