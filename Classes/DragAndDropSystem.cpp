#include "DragAndDropSystem.h"

#include "Constants.h"

#include "Artemis\Entity.h"

#include "GridEvent.h"

#include "TileRope.h"
#include "Position.h"
#include "GridElement.h"
#include "EntityFactory.h"
#include "DragIndicator.h"

DragAndDropSystem::DragAndDropSystem() : draggedTile(nullptr), isDragging(false), currentMouseX(0), currentMouseY(0), isMouseDown(false), _dragLayer(nullptr), _dragIndicator(nullptr)
{
	_dragLayer = Layer::create();
	_dragLayer->retain();
}


DragAndDropSystem::~DragAndDropSystem()
{
}

/* virtual override Implementation */
void DragAndDropSystem::initialize(){

	auto globalEventDispatcher = cocos2d::Director::getInstance()->getEventDispatcher();
	globalEventDispatcher->addCustomEventListener(Constants::GameEvent::SP_DISTRIBUTER_ITEM_CLICK, CC_CALLBACK_1(DragAndDropSystem::onSeaPathsDistributerItemClick, this));
	globalEventDispatcher->addCustomEventListener(Constants::GameEvent::SP_DISTRIBUTER_ITEM_MOUSEDOWN, CC_CALLBACK_1(DragAndDropSystem::onSeaPathsDistributerItemMouseDown,this));
	globalEventDispatcher->addCustomEventListener(Constants::GameEvent::GRID_MOUSEUP, CC_CALLBACK_1(DragAndDropSystem::onGridMouseUp, this));


	auto dragLayerMouseEventListener = EventListenerMouse::create();
	dragLayerMouseEventListener->onMouseMove = CC_CALLBACK_1(DragAndDropSystem::onDragLayerMouseMove, this);
	_dragLayer->getEventDispatcher()->addEventListenerWithSceneGraphPriority(dragLayerMouseEventListener, _dragLayer);

#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
	auto dragLayerTouchEventListener = EventListenerTouchOneByOne::create();
	dragLayerTouchEventListener->onTouchBegan = CC_CALLBACK_2(DragAndDropSystem::onDragLayerTouchBegan, this);
	dragLayerTouchEventListener->onTouchMoved = CC_CALLBACK_2(DragAndDropSystem::onDragLayerTouchMove, this);
	_dragLayer->getEventDispatcher()->addEventListenerWithSceneGraphPriority(dragLayerTouchEventListener, _dragLayer); //_dragLayer
#endif

}

void DragAndDropSystem::begin(){
	
}
void DragAndDropSystem::end(){}
void DragAndDropSystem::removed(artemis::Entity &e){}
void DragAndDropSystem::added(artemis::Entity &e){}
void DragAndDropSystem::processEntities(artemis::ImmutableBag<artemis::Entity*> &bag){
	
}
void DragAndDropSystem::processEntity(artemis::Entity &e){}
bool DragAndDropSystem::checkProcessing(){
	return true;
}

//Callbacks
void DragAndDropSystem::onSeaPathsDistributerItemMouseDown(EventCustom* customEvent){

	if (isMouseDown)
		return;

	distributerSelectedTile = (TileRope*)(customEvent->getUserData());
	draggedTile = TileRope::createFromType(distributerSelectedTile->tileType);
	draggedTile->setRotation(distributerSelectedTile->getRotation());
	draggedTile->setScale(0.88);
	draggedTile->retain();

	mouseDownAtX = currentMouseX;
	mouseDownAtY = currentMouseY;
	isMouseDown = true;
}

void DragAndDropSystem::onSeaPathsDistributerItemClick(EventCustom* customEvent){

	if (isDragging){
		this->cancelDrag();
		return;
	}

	TileRope* selectedTile = (TileRope*)(customEvent->getUserData());

	if (isMouseDown && !isDragging && selectedTile->getActionManager()->getNumberOfRunningActionsInTarget(selectedTile) == 0){
		
		selectedTile->runAction(RotateBy::create(0.5f,90));
		selectedTile->runAction(Sequence::createWithTwoActions(
											ScaleTo::create(0.25f, 1.5),
											ScaleTo::create(0.25f, 0.7))); //Constants::GameParams::gridElementWidth/releaseSize...
	}
	isMouseDown = false;
	
}

void DragAndDropSystem::onGridMouseUp(EventCustom* customEvent){
	
	ensureImportantComponent();

	GridEvent* gridEvent = (GridEvent*)(customEvent->getUserData());
	//is Inside the grid?
	//is cell available?

	if (isDragging && isMouseDown)
	{
		isDragging = false;

		if (gridEvent->isCellAvailable && gridEvent->canDropSeaPath && gridEvent->isInGridBoundary)
		{
			artemis::Entity& newTileEntity = EntityFactory::createTile(*world, draggedTile->tileType);
			auto position = (Position*)newTileEntity.getComponent<Position>();
			position->setLocation(gridEvent->x, gridEvent->y);
			position->setRotation(draggedTile->getRotation());

			auto gridElement = (GridElement*)newTileEntity.getComponent<GridElement>();
			gridElement->col = gridEvent->col;
			gridElement->row = gridEvent->row;
			gridElement->justDropped = true;
			gridElement->isOnGrid = false;

			/*if (!gridEvent->haveFixedSeaPathAround && gridEvent->haveMovingSeaPathAround){
			
				auto firstMovingSeaPathAround = _gridMatrix->getFirstMovingSeaPathAround(gridElement->col, gridElement->row);
				
				artemis::Entity& movingSeaPathEntity = world->getEntity(firstMovingSeaPathAround->entityId);
				Motion* motion = ((Motion*)movingSeaPathEntity.getComponent<Motion>())->clone();
				//motion->rotation = position->getRotation();
				newTileEntity.addComponent(motion);

				auto seaPathComp = (SeaPath*)newTileEntity.getComponent<SeaPath>();
				seaPathComp->isAMovingSeaPath = true;
				seaPathComp->rotation = position->getRotation();
				seaPathComp->linkedMovingSeaPathEntityId = firstMovingSeaPathAround->entityId;
				gridElement->justDropped = true;

			}*/

			
			
			newTileEntity.refresh();

			draggedTile->removeFromParent();
			draggedTile = nullptr;

			Director::getInstance()->getEventDispatcher()->dispatchCustomEvent(Constants::GameEvent::SP_ADDED_FROM_DISTRIBUTER, distributerSelectedTile);
		}
		else
		{
			cancelDrag();
		}
	}
	isMouseDown = false;

	if (_dragIndicator){
		_dragIndicator->remove();
		_dragIndicator->refresh();
		_dragIndicator = nullptr;
	}
	mouseDownAtX = currentMouseX;
	mouseDownAtY = currentMouseY;
}

void DragAndDropSystem::onDragLayerTouchMove(Touch* touch, Event* e){

	currentMouseX = touch->getLocation().x;
	currentMouseY = touch->getLocation().y;

	onDragLayerInputMove();
}

void DragAndDropSystem::onDragLayerMouseMove(Event* e){
	
	currentMouseX = ((EventMouse*)e)->getCursorX();
	currentMouseY = ((EventMouse*)e)->getCursorY();
	
	onDragLayerInputMove();
}

bool DragAndDropSystem::onDragLayerTouchBegan(Touch* touch, Event* e){
	return true;
}

void DragAndDropSystem::onDragLayerInputMove(){

	if (isDragging){
		draggedTile->setPosition(currentMouseX, currentMouseY);
	}
	else if (!isDragging && isMouseDown && ((std::max(mouseDownAtX, currentMouseX) - std::min(mouseDownAtX, currentMouseX) > 50) ||
											(std::max(mouseDownAtY, currentMouseY) - std::min(mouseDownAtY, currentMouseY) > 50)) ){
		
		isDragging = true;

		draggedTile->setRotation(ceilf(draggedTile->getRotation() / 90) * 90);
		draggedTile->setPosition(currentMouseX, currentMouseY);

		_dragLayer->addChild(draggedTile);

		_dragIndicator = &EntityFactory::createDragIndicator(*world);
	}
}

void DragAndDropSystem::cancelDrag(){


	isDragging = false;
	float w = draggedTile->getContentSize().width*draggedTile->getScale();
	draggedTile->runAction(Sequence::create(MoveTo::create(.5, Vec2(mouseDownAtX+w/2, mouseDownAtY/*+w/2*/)),
											CCCallFunc::create(draggedTile, callfunc_selector(TileRope::removeFromParent)),
											NULL));
	draggedTile->runAction(FadeOut::create(.5));
}


void DragAndDropSystem::ensureImportantComponent(){
	if (nullptr == _gridMatrix){
		auto levelEntity = &(world->getTagManager()->getEntity(Constants::Groups::LEVEL));
		if (levelEntity != nullptr){
			_gridMatrix = (GridMatrix*)levelEntity->getComponent(Constants::ComponentType::GRID_MATRIX);
		}
	}
}