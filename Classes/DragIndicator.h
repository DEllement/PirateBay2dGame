#pragma once

#include "artemis/Artemis.h"

class DragIndicator : public artemis::Component
{
public:
	DragIndicator();
	~DragIndicator();

	int currentColX;
	int currentRowY;

	bool isOutOfGrid;
	bool canDropHere;
};

