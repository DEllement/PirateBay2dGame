#pragma once

#include "artemis/Artemis.h"
#include "cocos2d.h"

#include "EntityFactory.h"
#include "Constants.h"
#include "LevelData.h"
#include "GridMatrix.h"
#include "Motion.h"
#include "Position.h"
#include "GridElement.h"

class RandomMovingSeaPathSystem : public artemis::EntityProcessingSystem
{
public:
	RandomMovingSeaPathSystem();
	~RandomMovingSeaPathSystem();

	virtual void initialize() override;
	virtual void added(artemis::Entity& entity) override;
	virtual void removed(artemis::Entity& entity) override;
	virtual void processEntities(artemis::ImmutableBag<artemis::Entity*> &bag);
	virtual void processEntity(artemis::Entity& entity) override;

	float lastCreationDelta;

private:
	LevelData* _levelData;
	GridMatrix* _gridMatrix;

	artemis::ComponentMapper<Position> _positionsMapper;
	artemis::ComponentMapper<GridElement> _gridElementsMapper;
	artemis::ComponentMapper<SeaPath> _seaPathsMapper;

	void ensureImportantComponent();
};

