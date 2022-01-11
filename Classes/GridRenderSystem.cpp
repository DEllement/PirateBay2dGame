#include "GridRenderSystem.h"

#include "Constants.h"

//Events
#include "GridEvent.h"

//Components
#include "Position.h"
#include "GridElement.h"
#include "PlayerData.h"
#include "SeaPath.h"
#include "DragIndicator.h"

//Rendering Components
#include "TileRope.h"
#include "TileBoard.h"
#include "Boat.h"
#include "Kraken.h"
#include "ObjectiveData.h"
#include "Vortex.h"
#include "TargetMark.h"

GridRenderSystem::GridRenderSystem() :_gridMatrix(nullptr)
{
	addComponentType<GridElement>();
	addComponentType<Position>();

	_seaPaths = new artemis::Bag<SeaPath*>();

	_canvas = Layer::create();
	_canvas->retain();

	currentMousePos = Vec2(0,0);
}

GridRenderSystem::~GridRenderSystem()
{
	_seaPaths->deleteData();
	delete _seaPaths;

	CC_SAFE_RELEASE(_canvas);
}

void GridRenderSystem::ensureGridMatrix(){
	if (nullptr == _gridMatrix){
		auto levelEntity = &(world->getTagManager()->getEntity(Constants::Groups::LEVEL));
		if (levelEntity != nullptr){
			_gridMatrix = (GridMatrix*)levelEntity->getComponent(Constants::ComponentType::GRID_MATRIX);
		}
	}

}

void GridRenderSystem::initialize(){

	_shipDatasMapper.init(*world);
	_seaPathsMapper.init(*world);
	_gridElementsMapper.init(*world);
	_positionsMapper.init(*world);
	_enemyDatasMapper.init(*world);
	_motionsMapper.init(*world);
	_vortexDatasMapper.init(*world);

	_tileBoard = TileBoard::CreateWithParams(Constants::GameParams::nbCols,
											 Constants::GameParams::nbRows,
											 CCSize(Constants::GameParams::gridElementWidth,
											 Constants::GameParams::gridElementHeight));
	_tileBoard->setPosition(Vec2(295, 105));
	_canvas->addChild(_tileBoard);

	//Events Listeners
	EventListenerMouse* mouseEventListener = EventListenerMouse::create();
	mouseEventListener->onMouseDown = CC_CALLBACK_1(GridRenderSystem::onMouseDown, this);
	mouseEventListener->onMouseMove = CC_CALLBACK_1(GridRenderSystem::onMouseMove, this);
	mouseEventListener->onMouseUp = CC_CALLBACK_1(GridRenderSystem::onMouseUp, this);
	_tileBoard->getEventDispatcher()->addEventListenerWithSceneGraphPriority(mouseEventListener, _tileBoard);

#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
	EventListenerTouchOneByOne* eventListenerTouchOneByOne = EventListenerTouchOneByOne::create();
	eventListenerTouchOneByOne->onTouchBegan = CC_CALLBACK_2(GridRenderSystem::onTouchBegan, this);
	eventListenerTouchOneByOne->onTouchMoved = CC_CALLBACK_2(GridRenderSystem::onTouchMove, this);
	eventListenerTouchOneByOne->onTouchEnded = CC_CALLBACK_2(GridRenderSystem::onTouchEnded, this);
	_tileBoard->getEventDispatcher()->addEventListenerWithSceneGraphPriority(eventListenerTouchOneByOne, _tileBoard);
#endif


}

/* virtual override Implementation */
bool GridRenderSystem::checkProcessing(){
	return true;
}
void GridRenderSystem::begin(){
	ensureGridMatrix();
}
void GridRenderSystem::processEntities(artemis::ImmutableBag<artemis::Entity*> &bag){

	EntityProcessingSystem::processEntities(bag);

}
void GridRenderSystem::processEntity(artemis::Entity &e){

	auto position = _positionsMapper.get(e);
	auto shipData = _shipDatasMapper.get(e);
	auto seaPath = _seaPathsMapper.get(e);
	auto enemyData = _enemyDatasMapper.get(e);
	auto vortexData = _vortexDatasMapper.get(e);
	auto gridElement = _gridElementsMapper.get(e);

	//Ship
	if (shipData != nullptr){

		auto boat = (Boat*)_tileBoard->getChildByName(Constants::Tags::PLAYER_SHIP + std::to_string(e.getId())); //Note: could keep a ref to be faster...
		position->setLocation(boat->getPositionX(), boat->getPositionY());

		//Check if boat just got selected and run proper animation
		if (shipData->isBoatJustSelected){
			
			boat->runShipSelectedAnimation();
			shipData->isBoatJustSelected = false;
			//Retreive the target mark associeted with that ship
			/*artemis::ImmutableBag<artemis::Entity*>* objectives = world->getGroupManager()->getEntities(Constants::Groups::OBJECTIVES);
			for (int i = 0; i < objectives->getCount(); i++){
				if (objectiveData)
			}*/

		}
		if (shipData->isJustDeselected){
			boat->runShipDeselectedAnimation();
			shipData->isJustDeselected = false;
		}

		//Check if we need to move the boat along a path (Pathfinding visual result)
		if (shipData->startFollowingPath){
			Vector<FiniteTimeAction*> actions;
			for (int i = 0; i < shipData->currentPath.size(); i++){
				actions.pushBack(MoveTo::create(1, Vec2(shipData->currentPath[i]->col * Constants::GameParams::gridElementWidth + (Constants::GameParams::gridElementWidth / 2), shipData->currentPath[i]->row * Constants::GameParams::gridElementHeight + (Constants::GameParams::gridElementHeight/2))));
			}
			actions.pushBack(CallFunc::create(boat, callfunc_selector(Boat::runFloatShipAnimation)));
			auto sequence = Sequence::create(actions);

			boat->runBalanceShipAnimation();
			boat->runAction(sequence);

			shipData->startFollowingPath = false;
		}
		return;
	}//Enemies
	else if (enemyData != nullptr){ 

		switch (enemyData->enemyType){
		case Constants::EnemyType::KRAKEN:{
			Kraken* krakenSprite = (Kraken*)this->_tileBoard->getChildByName("kraken" + std::to_string(e.getUniqueId())); 
			krakenSprite->setPosition(position->getX(), position->getY());
			krakenSprite->setRotation(position->getRotation());
			break;
		}
		case Constants::EnemyType::VORTEX:{
			Vortex* vortexSprite = (Vortex*)this->_tileBoard->getChildByName("vortex" + std::to_string(e.getUniqueId()));
			vortexSprite->setPosition(position->getX(), position->getY());
			vortexSprite->setRotation(position->getRotation());

			if (vortexData->showAtInterval){
				if (vortexSprite->getChildren().at(0)->getOpacity() == 255 && !vortexData->isActive){
					vortexSprite->getChildren().at(0)->setOpacity(254);
					vortexSprite->getChildren().at(0)->runAction(FadeTo::create(.5, 100));
				}
				else if (vortexSprite->getChildren().at(0)->getOpacity() == 100 && vortexData->isActive){
					vortexSprite->getChildren().at(0)->setOpacity(101);
					vortexSprite->getChildren().at(0)->runAction(FadeTo::create(.5, 255));
				}
			}

			break;
		}
		case Constants::EnemyType::MINE:{


			break;
		}
		case Constants::EnemyType::PIRATE_SHIP:{

			break;
		}
		}
				
	


		if (gridElement->toBeRemoved){
			//krakenSprite->release();
		}
		return;
	}
	//Moving SeaPath...
	else if (seaPath != nullptr && seaPath->isAMovingSeaPath){

		Node* node = _tileBoard->getChildByName("seaPath" + std::to_string(e.getUniqueId()));
		node->setPosition(position->getX(), position->getY());
		return;
	}//Normal SeaPath...
	else if (seaPath != nullptr){
		
		if (seaPath->isDirty){
			TileRope* tileRope = (TileRope*)_tileBoard->getChildByName("seaPath" + std::to_string(e.getUniqueId()));
			tileRope->updateTextureToType(seaPath->tileType);
			tileRope->setRotation(seaPath->rotation);
			position->setRotation(seaPath->rotation);
			seaPath->isDirty = false;
		}
		if (!seaPath->isAMovingSeaPath && gridElement->justDropped){
			TileRope* tileRope = (TileRope*)_tileBoard->getChildByName("seaPath" + std::to_string(e.getUniqueId()));
			tileRope->stopAllActions();
			tileRope->runAction(MoveTo::create(.5, getPositionFromGridElement(gridElement)));
			tileRope->setOpacity(255);
			if (tileRope->getScale() == 0)
				tileRope->runAction(ScaleTo::create(.5,tileScale));

			gridElement->justDropped = false;
		}

		return;
	}//DragIndicator
	else if (e.getComponent<DragIndicator>() != nullptr){

		auto dragIndicator = (DragIndicator*)e.getComponent<DragIndicator>();
		position->setLocation(currentMousePos.x - _tileBoard->getPositionX(), currentMousePos.y - _tileBoard->getPositionY());
		DrawNode* drawNode = (DrawNode*)_tileBoard->getChildByName("dragIndicator");
		drawNode->clear();

		if (dragIndicator->isOutOfGrid || !dragIndicator->canDropHere)
			return;

		float gW = Constants::GameParams::gridElementWidth;
		float gH = Constants::GameParams::gridElementHeight;
		
		//Vertical Rect
		Vec2 vVertices[] =
		{
			Vec2(gridElement->col*gW, gH * Constants::GameParams::nbRows),
			Vec2((gridElement->col*gW) + gW, gH * Constants::GameParams::nbRows),
			Vec2((gridElement->col*gW) + gW, 0),
			Vec2(gridElement->col*gW, 0)
		};
		drawNode->drawPolygon(vVertices, 4, Color4F(1.0f, 1.0f, 1.0f, .2), 0, Color4F(1.0f, 1.0f, 1.0f, 1));
		//Horizontal
		Vec2 hVertices[] =
		{
			Vec2(0, (gridElement->row*gH) + gH),
			Vec2(gW * Constants::GameParams::nbCols, (gridElement->row*gH) + gH),
			Vec2(gW * Constants::GameParams::nbCols, (gridElement->row*gH)),
			Vec2(0, (gridElement->row*gH))
		};
		drawNode->drawPolygon(hVertices, 4, Color4F(1.0f, 1.0f, 1.0f, .2), 0, Color4F(1.0f, 1.0f, 1.0f, 1));
	}
	else if (e.getComponent<TargetMark>() != nullptr){
	
		std::string eId = std::to_string(e.getId());
		auto targetMark = (TargetMark*)e.getComponent<TargetMark>();
		auto _targetSprite = this->_tileBoard->getChildByName("TargetMark" + eId);

		//Show the Mark
		if (_targetSprite && !_targetSprite->isVisible()){

			_targetSprite->setOpacity(0);
			_targetSprite->setScale(3);
			_targetSprite->setVisible(true);

			_targetSprite->runAction(FadeIn::create(.75));
			_targetSprite->runAction(ScaleTo::create(.75, markScale));
		}

		//Associeted Ship Selection/Deselection
		if (targetMark->associatedShipJustSelected){
			_targetSprite->runAction(RepeatForever::create(Sequence::createWithTwoActions(ScaleTo::create(.5, 1), ScaleTo::create(.5, markScale))));
			targetMark->associatedShipJustSelected = false; //NOTE: Shouldnt be the objective system responsbility?
		}
		if (targetMark->associatedShipJustDeSelected){
			_targetSprite->stopAllActions();
			_targetSprite->setScale(markScale);
			targetMark->associatedShipJustDeSelected = false;
		}



	}
	else if (e.getComponent<ObjectiveData>() != nullptr){
		//NOTE: Do i need this ???

		auto objectiveData = (ObjectiveData*)e.getComponent<ObjectiveData>();
		//std::string eId = std::to_string(e.getId());

		//Process Logic by Type
		switch (objectiveData->objectiveType){
		case ObjectiveData::ObjectiveType::REACH_THE_MARK:
			
			break;
		case ObjectiveData::ObjectiveType::BOATS_MUST_SURVIVE:
			
			break;
		case ObjectiveData::ObjectiveType::DEFEAT_ENNEMY:
			
			break;
		case ObjectiveData::ObjectiveType::ITEM_USAGE:
			
			break;
		}
	}
}
void GridRenderSystem::end(){

}
void GridRenderSystem::added(artemis::Entity &e){

	
	if (_shipDatasMapper.get(e) != nullptr){
		Boat *boat = Boat::create();
		boat->setName(Constants::Tags::PLAYER_SHIP + std::to_string(e.getId()));

		GridElement* gridElement = _gridElementsMapper.get(e);
		boat->setPosition(getPositionFromGridElement(gridElement));
		boat->setZOrder(1000);

		Position* position = _positionsMapper.get(e);
		position->setLocation(boat->getPositionX(), boat->getPositionY());
		this->_tileBoard->addChild(boat);
	}
	else if (_seaPathsMapper.get(e) != nullptr){

		SeaPath* seaPath = _seaPathsMapper.get(e);
		
		//Create tile...
		auto ropeTile = TileRope::createFromType(seaPath->tileType);
		ropeTile->setScale(tileScale); //Constants::GameParams::gridElementWidth/realsize
		ropeTile->setUserData(&e);
		ropeTile->setName("seaPath" + std::to_string(e.getUniqueId()));

		GridElement* gridElement = _gridElementsMapper.get(e);
		Position* position = _positionsMapper.get(e);
		this->_tileBoard->addChild(ropeTile);
		if (gridElement->justDropped)
		{
			ropeTile->setPosition(position->getX(), position->getY());
			ropeTile->setRotation(position->getRotation());

			if (seaPath->isAMovingSeaPath){
				
				//Adjust the position related to the linked one
				if (seaPath->linkedMovingSeaPathEntityId > 0){
					artemis::Entity& linkedMovingSeaPathEntity = world->getEntity(seaPath->linkedMovingSeaPathEntityId);
					auto motion = (Motion*)e.getComponent<Motion>();
					auto linkedPos = (Position*)linkedMovingSeaPathEntity.getComponent<Position>();
					auto linkedGridEle = (GridElement*)linkedMovingSeaPathEntity.getComponent<GridElement>();
					auto cellDestPos = getPositionFromGridElement(gridElement);

					float offsetX = (gridElement->col - linkedGridEle->col)*Constants::GameParams::gridElementWidth;
					float offsetY = (gridElement->row - linkedGridEle->row)*Constants::GameParams::gridElementHeight;

					position->setLocation(offsetX == 0 ? linkedPos->getX() : linkedPos->getX() + offsetX,
										  offsetY == 0 ? linkedPos->getY() : linkedPos->getY() + offsetY);
					ropeTile->setPosition(position->getX(), position->getY());
				}
				
				ropeTile->runAction(RepeatForever::create(Sequence::createWithTwoActions(FadeIn::create(.5), FadeTo::create(.5, 80))));
			}
			else{
				//MoveToCellCenter...
				ropeTile->runAction(MoveTo::create(.5, getPositionFromGridElement(gridElement)));
			}
			gridElement->justDropped = false;

			
		}
		else{
			ropeTile->setScale(0);
			ropeTile->setOpacity(.5);
			ropeTile->setPosition(getPositionFromGridElement(gridElement));
			ropeTile->setRotation(position->getRotation());
			position->setLocation(ropeTile->getPositionX(), ropeTile->getPositionY());
			ropeTile->runAction(ScaleTo::create(.5, tileScale));

			if (seaPath->isAMovingSeaPath){
				ropeTile->runAction(RepeatForever::create(Sequence::createWithTwoActions(FadeIn::create(.5), FadeTo::create(.5, 80))));
			}
			else{
				ropeTile->runAction(FadeIn::create(.5));
				ropeTile->runAction(ScaleTo::create(.5, tileScale));
			}

		}
		gridElement->isOnGrid = true;
	}
	else if (_enemyDatasMapper.get(e) != nullptr){

		EnemyData* enemyData = _enemyDatasMapper.get(e);

		switch (enemyData->enemyType)
		{
			case Constants::EnemyType::KRAKEN:
			{
				Kraken* krakenSprite = Kraken::create();
				krakenSprite->retain();
				krakenSprite->setName("kraken" + std::to_string(e.getUniqueId()));
				GridElement* gridElement = _gridElementsMapper.get(e);
				Position* position = _positionsMapper.get(e);

				krakenSprite->setPosition(getPositionFromGridElement(gridElement));
				krakenSprite->setZOrder(1001);

				position->setLocation(krakenSprite->getPositionX(), krakenSprite->getPositionY());

				this->_tileBoard->addChild(krakenSprite);
				//krakenSprite->runAction(Sequence::createWithTwoActions(MoveBy::create(1, Vec2(10, 0)), MoveBy::create(1, Vec2(-10, 0))));

				break;
			}
			case Constants::EnemyType::VORTEX:
			{
				Vortex* vortexSprite = Vortex::create();
				vortexSprite->retain();
				vortexSprite->setName("vortex" + std::to_string(e.getUniqueId()));
				GridElement* gridElement = _gridElementsMapper.get(e);
				Position* position = _positionsMapper.get(e);
				VortexData* vortexData = _vortexDatasMapper.get(e);

				vortexSprite->setPosition(getPositionFromGridElement(gridElement));
				if (vortexData->colspan > 1 && vortexData->rowspan > 1){
					vortexSprite->setInnerSpriteOffetPosition(Vec2(
						((Constants::GameParams::gridElementWidth / 2)*vortexData->colspan / 2),
						(((Constants::GameParams::gridElementHeight / 2)*vortexData->rowspan / 2))));
				}

				vortexSprite->setZOrder(1001);

				position->setLocation(vortexSprite->getPositionX(), vortexSprite->getPositionY());

				vortexSprite->getChildren().at(0)->setScale(0);
				vortexSprite->getChildren().at(0)->runAction(ScaleTo::create(1,
					(Constants::GameParams::gridElementWidth*vortexData->colspan) / vortexSprite->getContentSize().width,
					(Constants::GameParams::gridElementHeight*vortexData->rowspan) / vortexSprite->getContentSize().height)
					);

				this->_tileBoard->addChild(vortexSprite);

				break;
			}
			case Constants::EnemyType::MINE:
			{

				break;
			}
			case Constants::EnemyType::PIRATE_SHIP:
			{

				break;
			}
		}
	}
	else if (e.getComponent<DragIndicator>() != nullptr){
	
		DrawNode* drawNode = DrawNode::create();
		drawNode->setName("dragIndicator");
		drawNode->setZOrder(0);
		this->_tileBoard->addChild(drawNode);
	}
	else if (e.getComponent<TargetMark>() != nullptr){
	
		std::string eId = std::to_string(e.getId());
		auto _targetSprite = Sprite::create("red_mark.png");
		_targetSprite->setScale(markScale);
		_targetSprite->setPosition(getPositionFromGridElement((GridElement*)e.getComponent<GridElement>()));
		_targetSprite->setLocalZOrder(999);
		_targetSprite->setVisible(false);
		_targetSprite->setName("TargetMark" + eId);
		_targetSprite->retain();

		this->_tileBoard->addChild(_targetSprite);

		//Update MarkTarget's Position Component
		Position* targetPosition = (Position*)e.getComponent<Position>();
		targetPosition->setLocation(_targetSprite->getPositionX(), _targetSprite->getPositionY());
		targetPosition->worldX = _tileBoard->getPositionX() + _targetSprite->getPositionX(); //Used by Other Systems such as the hud
		targetPosition->worldY = _tileBoard->getPositionY() + _targetSprite->getPositionY(); //
	}
}
void GridRenderSystem::removed(artemis::Entity &e){
	//Remove it from the Grid Layer

	//Handle SeaPath Removal Animation/Rendering
	SeaPath* seaPath = _seaPathsMapper.get(e);
	if (seaPath != nullptr){

		Node* node = _tileBoard->getChildByName("seaPath" + std::to_string(e.getUniqueId()));
		if (seaPath->isAMovingSeaPath)
			node->stopAllActions();

		auto removeSequence = Sequence::createWithTwoActions(
			FadeOut::create(.5),
			CCCallFunc::create(node, callfunc_selector(TileRope::removeFromParent)));
		node->runAction(removeSequence);
		return;
	}

	//Handle Enemy Removal
	auto enemyData = _enemyDatasMapper.get(e);
	if (enemyData != nullptr){

		switch (enemyData->enemyType){
			case Constants::EnemyType::KRAKEN:{
		
				Kraken* krakenSprite = (Kraken*)this->_tileBoard->getChildByName("kraken" + std::to_string(e.getUniqueId()));

				auto removeSequence = Sequence::createWithTwoActions(
					FadeOut::create(.5),
					CCCallFunc::create(krakenSprite, callfunc_selector(TileRope::removeFromParent)));
				krakenSprite->runAction(removeSequence);

				break;
			}
			case Constants::EnemyType::VORTEX:{
			
				Vortex* vortexSprite = (Vortex*)this->_tileBoard->getChildByName("vortex" + std::to_string(e.getUniqueId()));

				auto removeSequence = Sequence::createWithTwoActions(
					ScaleTo::create(1,0),
					CCCallFunc::create(vortexSprite, callfunc_selector(TileRope::removeFromParent)));
				vortexSprite->getChildren().at(0)->runAction(removeSequence);

				break;
			}
			case Constants::EnemyType::MINE:{
			


				break;
			}
			case Constants::EnemyType::PIRATE_SHIP:{
			
				break;
			}
		}

		return;
	}

	//Handle TargetMark Removal / Completion
	if (e.getComponent<TargetMark>() != nullptr){

		std::string eId = std::to_string(e.getId());
		auto endSprite = this->_tileBoard->getChildByName("TargetMark" + eId);
		
		auto removeSequence = Sequence::createWithTwoActions(
			FadeOut::create(2),
			CCCallFunc::create(endSprite, callfunc_selector(TileRope::removeFromParent)));
		endSprite->runAction(removeSequence);
	}

	//Handle Ship Removal
	if (_shipDatasMapper.get(e) != nullptr){
		Boat* boat = (Boat*)_tileBoard->getChildByName(Constants::Tags::PLAYER_SHIP + std::to_string(e.getId()));
		boat->stopAllActions();
		boat->runAction(Sequence::createWithTwoActions(FadeOut::create(.5), CCCallFunc::create(boat, callfunc_selector(Node::removeFromParent))));
	}
	//Handle Drag Indicator Removal
	if (e.getComponent<DragIndicator>() != nullptr){

		DrawNode* drawNode = (DrawNode*)_tileBoard->getChildByName("dragIndicator");
		drawNode->removeFromParent();
	}
}


//Mouse Event Handlers
void GridRenderSystem::onMouseDown(Event* e){
	EventMouse* em = (EventMouse*)e;
	onInputDown(em->getCursorX(), em->getCursorY());
}

void GridRenderSystem::onMouseMove(Event* e){
	auto em = (EventMouse*)e;
	onInputMove(em->getCursorX(), em->getCursorY());
}
void GridRenderSystem::onMouseUp(Event* e){

	EventMouse* em = (EventMouse*)e;

	onInputUp(em->getCursorX(), em->getCursorY());
}
//Touch Event Handlers
bool GridRenderSystem::onTouchBegan(Touch* touch, Event* e){
	onInputDown(touch->getLocation().x, touch->getLocation().y);
	return true;
}
void GridRenderSystem::onTouchMove(Touch* touch, Event* e){
	onInputMove(touch->getLocation().x, touch->getLocation().y);
}
void GridRenderSystem::onTouchEnded(Touch* touch, Event* e){

	onInputUp(touch->getLocation().x, touch->getLocation().y);
}
//Input Handlers Logics
void GridRenderSystem::onInputDown(float cursorX, float cursorY){

	isMouseDown = true;
	mouseDownAtPoint = Vec2(cursorX, cursorY);
}

void GridRenderSystem::onInputMove(float cursorX, float cursorY){
	currentMousePos.x = cursorX;
	currentMousePos.y = cursorY;

	Director::getInstance()->getEventDispatcher()->dispatchCustomEvent(Constants::GameEvent::GRID_MOUSEMOVE, &currentMousePos);
}

void GridRenderSystem::onInputUp(float cursorX, float cursorY){
	//Regular MouseUp Event:
	GridEvent* gridEvent = new GridEvent();
	gridEvent->y = cursorY - _tileBoard->getPositionY();
	gridEvent->x = cursorX - _tileBoard->getPositionX();
	gridEvent->row = std::floorf(gridEvent->y / Constants::GameParams::gridElementHeight);
	gridEvent->col = std::floorf(gridEvent->x / Constants::GameParams::gridElementWidth);
	gridEvent->isCellAvailable = _gridMatrix != nullptr ? _gridMatrix->isCellAvailable(gridEvent->col, gridEvent->row) : false;
	gridEvent->canDropSeaPath = _gridMatrix != nullptr ? _gridMatrix->canDropSeaPathAt(gridEvent->col, gridEvent->row) : false;
	gridEvent->haveFixedSeaPathAround = _gridMatrix != nullptr ? _gridMatrix->haveFixedSeaPathAround(gridEvent->col, gridEvent->row) : false;
	gridEvent->haveMovingSeaPathAround = _gridMatrix != nullptr ? _gridMatrix->haveMovingSeaPathAround(gridEvent->col, gridEvent->row) : false;
	gridEvent->isInGridBoundary = gridEvent->row >= 0 && gridEvent->row <= 9 && gridEvent->col >= 0 && gridEvent->col <= 9;
	//gridEvent->targetedNode = nullptr;
	
	Director::getInstance()->getEventDispatcher()->dispatchCustomEvent(Constants::GameEvent::GRID_MOUSEUP, gridEvent);

	//Check if is it a click
	float offsetX = std::max(cursorX, mouseDownAtPoint.x) - std::min(cursorX, mouseDownAtPoint.x);
	float offsetY = std::max(cursorY, mouseDownAtPoint.y) - std::min(cursorY, mouseDownAtPoint.y);
	if (isMouseDown && offsetX < Constants::GameParams::gridElementWidth / 2 && offsetY < Constants::GameParams::gridElementHeight / 2){
		Director::getInstance()->getEventDispatcher()->dispatchCustomEvent(Constants::GameEvent::GRID_CLICK, gridEvent);
	}
	
	isMouseDown = false;
}

//Helpers

Vec2 GridRenderSystem::getPositionFromGridElement(GridElement* gridElement){
	return Vec2((gridElement->col * Constants::GameParams::gridElementWidth) + (Constants::GameParams::gridElementWidth / 2), (gridElement->row * Constants::GameParams::gridElementHeight) + (Constants::GameParams::gridElementHeight / 2));
}