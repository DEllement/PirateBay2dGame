#pragma once

#include "cocos2d.h"

#include "Artemis/Artemis.h"

#include "SeaPath.h"
#include "GridElement.h"
#include "TileBoard.h"
#include "GridMatrix.h"
#include "Position.h"
#include "EnemyData.h"
#include "ShipData.h"
#include "Motion.h"
#include "VortexData.h"

USING_NS_CC;

class GridRenderSystem : public artemis::EntityProcessingSystem
{
public:
	GridRenderSystem();
	~GridRenderSystem();


	cocos2d::Node* getCanvas() const{
		return _canvas;
	}

	virtual void initialize() override;

	void onMouseDown(Event* eventMode);
	void onMouseUp(Event* eventMouse);
	void onMouseMove(Event* eventMouse);
	bool onTouchBegan(Touch* touch, Event* e);
	void onTouchMove(Touch* touch, Event* e);
	void onTouchEnded(Touch* touch, Event* e);
	void onInputUp(float cursorX, float cursorY);
	void onInputMove(float cursorX, float cursorY);
	void onInputDown(float cursorX, float cursorY);

	virtual void begin() override;
	virtual void end() override;
	virtual void removed(artemis::Entity &e) override;
	virtual void added(artemis::Entity &e) override;
	virtual void processEntities(artemis::ImmutableBag<artemis::Entity*> &bag) override;
	virtual void processEntity(artemis::Entity &e) override;
	virtual bool checkProcessing() override;
protected:
	bool isMouseDown;
	Vec2 mouseDownAtPoint;
	Vec2 currentMousePos;

private:
	Node* _canvas;
	TileBoard* _tileBoard;
	GridMatrix* _gridMatrix;
	artemis::Bag<SeaPath*>* _seaPaths;

	float tileScale = 0.88;
	float markScale = 0.75;

	artemis::ComponentMapper<ShipData> _shipDatasMapper;
	artemis::ComponentMapper<SeaPath> _seaPathsMapper;
	artemis::ComponentMapper<GridElement> _gridElementsMapper;
	artemis::ComponentMapper<Position> _positionsMapper;
	artemis::ComponentMapper<EnemyData> _enemyDatasMapper;
	artemis::ComponentMapper<Motion> _motionsMapper;
	artemis::ComponentMapper<VortexData> _vortexDatasMapper;

	void ensureGridMatrix();
	Vec2 getPositionFromGridElement(GridElement* gridElement);
};

