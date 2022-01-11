#include "EnemiesSystem.h"

#include "EntityFactory.h"
#include "Kraken.h"

EnemiesSystem::EnemiesSystem() :_levelData(nullptr), _krakenLastCreationDelta(0), _vortexLastCreationDelta(0)
{
	addComponentType<EnemyData>();
	addComponentType<Position>();
	addComponentType<GridElement>();
}


EnemiesSystem::~EnemiesSystem()
{
}

void EnemiesSystem::initialize()
{
	_positionsMapper.init(*world);
	_gridElementsMapper.init(*world);
	_enemyDatasMapper.init(*world);
	_vortexDatasMapper.init(*world);
}

void EnemiesSystem::added(artemis::Entity &e){
	
}
void EnemiesSystem::removed(artemis::Entity &e){

}

void EnemiesSystem::processEntities(artemis::ImmutableBag<artemis::Entity*> &bag){
	
	ensureImportantComponent();

	//Kraken Creation Logic
	if (_levelData->krakenCreationIntervalDelay > 0){
		_krakenLastCreationDelta += this->world->getDelta();
		if (_krakenLastCreationDelta >= _levelData->krakenCreationIntervalDelay){

			//Random edge positin
			int possibleRowsOrCols[] = { 2,5,8 };
			Position* randomGridPosition = _gridMatrix->getRandomEdgePosition(possibleRowsOrCols, 3);

			artemis::Entity &kraken = EntityFactory::createKraken(*world, randomGridPosition->getX(), randomGridPosition->getY());
			
			Position* position = _positionsMapper.get(kraken);
			position->setRotation(randomGridPosition->getRotation());

			Motion* motion = (Motion*)kraken.getComponent<Motion>();
			motion->rotation = position->getRotation();

			kraken.refresh();

			_krakenLastCreationDelta = 0;
		}
	}

	//Vortex Creation Logic
	if (_levelData->vortexCreationIntervalDelay > 0){
		_vortexLastCreationDelta += this->world->getDelta();
		if (_vortexLastCreationDelta >= _levelData->vortexCreationIntervalDelay){
			
			Position* randomGridPosition = _gridMatrix->getRandomPosition();
			//create vortex with lifeSpan = creation delta for now...
			artemis::Entity& vortex = EntityFactory::createVortex(*world, randomGridPosition->getX(), randomGridPosition->getY(), 1,1, 
																   true, _levelData->vortexCreationIntervalDelay, true, false, 0, true);

			Position* position = _positionsMapper.get(vortex);
			position->setLocation(randomGridPosition->getX(), randomGridPosition->getY());
			vortex.refresh();

			_vortexLastCreationDelta = 0;
		}
	}


	EntityProcessingSystem::processEntities(bag);

}

void EnemiesSystem::processEntity(artemis::Entity &e)
{
	auto gridElement = _gridElementsMapper.get(e);
	auto enemyData = _enemyDatasMapper.get(e);
	auto position = _positionsMapper.get(e);

	bool isOutOfGrid = _gridMatrix->isCellOutOfGrid(gridElement->col, gridElement->row);
	int nbRows = 1;
	int nbCols = 1;

	switch (enemyData->enemyType)
	{
	case Constants::EnemyType::KRAKEN:

		


		

		break;
	
	case Constants::EnemyType::MINE:

		break;
	case Constants::EnemyType::PIRATE_SHIP:

		break;
	case Constants::EnemyType::VORTEX:
		
		auto vortexData = _vortexDatasMapper.get(e);

		//Handle LifeSpan Logics
		if (vortexData->haveLifeSpan){
			vortexData->lifeSpanDelta += world->getDelta();
			if (vortexData->lifeSpanDelta >= vortexData->lifeSpan){
				enemyData->toBeRemoved = true; //Might not need that flag...
				gridElement->toBeRemoved = true;
			}
		}

		//Handle Inteval Display
		if (vortexData->showAtInterval){
			vortexData->showAtIntervalDelta += world->getDelta();
			if (vortexData->showAtIntervalDelta >= vortexData->showAtIntervalDelay){
				vortexData->showAtIntervalDelta = 0;
				vortexData->isActive = !vortexData->isActive; //To alternate
			}
		}

		//CanEatTile State
		enemyData->canEatTile = vortexData->isActive;

		nbCols = vortexData->colspan;
		nbRows = vortexData->rowspan;

		break;
	}

	/*-----------------	Shared Behaviours ------------------*/

	//Remove if out of grid
	if (isOutOfGrid){
		gridElement->toBeRemoved = true;
	}

	//Check Collision
	if (!isOutOfGrid && enemyData->canEatTile){
		for (int x = 0; x < nbCols; x++){
			for (int y = 0; y < nbRows; y++){

				//Check SeaPath collision
				auto seaPath = _gridMatrix->seaPathAt(gridElement->col+x, gridElement->row+y);
				if (seaPath != nullptr && !seaPath->toBeRemoved && !seaPath->isAMovingSeaPath){ //The Kraken eat the seapath!
					seaPath->toBeRemoved = true;
				}

				//Check Ship collision
				if (!isOutOfGrid && enemyData->canDestroyShip){
					auto shipData = _gridMatrix->shipDataAt(gridElement->col+x, gridElement->row+y);
					if (shipData != nullptr && !shipData->toBeRemoved){ //The Kraken eat the seapath!
						shipData->destroyedBy = enemyData->enemyType;
						shipData->toBeRemoved = true;
					}
				}
			}
		}
	}

	
}

bool EnemiesSystem::checkProcessing()
{
	return true;
}

void EnemiesSystem::ensureImportantComponent(){
	if (nullptr == _gridMatrix || nullptr == _levelData){
		auto levelEntity = &(world->getTagManager()->getEntity(Constants::Groups::LEVEL));
		if (levelEntity != nullptr){
			_gridMatrix = (GridMatrix*)levelEntity->getComponent(Constants::ComponentType::GRID_MATRIX);
			_levelData = (LevelData*)levelEntity->getComponent(Constants::ComponentType::LEVEL_DATA);
		}
	}
}