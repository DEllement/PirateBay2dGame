#pragma once

#include "Artemis/Artemis.h"

#include "cocos2d.h"

class SeaPath : public artemis::Component
{
public:
	SeaPath();
	~SeaPath();

	int entityId;
	int linkedMovingSeaPathEntityId;

	enum Direction{
		NORTH = 0,
		EAST = 1,
		SOUTH = 2,
		WEST = 3,
		WEST_NORTH = 4,
		NORTH_EAST = 5,
		EAST_SOUTH = 6,
		SOUTH_WEST = 7,
		NORTH_SOUTH = 8,
		WEST_EAST = 9
	};
	int edgesMask[10];

	int tileType;
	float rotation;
	bool toBeRemoved;
	bool isAMovingSeaPath;
	bool isDirty;
	//Way to removed the tile, ex: Explose, Drown, Eat etc...

	bool haveEdgeAt(Direction direction);
	void setTileType(int tileType);
};