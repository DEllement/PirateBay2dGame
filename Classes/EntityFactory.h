#pragma once

#include "Artemis\Artemis.h"
#include "cocos2d.h"

// game component
#include "Position.h"
#include "Boat.h"
#include "EconomicAsset.h"
#include "LevelData.h"
#include "SeaPath.h"
#include "GridElement.h"
#include "GridMatrix.h"
#include "EnemyData.h"
#include "Kraken.h"
#include "ObjectiveData.h"
#include "PlayerData.h"
#include "ShipData.h"
#include "VortexData.h"
#include "Motion.h"
#include "DragIndicator.h"
#include "TargetMark.h"

using namespace artemis;

class EntityFactory
{
public:
	static Entity& createPlayer(World& world);

	static Entity& createLevel(World& world);

	static Entity& createShip(World& world, float x, float y);

	static Entity& createTile(World& world, int tileType);

	static Entity& createKraken(World& world, float startCol, float startRow);

	static Entity& createReachTheTargetsObjective(World& world, std::vector<Vec2> targets, int minTargetRequired, bool simultaneousTarget, float startTime, float timeLimit, char shipId='_');
	static Entity& createBoatsMustSurviveObjective(World& world, bool allBoatRequired, int minBoatAliveRequired);
	static Entity& createItemsUsageObjective(World& world, bool mustUseItem, bool cantUseItem, bool minItemUsageTimeRequired, int maxItemUsageLimit);

	static Entity& createTargetMark(World& world, int col, int row);

	static Entity& createVortex(World& world, 
		int colX, int rowY,
		int colspan, int rowspan, 
		bool haveLifeSpan, float lifeSpan, 
		bool startActive, 
		bool showAtInterval, float showAtIntervalDelay,
		bool showAtRandomPosition);

	static Entity& createDragIndicator(World& world);
};

