#include "TileDistributer.h"

#include <vector>

TileDistributer::TileDistributer() :availableTileTypes(nullptr), totalType(0)
{
	this->tiles = Vector<TileRope*>();
}


TileDistributer::~TileDistributer()
{
	this->tiles.clear();
}


void TileDistributer::distributeTiles(int* availableTileTypes, int totalType, int nbToDisplay){

	
	std::vector<int> availableTypes;
	this->nbToDisplay = nbToDisplay;
	this->totalType = totalType;
	this->availableTileTypes = new int[totalType];

	for (int i = 0; i < totalType; i++){
		this->availableTileTypes[i] = availableTileTypes[i];
		availableTypes.push_back(availableTileTypes[i]);
	}


	for (int i = 0; i < this->nbToDisplay; i++){

		//Create a random tile
		int randomIndex = rand() % availableTypes.size();
		int type = availableTypes.at(randomIndex);
		availableTypes.erase(availableTypes.begin() + randomIndex);

		Vec2 tilePos(0, i*(tileSize.height + 10));
		TileRope * tileRope = TileRope::createFromType(type);
		tileRope->setScale(tileSize.height / 100);
		tileRope->setPosition(tilePos);
		tileRope->setOpacity(0);
		tileRope->runAction(FadeIn::create(1));
		this->tiles.pushBack(tileRope);

		this->addChild(tileRope);
	}

	//Events Listeners
	EventListenerMouse* tileDistributerMouseEventListener = EventListenerMouse::create();
	tileDistributerMouseEventListener->onMouseDown = CC_CALLBACK_1(TileDistributer::onMouseDown, this);
	tileDistributerMouseEventListener->onMouseUp = CC_CALLBACK_1(TileDistributer::onMouseUp, this);
	
	getEventDispatcher()->addEventListenerWithSceneGraphPriority(tileDistributerMouseEventListener, this);

#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
	EventListenerTouchOneByOne* tileDistributerTouchOneByOneListener = EventListenerTouchOneByOne::create();
	tileDistributerTouchOneByOneListener->onTouchBegan = CC_CALLBACK_2(TileDistributer::onTouchBegan, this);
	tileDistributerTouchOneByOneListener->onTouchEnded = CC_CALLBACK_2(TileDistributer::onTouchEnded, this);
	getEventDispatcher()->addEventListenerWithSceneGraphPriority(tileDistributerTouchOneByOneListener, this);
#endif

	Director::getInstance()->getEventDispatcher()->addCustomEventListener(Constants::GameEvent::SP_ADDED_FROM_DISTRIBUTER, CC_CALLBACK_1(TileDistributer::onSeaPathAddedFromDistributer, this));

	
}
void TileDistributer::replaceTileRandomly(TileRope* tileToReplace){

	int newTileType = 1;
	do{
		newTileType = rand() % totalType + 1;
	} while (newTileType == tileToReplace->tileType);

	TileRope * tileRope = TileRope::createFromType(newTileType);
	tileRope->setScale(tileSize.height / 100);
	tileRope->setPosition(tileToReplace->getPosition());
	tileRope->setOpacity(0);
	this->addChild(tileRope);

	this->tiles.replace(this->tiles.getIndex(tileToReplace), tileRope);

	auto replaceSequence = Sequence::createWithTwoActions(
		FadeOut::create(.5),
		CCCallFunc::create(tileToReplace, callfunc_selector(TileRope::removeFromParent)));
	
	tileToReplace->runAction(replaceSequence);
	tileRope->runAction(FadeIn::create(.5));
}


// Mouse Event
void TileDistributer::onMouseUp(Event* e){
	
	if (_mouseDownOnTile == nullptr)
		return;

	EventMouse* eMouse = (EventMouse*)e;
	onInputUp(eMouse->getCursorX(), eMouse->getCursorY());
}

void TileDistributer::onMouseDown(Event* e){

	EventMouse* eMouse = (EventMouse*)e;
	onInputDown(eMouse->getCursorX(), eMouse->getCursorY());
}

//Touch Event
bool  TileDistributer::onTouchBegan(Touch* touch, Event* e){
	onInputDown(touch->getLocation().x, touch->getLocation().y);
	return true;
}
void TileDistributer::onTouchEnded(Touch* touch, Event* e){
	if (_mouseDownOnTile == nullptr)
		return;

	onInputUp(touch->getLocation().x, touch->getLocation().y);
}

//Input Handling Logics

void TileDistributer::onInputUp(float cursorX, float cursorY){

	if (_mouseDownOnTile != nullptr)
		_mouseDownOnTile->setOpacity(255);

	TileRope* tile = getTileAt(cursorX, cursorY);
	if (tile == nullptr)
		return;
	//Click  on a Tile Detected
	if (tile == _mouseDownOnTile)
		Director::getInstance()->getEventDispatcher()->dispatchCustomEvent(Constants::GameEvent::SP_DISTRIBUTER_ITEM_CLICK, tile);
	
	

}
void TileDistributer::onInputDown(float cursorX, float cursorY){
	TileRope* tile = getTileAt(cursorX, cursorY);
	_mouseDownOnTile = tile;
	//Mouse Down on a Tile Detected
	if (tile != nullptr){
		tile->setOpacity(100);
		Director::getInstance()->getEventDispatcher()->dispatchCustomEvent(Constants::GameEvent::SP_DISTRIBUTER_ITEM_MOUSEDOWN, tile);
	}
}

void TileDistributer::removeTile(TileRope* tileRope){

	int index = tiles.getIndex(tileRope);
	if (index >= 0)
		tiles.erase(index);

	tileRope->setOpacity(100);
	tileRope->runAction(Sequence::createWithTwoActions(
		FadeOut::create(.5),
		CCCallFunc::create(tileRope, callfunc_selector(TileRope::removeFromParent))));

	if (tiles.size() == 0){
		distributeTiles(availableTileTypes, totalType, nbToDisplay);
	}
}

// CustomEvent Handlers

void TileDistributer::onSeaPathAddedFromDistributer(EventCustom *e){

	removeTile((TileRope*)e->getUserData());
	//replaceTileRandomly((TileRope*)e->getUserData());
}

// Helpers

TileRope* TileDistributer::getTileAt(float x, float y){

	TileRope* tile = nullptr;
	for (int i = 0; i < tiles.size(); i++){

		tile = tiles.at(i);

		Vec2 tilePos = tile->convertToNodeSpace(Vec2(x,y));
		Rect rect(0, 0, 100, 100);

		if (!rect.containsPoint(tilePos))
			tile = nullptr;
		else
			break;
	}
	return tile;
}


