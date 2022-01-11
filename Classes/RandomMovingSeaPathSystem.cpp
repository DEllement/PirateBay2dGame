#include "RandomMovingSeaPathSystem.h"

#include "Constants.h"


RandomMovingSeaPathSystem::RandomMovingSeaPathSystem()
:lastCreationDelta(0), _gridMatrix(nullptr), _levelData(nullptr)
{
	addComponentType<Motion>();
	addComponentType<SeaPath>();
}


RandomMovingSeaPathSystem::~RandomMovingSeaPathSystem()
{
}

void RandomMovingSeaPathSystem::initialize()
{
	_positionsMapper.init(*world);
	_gridElementsMapper.init(*world);
	_seaPathsMapper.init(*world);

}

void RandomMovingSeaPathSystem::added(artemis::Entity& entity){

}
void RandomMovingSeaPathSystem::removed(artemis::Entity& entity){

}

void RandomMovingSeaPathSystem::processEntities(artemis::ImmutableBag<artemis::Entity*> &bag){
	
	EntityProcessingSystem::processEntities(bag);

	ensureImportantComponent();

	//Moving Sea Path 
	if (_levelData->movingSeaPathCreationIntervalDelay > 0){
		lastCreationDelta += world->getDelta();
		if (lastCreationDelta >= _levelData->movingSeaPathCreationIntervalDelay){
			lastCreationDelta = 0;

			//Random startUp position
			int possibleRowsOrCols[] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };
			Position* randomGridPosition = nullptr;
			int totalTry = 0;
			do{
				randomGridPosition = _gridMatrix->getRandomEdgePosition(possibleRowsOrCols, 10);
				if (totalTry > 100)
					return;
				totalTry++;
			} while (!_gridMatrix->isCellAvailable(randomGridPosition->getX(), randomGridPosition->getY()));

			//Create a new moving seapath
			artemis::Entity& movSeaPath = EntityFactory::createTile(*world, rand() % 8 + 1);
			Position* position = (Position*)movSeaPath.getComponent<Position>();
			position->setRotation(randomGridPosition->getRotation());
			SeaPath* seaPath = (SeaPath*)movSeaPath.getComponent<SeaPath>();
			seaPath->rotation = position->getRotation();
			seaPath->isAMovingSeaPath = true;

			GridElement* gridElement = (GridElement*)movSeaPath.getComponent<GridElement>();
			gridElement->col = randomGridPosition->getX();
			gridElement->row = randomGridPosition->getY();
			gridElement->isSolid = false;

			Motion* motion = new Motion();
			motion->velocity = .5;
			motion->isMoving = true;
			motion->rotation = position->getRotation();

			movSeaPath.addComponent(motion);
			movSeaPath.refresh();
		}
	}
}

void RandomMovingSeaPathSystem::processEntity(artemis::Entity& e){
	
	auto gridElement = _gridElementsMapper.get(e);
	auto seapath = _seaPathsMapper.get(e);
	auto position = _positionsMapper.get(e);

	bool isOutOfGrid = _gridMatrix->isCellOutOfGrid(gridElement->col, gridElement->row);
	//Remove if out of grid
	if (isOutOfGrid){
		gridElement->toBeRemoved = true;
	}
	else{
		auto motion = (Motion*)e.getComponent<Motion>();
		SeaPath* nextSeaPath = nullptr;
		if (!_gridMatrix->isCellOutOfGrid(gridElement->col + motion->directionX, gridElement->row + motion->directionY))
			nextSeaPath = _gridMatrix->seaPathAt(gridElement->col + motion->directionX, gridElement->row + motion->directionY);
	
		if (seapath->isAMovingSeaPath){
			//Stop if meet one of the conditions
			if ( (nextSeaPath != nullptr && !nextSeaPath->isAMovingSeaPath) ||							//Next to a fix path
				(motion->directionX == 1 && gridElement->col == Constants::GameParams::nbCols-1) ||		//At the edge
				(motion->directionY == 1 && gridElement->row == Constants::GameParams::nbRows-1) ||		//At the edge
				(motion->directionX == -1 && gridElement->col == 0) ||									//At the edge
				(motion->directionY == -1 && gridElement->row == 0)	){									//At the edge

				e.removeComponent<Motion>();
				gridElement->isSolid = true;
				gridElement->justDropped = true;
				seapath->isAMovingSeaPath = false;
				e.refresh();
			}
		}
	}
}

void RandomMovingSeaPathSystem::ensureImportantComponent(){
	if (nullptr == _gridMatrix || nullptr == _levelData){
		auto levelEntity = &(world->getTagManager()->getEntity(Constants::Groups::LEVEL));
		if ( levelEntity != nullptr){
			 _gridMatrix = (GridMatrix*)levelEntity->getComponent(Constants::ComponentType::GRID_MATRIX);
			 _levelData = (LevelData*)levelEntity->getComponent(Constants::ComponentType::LEVEL_DATA);
		}
	}
}