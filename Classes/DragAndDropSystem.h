#pragma once

#include "Artemis/artemis.h"

#include "cocos2d.h"

#include "TileRope.h"
#include "GridMatrix.h"

USING_NS_CC;

class DragAndDropSystem : public artemis::EntityProcessingSystem
{
public:
	DragAndDropSystem();
	~DragAndDropSystem();

	virtual void initialize() override;

	Node* getDragLayer() const{
		return _dragLayer;
	}
protected:
	virtual void processEntities(artemis::ImmutableBag<artemis::Entity*>& bag) override;
	virtual void processEntity(artemis::Entity &e) override;
	virtual bool checkProcessing() override;
	virtual void begin() override;
	virtual void end() override;
	virtual void removed(artemis::Entity &e) override;
	virtual void added(artemis::Entity &e) override;

	void onSeaPathsDistributerItemMouseDown(EventCustom* customEvent);
	void onSeaPathsDistributerItemClick(EventCustom* customEvent);
	void onGridMouseUp(EventCustom* customEvent);
	void onDragLayerMouseMove(Event* e);
	bool onDragLayerTouchBegan(Touch* touch, Event* e);
	void onDragLayerTouchMove(Touch* touch, Event* e);
	void onDragLayerInputMove();

	void cancelDrag();
	void ensureImportantComponent();

private:
	GridMatrix* _gridMatrix = nullptr;
	Node* _dragLayer;
	TileRope* draggedTile;
	TileRope* distributerSelectedTile;
	artemis::Entity* _dragIndicator;

	float currentMouseX;
	float currentMouseY;
	float mouseDownAtX;
	float mouseDownAtY;
	bool isMouseDown;
	bool isDragging;
};

