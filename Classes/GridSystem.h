#pragma once

#include "Artemis\Artemis.h"

#include "Constants.h"
#include "Position.h"
#include "GridElement.h"
#include "GridMatrix.h"
#include "SeaPath.h"
#include "Boat.h"
#include "EnemyData.h"
#include "ShipData.h"

class GridSystem : public artemis::EntityProcessingSystem
{
public:
	GridSystem();
	~GridSystem();

public:
	virtual void initialize() override;
	virtual void begin() override;
	virtual void processEntities(artemis::ImmutableBag<artemis::Entity*> &bag) override;
	
	virtual void end() override;

	virtual void removed(artemis::Entity &e) override;
	virtual void added(artemis::Entity &e) override;

	virtual bool checkProcessing() override;
protected:
	virtual void processEntity(artemis::Entity &e) override;

	artemis::ComponentMapper<Position> _positionsMapper;
	artemis::ComponentMapper<GridElement> _gridElementsMapper;
	artemis::ComponentMapper<SeaPath> _seaPathsMapper;
	artemis::ComponentMapper<EnemyData> _enemyDatasMapper;
	artemis::ComponentMapper<ShipData> _shipDatasMapper;
	
private:
	GridMatrix* _gridMatrix;
	void ensureGridMatrix();
};

