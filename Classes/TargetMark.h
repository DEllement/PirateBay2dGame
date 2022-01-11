#pragma once

#include "Artemis\Artemis.h"

class TargetMark : public artemis::Component
{
public:
	TargetMark();
	~TargetMark();

	bool associatedShipJustSelected = false;
	bool associatedShipJustDeSelected = false;
};

