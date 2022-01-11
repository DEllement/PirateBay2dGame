#include "LevelData.h"


LevelData::LevelData()
{
	shipCount = 0;
	boatStartingPositions = std::vector<GridElement*>();
	shipDatas = std::vector<ShipData*>();
}


LevelData::~LevelData()
{
}
