#pragma once
#include "artemis/Artemis.h"

#include "Position.h"
#include "GridElement.h"
#include "EnemyData.h"
#include "EconomicAsset.h"
#include "LevelData.h"
#include "GridMatrix.h"
#include "VortexData.h"
#include "Motion.h"

class EnemiesSystem : public artemis::EntityProcessingSystem
{
public:
	EnemiesSystem();
	~EnemiesSystem();

	virtual void initialize() override;
	virtual void processEntity(artemis::Entity &e) override;
	virtual void removed(artemis::Entity &e) override;
	virtual void added(artemis::Entity &e) override;
	virtual bool checkProcessing() override;
	virtual void processEntities(artemis::ImmutableBag<artemis::Entity*> &bag);
private:
	float _krakenLastCreationDelta;
	float _vortexLastCreationDelta;

	LevelData* _levelData;
	GridMatrix* _gridMatrix;

	artemis::ComponentMapper<Position> _positionsMapper;
	artemis::ComponentMapper<GridElement> _gridElementsMapper;
	artemis::ComponentMapper<EnemyData> _enemyDatasMapper;
	artemis::ComponentMapper<VortexData> _vortexDatasMapper;

	void ensureImportantComponent();
};

