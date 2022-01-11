#pragma once

#include "Artemis/Artemis.h"
#include "Array2D.h"
#include "GridElement.h"
#include "DijkstraAlgorithm.h"
#include "SeaPath.h"
#include "Position.h"
#include "ShipData.h"

class GridMatrix : public artemis::Component
{
public:
	GridMatrix();
	GridMatrix(int nbCols, int nbRows);
	~GridMatrix();

	int nbCols;
	int nbRows;

	bool isCellAvailable(int col, int row);
	bool isCellOutOfGrid(int col, int row);
	bool canDropSeaPathAt(int col, int row);
	bool haveFixedSeaPathAround(int col, int row);
	bool haveMovingSeaPathAround(int col, int row);
	void set(GridElement* gridElement);
	void setSeaPath(SeaPath* seaPath, int col, int row);
	void setShipData(ShipData* shipData, int col, int row);
	GridElement* elementAt(int col, int row);
	SeaPath* seaPathAt(int col, int row);
	SeaPath* getFirstMovingSeaPathAround(int col, int row);
	ShipData* shipDataAt(int col, int row);
	Position* getRandomEdgePosition(int* possibleRowsOrCols, int totalPossible);
	Position* getRandomPosition();
	Position* getRandomAvailablePosition();

	std::vector<GridElement*> getPathToDestination(int startCol, int startRow, int endCol, int endRow);

	void nullAllSeaPaths();
private:
	Array2D<GridElement*>* _gridElementsMatrix;
	Array2D<SeaPath*>* _seaPathsMatrix;
	Array2D<ShipData*>* _shipDatasMatrix;
	void init();
};

