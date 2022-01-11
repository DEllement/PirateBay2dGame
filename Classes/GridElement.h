#pragma once

#include "Artemis/artemis.h"

class GridElement : public artemis::Component
{
public:
	GridElement();
	~GridElement();

	int col;
	int row;

	bool isOnGrid;
	bool justDropped;
	bool isSolid;
	bool toBeRemoved;
};

