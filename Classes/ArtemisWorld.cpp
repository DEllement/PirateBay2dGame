#include "ArtemisWorld.h"

// artemis

using namespace artemis;

// cocos2dx
USING_NS_CC;

// game
#include "SeaPath.h"
#include "EconomicAsset.h"
#include "Position.h"
#include "GridElement.h"
#include "LevelData.h"
#include "GridMatrix.h"

artemis::ArtemisWorld::ArtemisWorld()
{
	
	//Constants::ComponentType::PLAYER = ComponentTypeManager::getTypeFor<Player>();
	//Constants::ComponentType::TILE = ComponentTypeManager::getTypeFor<Tile>();
	//Constants::ComponentType::BOMB = ComponentTypeManager::getTypeFor<Bomb>();
	//Constants::ComponentType::MINA = ComponentTypeManager::getTypeFor<Mina>();
	//Constants::ComponentType::SHIP = ComponentTypeManager::getTypeFor<Ship>();
	//Constants::ComponentType::PIRATE_SHIP = ComponentTypeManager::getTypeFor<SoundFile>();
	//Constants::ComponentType::ISLAND = ComponentTypeManager::getTypeFor<SpatialForm>();
	//Constants::ComponentType::CITY = ComponentTypeManager::getTypeFor<Tower>();
	//Constants::ComponentType::SPRITE = ComponentTypeManager::getTypeFor<>();
	//Constants::ComponentType::VOLCANO = ComponentTypeManager::getTypeFor<TurnFactor>();
	//Constants::ComponentType::SPYGLASS = ComponentTypeManager::getTypeFor<Velocity>();
	//Constants::ComponentType::SEA_DIGGER = ComponentTypeManager::getTypeFor<Velocity>();
	//Constants::ComponentType::PLAYER = ComponentTypeManager::getTypeFor<Player>();
	Constants::ComponentType::ECONOMIC_ASSET = ComponentTypeManager::getTypeFor<EconomicAsset>();
	Constants::ComponentType::POSITION = ComponentTypeManager::getTypeFor<Position>();
	Constants::ComponentType::GRID_ELEMENT = ComponentTypeManager::getTypeFor<GridElement>();
	Constants::ComponentType::GRID_MATRIX = ComponentTypeManager::getTypeFor<GridMatrix>();
	Constants::ComponentType::LEVEL_DATA = ComponentTypeManager::getTypeFor<LevelData>();
	Constants::ComponentType::SEA_PATH = ComponentTypeManager::getTypeFor<SeaPath>();
}

artemis::ArtemisWorld::~ArtemisWorld()
{

}

bool artemis::ArtemisWorld::init()
{
	do
	{
		CC_BREAK_IF(!Component::init());
		this->setName("ArtemisWorld");
		_world.getSystemManager()->initializeAll();
		return true;
	} while (0);
	return false;
}

void artemis::ArtemisWorld::update(float delta)
{
	_world.loopStart();
	_world.setDelta(delta);
	
}

EntitySystem* artemis::ArtemisWorld::setSystem(EntitySystem* system)
{
	return _world.getSystemManager()->setSystem(system);
}

artemis::World& artemis::ArtemisWorld::getWorld()
{
	return _world;
}

void artemis::ArtemisWorld::removeAllEntities()
{
	_world.getEntityManager()->removeAllEntities();
}
