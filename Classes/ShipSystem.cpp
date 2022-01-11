#include "ShipSystem.h"

#include "Position.h"
#include "GridMatrix.h"
#include "SeaPath.h"

#include "Constants.h"
#include "GridEvent.h"

ShipSystem::ShipSystem() :_gridMatrix(nullptr), _selectedShip(nullptr)
{
	addComponentType<ShipData>();
	addComponentType<Position>();
	addComponentType<GridMatrix>();
	addComponentType<GridElement>();
}

ShipSystem::~ShipSystem()
{
}

void ShipSystem::initialize(){
	
	_shipDatasMapper.init(*world);
	_gridElementsMapper.init(*world);
	_positionsMapper.init(*world);
	//Events Listeners
	Director::getInstance()->getEventDispatcher()->addCustomEventListener(Constants::GameEvent::GRID_CLICK, CC_CALLBACK_1(ShipSystem::onGridClick, this));

	
}
void ShipSystem::begin() {
	//ensureGridMatrix();
}
void ShipSystem::processEntities(artemis::ImmutableBag<artemis::Entity*> &bag) {

	

}
void ShipSystem::processEntity(artemis::Entity &e){

	auto shipData = _shipDatasMapper.get(e);
	if (shipData->toBeRemoved){
		e.remove();
	}

}
void ShipSystem::end(){

}

void ShipSystem::removed(artemis::Entity &e) {

}
void ShipSystem::added(artemis::Entity &e){

	if (_shipDatasMapper.get(e) != nullptr){
		


	}
}

bool ShipSystem::checkProcessing(){
	return true;
}


void ShipSystem::onGridClick(EventCustom* e){

	ensureGridMatrix();
	
	GridEvent* gridEvent = (GridEvent*)(e->getUserData());
	if (!gridEvent->isInGridBoundary){
		return;
	}

	//Check if user clicked on a SeaPath
	auto seaPathBags = (world->getGroupManager()->getEntities(Constants::Groups::TILES));
	for (int i = 0; i < seaPathBags->getCount(); i++){
		auto seaPathEntity = seaPathBags->get(i);
		auto seaPathGridElement = _gridElementsMapper.get(*seaPathEntity);
		if (seaPathGridElement->col == gridEvent->col && seaPathGridElement->row == gridEvent->row){
			this->onSeaPathClicked(*seaPathEntity);
		}
	}

	//Check if user clicked on a boat

	artemis::ImmutableBag<artemis::Entity*>* _shipsEntities = world->getGroupManager()->getEntities(Constants::Groups::PLAYER_SHIP);
	isBoatSelected = false;
	for (int i = 0; i < _shipsEntities->getCount(); i++){

		auto playerShip = _shipsEntities->get(i);
		auto shipGridElement = _gridElementsMapper.get(*playerShip);
		if (shipGridElement->col == gridEvent->col && shipGridElement->row == gridEvent->row){
			this->onShipClicked(*playerShip);
		}
		else{
			ShipData* shipData = (ShipData*)playerShip->getComponent<ShipData>();
			if (shipData->isSelected)
				shipData->isJustDeselected = true;
			shipData->isSelected = false;
			//Director::getInstance()->getEventDispatcher()->dispatchCustomEvent(Constants::GameEvent::DEBUG_LOG, "onGridClick isBoatSelected = false");
			//CCLOG("onGridClick isBoatSelected = false");
		}
	}
}

void ShipSystem::onShipClicked(artemis::Entity& e){
	
	_selectedShip = &e;

	ShipData* shipData = (ShipData*)_selectedShip->getComponent<ShipData>();
	shipData->isBoatJustSelected = true;
	shipData->isSelected = true;

	isBoatSelected = true;
	Director::getInstance()->getEventDispatcher()->dispatchCustomEvent(Constants::GameEvent::DEBUG_LOG, (char*)"onShipClicked");

}
void ShipSystem::onSeaPathClicked(artemis::Entity& e){

	Director::getInstance()->getEventDispatcher()->dispatchCustomEvent(Constants::GameEvent::DEBUG_LOG, (char*)"onSeaPathClicked Begin...");
	SeaPath* seaPath = (SeaPath*)e.getComponent<SeaPath>();

	if (isBoatSelected && _selectedShip != nullptr)
	{
		GridElement* seaPath_gridElement = _gridElementsMapper.get(e);
		GridElement* ship_gridElement	 = _gridElementsMapper.get(*_selectedShip);
		std::vector<GridElement*> path	 = _gridMatrix->getPathToDestination(ship_gridElement->col, ship_gridElement->row, seaPath_gridElement->col, seaPath_gridElement->row);
		
		if (path.size() == 1 && path[0]->col == seaPath_gridElement->col && path[0]->row == seaPath_gridElement->row){
			return;
		}
		//Director::getInstance()->getEventDispatcher()->dispatchCustomEvent(Constants::GameEvent::DEBUG_LOG, (char*)"Path Computed");
		ShipData* shipData = (ShipData*)_selectedShip->getComponent<ShipData>();
		shipData->currentPath = path;
		shipData->startFollowingPath = true;
	
		//Director::getInstance()->getEventDispatcher()->dispatchCustomEvent(Constants::GameEvent::DEBUG_LOG, (char*)"onSeaPathClicked startFollowingPath flagged");
	}
	//isBoatSelected = false;
}

void ShipSystem::ensureGridMatrix(){
	if (nullptr == _gridMatrix){
		auto levelEntity = &(world->getTagManager()->getEntity(Constants::Groups::LEVEL));
		if ( levelEntity != nullptr ){
			_gridMatrix = (GridMatrix*)levelEntity->getComponent(Constants::ComponentType::GRID_MATRIX);
		}
	}
}
