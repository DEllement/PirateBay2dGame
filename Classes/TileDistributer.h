#pragma once

#include "cocos2d.h"
#include "TileRope.h"
#include "Constants.h"

USING_NS_CC;

class TileDistributer :
	public Sprite
{
public:
	TileDistributer();
	~TileDistributer();

	float speed;
	int* availableTileTypes;
	int totalType;
	int nbToDisplay;
	Vector<TileRope*> tiles;
	Size tileSize;

	CREATE_FUNC(TileDistributer);

	void distributeTiles(int* availableTileTypes, int totalType, int nbToDisplay);
	void onMouseUp(Event* e);
	void onMouseDown(Event* e);
	bool onTouchBegan(Touch* touch, Event* e);
	void onTouchEnded(Touch* touch, Event* e);
	void onInputUp(float cursorX, float cursorY);
	void onInputDown(float cursorX, float cursorY);

	TileRope* getTileAt(float x, float y);
	void replaceTileRandomly(TileRope* tileIndex);
	void removeTile(TileRope* tileRope);
	void onSeaPathAddedFromDistributer(EventCustom *e);

private:
	TileRope* _mouseDownOnTile;
	

};

