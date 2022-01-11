#pragma once
#include "artemis/Artemis.h"

class VortexData : public artemis::Component
{
public:
	VortexData();
	~VortexData();

	//Positioning / Size
	int cellX;
	int cellY;
	int colspan;
	int rowspan;
	bool isActive;

	//Lifespan
	bool haveLifeSpan;
	float lifeSpan;
	float lifeSpanDelta;

	//Interval Appearance
	bool showAtInterval;
	float showAtIntervalDelay;
	float showAtIntervalDelta;
	bool showAtRandomPosition;
	

};

