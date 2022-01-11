#pragma once
#include "Artemis/artemis.h"

#include "cocos2d.h"

#include "Boat.h"
#include "Position.h"
#include "GridElement.h"
#include "GridMatrix.h"
#include "ShipData.h"

class ShipSystem : public artemis::EntityProcessingSystem
{
public:
	ShipSystem();
	~ShipSystem();

	virtual void initialize() override;
	virtual void begin() override;
	virtual void processEntities(artemis::ImmutableBag<artemis::Entity*> &bag) override;
	virtual void processEntity(artemis::Entity &e) override;
	virtual void end() override;
	virtual void removed(artemis::Entity &e) override;
	virtual void added(artemis::Entity &e) override;
	virtual bool checkProcessing() override;

	void onGridClick(EventCustom* e);

protected:
	artemis::ComponentMapper<ShipData> _shipDatasMapper;
	artemis::ComponentMapper<Position> _positionsMapper;
	artemis::ComponentMapper<GridElement> _gridElementsMapper;

	void onShipClicked(artemis::Entity& shipEntity);
	void onSeaPathClicked(artemis::Entity& seaPathEntity);

private:
	bool isBoatSelected;
	artemis::Entity* _selectedShip;
	GridMatrix* _gridMatrix;
	void ensureGridMatrix();
};

