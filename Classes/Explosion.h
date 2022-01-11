#pragma once

#include "Artemis/Artemis.h"

/*class enum ExplosionRange{
	ONE_CELL,
	THREE_CELLS_HORIZONTALY,
	THREE_CELLS_VERTICALY,
	FIVE_CELLS,
	NINE_CELLS,
};*/

class Explosion : public artemis::Component
{
public:
	Explosion();
	~Explosion();

	bool canHurt;
	//bool range;
};

