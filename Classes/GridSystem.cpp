#include "GridSystem.h"

#include "DragIndicator.h"

GridSystem::GridSystem() :_gridMatrix(nullptr)
{
	addComponentType<GridElement>();
	addComponentType<Position>();
}


GridSystem::~GridSystem()
{
	
}

void GridSystem::initialize(){

	_seaPathsMapper.init(*world);
	_gridElementsMapper.init(*world);
	_positionsMapper.init(*world);
	_enemyDatasMapper.init(*world);
	_shipDatasMapper.init(*world);
}

void GridSystem::begin(){
	ensureGridMatrix();

	//nullify all seaPath
	if (_gridMatrix)
		_gridMatrix->nullAllSeaPaths();

}
void GridSystem::end(){}
void GridSystem::removed(artemis::Entity &e){

	auto gridElement = _gridElementsMapper.get(e);
	auto seaPath = _seaPathsMapper.get(e);

	if (seaPath != nullptr){
		_gridMatrix->setSeaPath(nullptr, gridElement->col, gridElement->row);
	}
}
void GridSystem::added(artemis::Entity &e){


}
void GridSystem::processEntities(artemis::ImmutableBag<artemis::Entity*> &bag){

	for (int i = 0; i < bag.getCount(); ++i)
		processEntity(*bag.get(i));
	
}
void GridSystem::processEntity(artemis::Entity &e){

	//
	auto position = _positionsMapper.get(e);
	auto gridElement = _gridElementsMapper.get(e);
	auto seaPath = _seaPathsMapper.get(e);
	auto shipData = _shipDatasMapper.get(e);
	auto dragIndicator = (DragIndicator*)e.getComponent<DragIndicator>();

	gridElement->col = std::floor(position->getX() / Constants::GameParams::gridElementWidth);
	gridElement->row = std::floor(position->getY() / Constants::GameParams::gridElementHeight);

	if (seaPath != nullptr){
		
		if (seaPath->toBeRemoved){
			e.remove();
			return;
		}
	}
	if (gridElement->toBeRemoved){
		e.remove();
		return;
	}

	if (_gridMatrix != nullptr){
		if (seaPath != nullptr){
			seaPath->rotation = position->getRotation();
			if ((gridElement->isSolid || seaPath->isAMovingSeaPath) && !_gridMatrix->isCellOutOfGrid(gridElement->col, gridElement->row))
				_gridMatrix->setSeaPath(seaPath, gridElement->col, gridElement->row);
			
		}
		if (shipData != nullptr){
			_gridMatrix->setShipData(shipData, gridElement->col, gridElement->row);
		}	
	}

	if (dragIndicator != nullptr){
		dragIndicator->isOutOfGrid = _gridMatrix->isCellOutOfGrid(gridElement->col, gridElement->row);
		if (!dragIndicator->isOutOfGrid)
			dragIndicator->canDropHere = _gridMatrix->seaPathAt(gridElement->col, gridElement->row) == nullptr; //Do real check up
		else
			dragIndicator->canDropHere = false;
	}

}
bool GridSystem::checkProcessing(){
	return true;
}

void GridSystem::ensureGridMatrix(){
	if (nullptr == _gridMatrix){
		auto levelEntity = &(world->getTagManager()->getEntity(Constants::Groups::LEVEL));
		if (levelEntity != nullptr){
			_gridMatrix = (GridMatrix*)levelEntity->getComponent(Constants::ComponentType::GRID_MATRIX);
		}
	}
}
