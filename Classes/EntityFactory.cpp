#include "EntityFactory.h"

#include "Constants.h"



using namespace artemis;
USING_NS_CC;

Entity& EntityFactory::createShip(World& world, float col, float row)
{
	Entity& e = world.createEntity();
	e.setTag(Constants::Tags::PLAYER_SHIP);
	e.setGroup(Constants::Groups::PLAYER_SHIP);

	GridElement* gridElement = new GridElement();
	gridElement->col = col;
	gridElement->row = row;
	gridElement->isOnGrid = true;
	gridElement->isSolid = false;

	//ShipData* shipData = new ShipData();

	e.addComponent(new Position());
	e.addComponent(gridElement);
	//e.addComponent(shipData);

	//e.refresh();
	return e;
}

Entity& EntityFactory::createPlayer(artemis::World& world){
	
	Entity& e = world.createEntity();
	e.setTag(Constants::Tags::PLAYER);

	auto playerData = new PlayerData();

	auto economicAsset = new EconomicAsset();
	economicAsset->lifes = 3;
	economicAsset->goldCoins = 0;
	economicAsset->jewels = 0;
	economicAsset->setGoldCoinsDirty(true);
	economicAsset->setJewelsDirty(true);
	economicAsset->setLifesDirty(true);
	
	e.addComponent(playerData);
	e.addComponent(economicAsset);
	
	world.getTagManager()->subscribe(Constants::Groups::PLAYER, e);
	e.refresh();
	return e;
}

Entity& EntityFactory::createLevel(artemis::World& world){

	Entity& e = world.createEntity();
	e.setTag(Constants::Tags::LEVEL);

	auto levelData = new LevelData();
	levelData->levelNum = 1;
	levelData->name = "Level 1 Title";
	levelData->krakenCreationIntervalDelay = 10;
	//levelData->movingSeaPathCreationIntervalDelay = 5;
	levelData->vortexCreationIntervalDelay = 5;
	levelData->shipCount = 2;

	//Test--------
	auto shipData1 = new ShipData();
	shipData1->id = 'A';
	shipData1->health = 4;
	auto shipStartPos1 = new GridElement();
	shipStartPos1->col = 0;
	shipStartPos1->row = 0;

	auto shipData2 = new ShipData();
	shipData2->id = 'B';
	shipData2->health = 4;
	auto shipStartPos2 = new GridElement();
	shipStartPos2->col = 0;
	shipStartPos2->row = 7;

	levelData->shipDatas.push_back(shipData1);
	levelData->shipDatas.push_back(shipData2);
	levelData->boatStartingPositions.push_back(shipStartPos1);
	levelData->boatStartingPositions.push_back(shipStartPos2);

	//----------

	auto gridMatrix = new GridMatrix(Constants::GameParams::nbCols, Constants::GameParams::nbRows);

	auto economicAssetIfPerfect = new EconomicAsset();
	economicAssetIfPerfect->lifes = 1;
	
	e.addComponent(levelData);
	e.addComponent(gridMatrix);
	e.addComponent(economicAssetIfPerfect);
	e.refresh();

	//Create Players Ship
	for (int i = 0; i < levelData->shipCount; i++){

		GridElement* boatStartPos = levelData->boatStartingPositions.at(i);
		ShipData* shipData = levelData->shipDatas.at(i);

		Entity& playerShip = createShip(world, boatStartPos->col, boatStartPos->row);
		playerShip.addComponent(shipData);
		playerShip.refresh();
		

		//SeaPath underneath
		artemis::Entity& e1 = EntityFactory::createTile(world, 2);
		Position* tilePos = (Position*)e1.getComponent<Position>();
		GridElement* tileGE = new GridElement();
		tileGE->col = boatStartPos->col;
		tileGE->row = boatStartPos->row;
		tileGE->isOnGrid = false;
		tileGE->isSolid = true;
		//tileGE->justDropped = true;
		e1.addComponent(tileGE);
		e1.refresh();

	}

	//Create Level Objectives
	/*createObjective(world, 0, 0, 3, 0, 200, 100, 2, 0, ObjectiveData::ObjectiveType::PRIMARY);
	createObjective(world, 0, 0, 4, 0, 200, 100, 2, 0, ObjectiveData::ObjectiveType::PRIMARY);
	createObjective(world, 0, 0, 5, 0, 200, 100, 2, 0, ObjectiveData::ObjectiveType::PRIMARY);
	createObjective(world, 0, 0, 6, 0, 200, 100, 2, 0, ObjectiveData::ObjectiveType::PRIMARY);
	createObjective(world, 0, 0, 7, 0, 200, 100, 2, 0, ObjectiveData::ObjectiveType::PRIMARY);*/
	/*createObjective(world, 0, 0, 6, 3, 30, 100, 2, 0, ObjectiveData::ObjectiveType::SECONDARY,30);
	createObjective(world, 0, 9, 3, 5, 60, 50, 1, 0, ObjectiveData::ObjectiveType::SECONDARY,45);
	createObjective(world, 0, 0, 6, 7, 30, 100, 2, 0, ObjectiveData::ObjectiveType::SECONDARY,60);
	createObjective(world, 0, 5, 9, 9, 120, 25, 0, 0, ObjectiveData::ObjectiveType::SECONDARY,75);*/

	std::vector<Vec2> targets;
	targets.push_back(Vec2(5, 0));
	targets.push_back(Vec2(2, 3));
	targets.push_back(Vec2(3, 5));
	std::vector<Vec2> targets2;
	targets2.push_back(Vec2(7, 5));
	targets2.push_back(Vec2(7, 7));
	targets2.push_back(Vec2(7, 8));
	targets2.push_back(Vec2(5, 5));

	Entity& objective1 = createReachTheTargetsObjective(world, targets, 3, false, 0, 200, 'A');
	Entity& objective2 = createReachTheTargetsObjective(world, targets2, 2, false, 0, 200, 'B');
	
	

	//Create Permanent Game Entities
	createVortex(world, 3, 3, 2, 2, false, 0, true, true, 5, false);

	return e;
}

Entity& EntityFactory::createTile(artemis::World& world, int tileType){

	Entity& e = world.createEntity();
	world.getGroupManager()->set(Constants::Groups::TILES, e);

	auto tile = new SeaPath();
	tile->setTileType(tileType);
	tile->entityId = e.getId();

	auto gridElement = new GridElement();
	gridElement->isSolid = true;
	
	auto position = new Position();

	e.addComponent(tile);
	e.addComponent(gridElement);
	e.addComponent(position);
	e.refresh();
	return e;
}

Entity& EntityFactory::createKraken(artemis::World& world, float startCol, float startRow){

	Entity& e = world.createEntity();
	world.getGroupManager()->set(Constants::Groups::ENNEMIES, e);

	auto gridElement = new GridElement();
	gridElement->isSolid = true;
	gridElement->col = startCol;
	gridElement->row = startRow;

	auto position = new Position();
	position->setX(0);
	position->setY(0);

	auto economicAsset = new EconomicAsset();
	economicAsset->lifes = 0;
	economicAsset->goldCoins = 50;
	economicAsset->jewels = 0;

	auto enemyData = new EnemyData();
	enemyData->enemyType = Constants::EnemyType::KRAKEN;
	enemyData->canEatTile = true;
	enemyData->canDestroyShip = true;

	auto motion = new Motion();
	motion->velocity = .5;
	motion->isMoving = true;

	e.addComponent(enemyData);
	e.addComponent(gridElement);
	e.addComponent(position);
	e.addComponent(economicAsset);
	e.addComponent(motion);
	e.refresh();
	return e;
}
Entity& EntityFactory::createVortex(artemis::World& world, int colX, int rowY, int colspan, int rowspan, bool haveLifeSpan, float lifeSpan, bool startActive, bool showAtInterval, float showAtIntervalDelay, bool showAtRandomPosition){

	Entity& e = world.createEntity();
	e.setGroup(Constants::Groups::ENNEMIES);

	auto enemyData = new EnemyData();
	enemyData->canDestroyShip = true;
	enemyData->canEatTile = true;
	enemyData->canMove = false;
	enemyData->isAlive = true;
	enemyData->enemyType = Constants::EnemyType::VORTEX;
	
	auto vortexData = new VortexData();
	vortexData->cellX = colX;
	vortexData->cellY = rowY;
	vortexData->colspan = colspan;
	vortexData->rowspan = rowspan;
	vortexData->haveLifeSpan = haveLifeSpan;
	vortexData->isActive = startActive;
	vortexData->lifeSpan = haveLifeSpan ? lifeSpan : 0;
	vortexData->showAtInterval = showAtInterval;
	vortexData->showAtIntervalDelay = showAtInterval ? showAtIntervalDelay : 0;
	vortexData->showAtRandomPosition = showAtRandomPosition;

	auto gridElement = new GridElement();
	gridElement->isOnGrid = true;
	gridElement->isSolid = false;
	gridElement->col = colX;
	gridElement->row = rowY;

	auto position = new Position();

	e.addComponent(position);
	e.addComponent(gridElement);
	e.addComponent(enemyData);
	e.addComponent(vortexData);
	e.refresh();
	return e;
}
Entity& EntityFactory::createReachTheTargetsObjective(artemis::World& world, std::vector<Vec2> targets, int minTargetRequired, bool simultaneousTarget, float startTime, float timeLimit, char shipId){

	Entity& e = world.createEntity();
	e.setGroup(Constants::Groups::OBJECTIVES);
	
	auto objectiveData = new ObjectiveData();
	objectiveData->objectiveType = ObjectiveData::ObjectiveType::REACH_THE_MARK;
	objectiveData->simultaneousTarget = simultaneousTarget;
	objectiveData->minTargetRequired = minTargetRequired;
	objectiveData->targets = targets;
	objectiveData->totalTargets = targets.size();
	objectiveData->shipId = shipId;

	objectiveData->limitTime = timeLimit;
	objectiveData->startTime = startTime;

	auto economicAsset = new EconomicAsset();
	economicAsset->goldCoins = 150;
	economicAsset->jewels = 1;

	auto gridElement = new GridElement();
	gridElement->isOnGrid = true;
	gridElement->isSolid = false;
	gridElement->col = -1;
	gridElement->row = -1;

	auto position = new Position();
	position->setX(0);
	position->setY(0);

	e.addComponent(objectiveData);
	e.addComponent(economicAsset);
	e.addComponent(gridElement);
	e.addComponent(position);

	e.refresh();
	return e;
}

Entity& EntityFactory::createBoatsMustSurviveObjective(artemis::World& world, bool allBoatRequired, int minBoatAliveRequired){

	Entity& e = world.createEntity();
	e.setGroup(Constants::Groups::OBJECTIVES);

	auto objectiveData = new ObjectiveData();
	objectiveData->objectiveType = ObjectiveData::ObjectiveType::BOATS_MUST_SURVIVE;
	objectiveData->allBoatRequired = allBoatRequired;
	objectiveData->minBoatAlive = minBoatAliveRequired;
	
	auto economicAsset = new EconomicAsset();

	e.addComponent(objectiveData);
	e.addComponent(economicAsset);

	e.refresh();
	return e;
}

Entity& EntityFactory::createItemsUsageObjective(World& world, bool mustUseItem, bool cantUseItem, bool minItemUsageTimeRequired, int maxItemUsageLimit){

	Entity& e = world.createEntity();
	e.setGroup(Constants::Groups::OBJECTIVES);

	auto objectiveData = new ObjectiveData();
	objectiveData->objectiveType = ObjectiveData::ObjectiveType::BOATS_MUST_SURVIVE;
	objectiveData->mustUseItem = mustUseItem;
	objectiveData->cantUseItem = cantUseItem;
	objectiveData->minItemUsageTimeRequired = minItemUsageTimeRequired;

	auto economicAsset = new EconomicAsset();

	e.addComponent(objectiveData);
	e.addComponent(economicAsset);

	e.refresh();
	return e;

}

Entity& EntityFactory::createDragIndicator(World& world){
	Entity& e = world.createEntity();
	DragIndicator* dragIndicator = new DragIndicator();
	Position* position = new Position();
	position->setLocation(-999, -999);
	GridElement* gridElement = new GridElement();
	e.addComponent(position);
	e.addComponent(dragIndicator);
	e.addComponent(gridElement);
	e.refresh();
	return e;
}

Entity& EntityFactory::createTargetMark(World& world, int col, int row){
	
	Entity& e = world.createEntity();

	TargetMark* targetmark = new TargetMark();
	Position* position = new Position();
	GridElement* gridElement = new GridElement();
	gridElement->col = col;
	gridElement->row = row;
	gridElement->isSolid = false;
	gridElement->isOnGrid = true;
	gridElement->justDropped = true;

	e.addComponent(position);
	e.addComponent(gridElement);
	e.addComponent(targetmark);

	e.refresh();
	return e;
}
