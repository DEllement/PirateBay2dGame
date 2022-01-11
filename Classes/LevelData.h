#pragma once

#include <string>
#include <vector>
#include "Artemis\Artemis.h"

#include "GridElement.h"
#include "ShipData.h"

class LevelData : public artemis::Component
{
public:
	LevelData();
	~LevelData();

	int levelNum;
	std::string name;

	//Level Settings
	int shipCount;
	int enemyCount;
	int islandCount;

	//Level Status
	bool justStarted;

	//Entity created at Interval (repeaet forever )
	float krakenCreationIntervalDelay;
	float movingSeaPathCreationIntervalDelay;
	float vortexCreationIntervalDelay;

	float time;
	float speed;

	std::vector<GridElement*> boatStartingPositions;
	std::vector<ShipData*> shipDatas;
};

