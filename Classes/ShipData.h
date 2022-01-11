#pragma once

#include "artemis/artemis.h"
#include <vector>

#include "GridElement.h"
#include "Constants.h"

class ShipData : public artemis::Component
{
public:
	ShipData();
	~ShipData();

	char id; //Note: this is note entity id, but a letter id such as A, B, C, D

	float health;
	float armor;

	bool isSelected;
	bool isBoatJustSelected;
	bool isJustDeselected;
	bool startFollowingPath;
	std::vector<GridElement*> currentPath;
	
	bool toBeRemoved;
	Constants::EnemyType destroyedBy;
};

