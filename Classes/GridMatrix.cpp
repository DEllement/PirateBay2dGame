#include "GridMatrix.h"
#include "SeaPath.h"
#include "Boat.h"
#include "Constants.h"

GridMatrix::GridMatrix(){
	
	init();

}
GridMatrix::GridMatrix(int nbCols, int nbRows) :
nbCols(nbCols), nbRows(nbRows)
{
	init();
}


GridMatrix::~GridMatrix()
{
}

void GridMatrix::init(){

	_gridElementsMatrix = new Array2D<GridElement*>(nbCols, nbRows);
	_seaPathsMatrix = new Array2D<SeaPath*>(nbCols, nbRows);
	_shipDatasMatrix = new Array2D<ShipData*>(nbCols, nbRows);
	for (int x = 0; x < nbCols; x++){
		for (int y = 0; y < nbRows; y++){
			_gridElementsMatrix->set(nullptr, x, y);
			_seaPathsMatrix->set(nullptr, x, y);
			_shipDatasMatrix->set(nullptr, x, y);
		}
	}
}

bool GridMatrix::isCellAvailable(int col, int row){

	if (isCellOutOfGrid(col, row))
		return false;

	SeaPath* seaPath = _seaPathsMatrix->get(col, row); //Later check other type of components
	if (seaPath != nullptr)
		return false;
	return true;
}
bool GridMatrix::isCellOutOfGrid(int col, int row){
	if (col < 0 || col > nbCols - 1 || row < 0 || row > nbRows - 1)
		return true;
	return false;
}

bool GridMatrix::canDropSeaPathAt(int col, int row){
	if (isCellOutOfGrid(col, row))
		return false;
	if (!isCellAvailable(col, row))
		return false;

	
	/*if (!isCellOutOfGrid(col, row - 1) && seaPathAt(col, row - 1) != nullptr)
		return true;
	if (!isCellOutOfGrid(col, row + 1) && seaPathAt(col, row + 1) != nullptr)
		return true;
	if (!isCellOutOfGrid(col - 1, row) && seaPathAt(col - 1, row) != nullptr)
		return true;
	if (!isCellOutOfGrid(col + 1, row) && seaPathAt(col + 1, row) != nullptr)
		return true;*/

	return true;
}

bool GridMatrix::haveFixedSeaPathAround(int col, int row){
	if (isCellOutOfGrid(col, row))
		return false;
	if (!isCellAvailable(col, row))
		return false;


	if (!isCellOutOfGrid(col, row - 1) && seaPathAt(col, row - 1) != nullptr && !seaPathAt(col, row - 1)->isAMovingSeaPath)
		return true;
	if (!isCellOutOfGrid(col, row + 1) && seaPathAt(col, row + 1) != nullptr && !seaPathAt(col, row + 1)->isAMovingSeaPath)
		return true;
	if (!isCellOutOfGrid(col - 1, row) && seaPathAt(col - 1, row) != nullptr && !seaPathAt(col - 1, row)->isAMovingSeaPath)
		return true;
	if (!isCellOutOfGrid(col + 1, row) && seaPathAt(col + 1, row) != nullptr && !seaPathAt(col + 1, row)->isAMovingSeaPath)
		return true;

	return false;
}

bool GridMatrix::haveMovingSeaPathAround(int col, int row){
	if (isCellOutOfGrid(col, row))
		return false;
	if (!isCellAvailable(col, row))
		return false;


	if (!isCellOutOfGrid(col, row - 1) && seaPathAt(col, row - 1) != nullptr && seaPathAt(col, row - 1)->isAMovingSeaPath)
		return true;
	if (!isCellOutOfGrid(col, row + 1) && seaPathAt(col, row + 1) != nullptr && seaPathAt(col, row + 1)->isAMovingSeaPath)
		return true;
	if (!isCellOutOfGrid(col - 1, row) && seaPathAt(col - 1, row) != nullptr && seaPathAt(col - 1, row)->isAMovingSeaPath)
		return true;
	if (!isCellOutOfGrid(col + 1, row) && seaPathAt(col + 1, row) != nullptr && seaPathAt(col + 1, row)->isAMovingSeaPath)
		return true;

	return false;
}

SeaPath* GridMatrix::getFirstMovingSeaPathAround(int col, int row){

	if (isCellOutOfGrid(col, row))
		return nullptr;
	if (!isCellAvailable(col, row))
		return nullptr;

	if (!isCellOutOfGrid(col, row - 1) && seaPathAt(col, row - 1) != nullptr && seaPathAt(col, row - 1)->isAMovingSeaPath)
		return seaPathAt(col, row - 1);
	if (!isCellOutOfGrid(col, row + 1) && seaPathAt(col, row + 1) != nullptr && seaPathAt(col, row + 1)->isAMovingSeaPath)
		return seaPathAt(col, row + 1);
	if (!isCellOutOfGrid(col - 1, row) && seaPathAt(col - 1, row) != nullptr && seaPathAt(col - 1, row)->isAMovingSeaPath)
		return seaPathAt(col - 1, row);
	if (!isCellOutOfGrid(col + 1, row) && seaPathAt(col + 1, row) != nullptr && seaPathAt(col + 1, row)->isAMovingSeaPath)
		return seaPathAt(col + 1, row);

	return nullptr;

}

GridElement* GridMatrix::elementAt(int col, int row){
	return _gridElementsMatrix->get(col, row);
}

void GridMatrix::set(GridElement* gridElement){
	if(!isCellOutOfGrid(gridElement->col, gridElement->row))
		_gridElementsMatrix->set(gridElement, gridElement->col, gridElement->row);
}
//SeaPath
void GridMatrix::setSeaPath(SeaPath* entity, int col, int row){

	//check previous pos and nullify
	/*bool found = false;
	for (int x = 0; x < nbCols; x++){
		for (int y = 0; y < nbRows; y++){
			if (_seaPathsMatrix->get(x, y) == entity && (x != col || y != row)){
				_seaPathsMatrix->set(nullptr, x, y);
				found = true;
			}
			if (found)
				break;
		}
		if (found)
			break;
	}*/

	if (!isCellOutOfGrid(col, row))
		_seaPathsMatrix->set(entity, col, row);
}
SeaPath* GridMatrix::seaPathAt(int col, int row){
	return _seaPathsMatrix->get(col, row);
}
//ShipData
void GridMatrix::setShipData(ShipData* entity, int col, int row){
	if (!isCellOutOfGrid(col, row))
		_shipDatasMatrix->set(entity, col, row);
}
ShipData* GridMatrix::shipDataAt(int col, int row){
	return _shipDatasMatrix->get(col, row);
}

/* Althought it is the Position component, do not use it as a Component but only use its values */
Position* GridMatrix::getRandomEdgePosition(int* possibleRowsOrCols, int totalPossible){
	

	int startDirection[] = { 0, 1, 2, 3 }; //0 = N, 1 = E, 2 = S, 3 = W
	int startX, startY;
	float rotation;
	switch (std::rand() % 4){
	case 0:
		startY = nbRows - 1;
		startX = possibleRowsOrCols[rand() % totalPossible];
		rotation = 0;
		break;
	case 1:
		startY = possibleRowsOrCols[rand() % totalPossible];
		startX = nbCols - 1;
		rotation = 90;
		break;
	case 2:
		startY = 0;
		startX = possibleRowsOrCols[rand() % totalPossible];
		rotation = 180;
		break;
	case 3:
		startY = possibleRowsOrCols[rand() % totalPossible];
		startX = 0;
		rotation = 270;
		break;
	}

	Position* position = new Position();
	position->setX(std::min(startX, Constants::GameParams::nbCols - 1));
	position->setY(std::min(startY, Constants::GameParams::nbRows - 1));
	position->setRotation(rotation);

	return position;
}
Position* GridMatrix::getRandomPosition(){

	Position* position = new Position();
	position->setX(rand()%nbCols);
	position->setY(rand()%nbRows);

	return position;
}

Position* GridMatrix::getRandomAvailablePosition(){
	
	//Because it is just faster most of the time
	Position* pos = getRandomPosition();
	if( this->isCellAvailable(pos->getX(), pos->getY()) )
		return pos;

	//Or retreive the available random position (Slower)
	std::vector<Vec2> availablePos;
	for (int x = 0; x < nbCols; x++){
		for (int y = nbRows - 1; y >= 0; y--){
			if (this->isCellAvailable(x, y))
				availablePos.push_back(Vec2(x, y));
		}
	}

	if (availablePos.size() == 0)
		return getRandomPosition(); //Just return any in that case or null? (TODO: To improve)

	Vec2 randomPos = availablePos.at(rand() % availablePos.size());
	Position* position = new Position();
	position->setX(randomPos.x);
	position->setY(randomPos.y);
	
	return position;
}

void GridMatrix::nullAllSeaPaths(){
	for (int x = 0; x < nbCols; x++){
		for (int y = nbRows - 1; y >= 0; y--){
			setSeaPath(nullptr, x, y);
		}
	}
}

std::vector<GridElement*> GridMatrix::getPathToDestination(int startCol, int startRow, int endCol, int endRow){

	std::vector<std::vector<neighbor> > adjacency_list((nbCols*nbRows) * 25); 
	Array2D<Array2D<int>*>* vertexesIndex = new Array2D<Array2D<int>*>(nbCols,nbRows);
	std::vector<Vec2> vertexesLocation((nbCols*nbRows) * 25);

	int startVertexIndex = 0;
	int endVertexIndex = 0;
	bool isStartCell = false;
	bool isEndCell = false;

	//Create vertexIndexes
	int currentNodeIndex = 0;
	for (int x = 0; x < nbCols; x++){
		for (int y = nbRows-1; y >=0; y--){
			Array2D<int> *arr = new Array2D<int>(5, 5);
			for (int inner_x = 0; inner_x < 5; inner_x++){					
				for (int inner_y = 0; inner_y < 5; inner_y++){				
					vertexesLocation[currentNodeIndex] = Vec2(x, y);		
					arr->set(currentNodeIndex++, inner_x, inner_y);			
				}															
			}
			vertexesIndex->set(arr, x, y);
		}
	}
	//Create edges
	int NE = 0; // 
	int N1 = 0; // Coord= 0,0  1,0  2,0  3,0  4,0
	int N2 = 0; //        0,1  1,1  2,1  3,1  4,1
	int N3 = 0; //        0,2  1,2  2,2  3,2  4,2
	int NW = 0; //        0,3  1,3  2,3  3,3  4,3
	int W1 = 0; //        0,4  1,4  2,4  3,4  4,4
	int W2 = 0; //
	int W3 = 0; // 
	int E1 = 0; // 
	int E2 = 0; // Var=   NE N1 N2 N3 NW
	int E3 = 0; //        W1          E1
	int SE = 0; //        W2    CV    E2
	int S1 = 0; //		  W3          E3 
	int S2 = 0; //        SE S1 S2 S3 SW	
	int S3 = 0;          
	int SW = 0; 
	int CV = 0;

	currentNodeIndex = 0;
	int total = 0;
	for (int x = 0; x < nbCols; x++){
		for (int y = nbRows - 1; y >= 0; y--){
		
			isStartCell = false;
			isEndCell = false;
			
			NE = vertexesIndex->get(x, y)->get(0, 0);
			N1 = vertexesIndex->get(x, y)->get(1, 0);
			N2 = vertexesIndex->get(x, y)->get(2, 0);
			N3 = vertexesIndex->get(x, y)->get(3, 0);
			NW = vertexesIndex->get(x, y)->get(4, 0);
			W1 = vertexesIndex->get(x, y)->get(0, 1);
			W2 = vertexesIndex->get(x, y)->get(0, 2);
			W3 = vertexesIndex->get(x, y)->get(0, 3);
			E1 = vertexesIndex->get(x, y)->get(4, 1);
			E2 = vertexesIndex->get(x, y)->get(4, 2);
			E3 = vertexesIndex->get(x, y)->get(4, 3);
			SE = vertexesIndex->get(x, y)->get(0, 4);
			S1 = vertexesIndex->get(x, y)->get(1, 4);
			S2 = vertexesIndex->get(x, y)->get(2, 4);
			S3 = vertexesIndex->get(x, y)->get(3, 4);
			SW = vertexesIndex->get(x, y)->get(4, 4);
			CV = vertexesIndex->get(x, y)->get(2, 2);

			if (x == startCol && y == startRow){
				startVertexIndex = CV;
				isStartCell = true;
			}
			if (x == endCol && y == endRow){
				endVertexIndex = CV;
				isEndCell = true;
			}

			int upNeighborSouthVertexIndex1 = y < (nbRows)-1 ? vertexesIndex->get(x, y + 1)->get(1, 4) : -1;
			int upNeighborSouthVertexIndex3 = y < (nbRows)-1 ? vertexesIndex->get(x, y + 1)->get(3, 4) : -1;
			int leftNeighborEastVertexIndex1 = x > 0 ? vertexesIndex->get(x - 1, y)->get(4, 1) : -1;
			int leftNeighborEastVertexIndex3 = x > 0 ? vertexesIndex->get(x - 1, y)->get(4, 3) : -1;
			int rightNeighborWestVertexIndex1 = x < (nbCols)-1 ? vertexesIndex->get(x + 1, y)->get(0, 1) : -1;
			int rightNeighborWestVertexIndex3 = x < (nbCols)-1 ? vertexesIndex->get(x + 1, y)->get(0, 3) : -1;
			int downNeighborNorthVertexIndex1 = y > 0 ? vertexesIndex->get(x, y - 1)->get(1, 0) : -1;
			int downNeighborNorthVertexIndex3 = y > 0 ? vertexesIndex->get(x, y - 1)->get(3, 0) : -1;

			SeaPath* seaPath = _seaPathsMatrix->get(x, y);
			if (seaPath != nullptr && !seaPath->isAMovingSeaPath){

				//Neighbor Edges
				if (upNeighborSouthVertexIndex3 > -1 && seaPath->haveEdgeAt(SeaPath::Direction::NORTH)){
					adjacency_list[N3].push_back(neighbor(upNeighborSouthVertexIndex3, 1));
					if (isStartCell)
						adjacency_list[CV].push_back(neighbor(N3, 1));
					if (isEndCell)
						adjacency_list[N1].push_back(neighbor(CV, 1));
					total++;
				}
				if (downNeighborNorthVertexIndex1 > -1 && seaPath->haveEdgeAt(SeaPath::Direction::SOUTH)){
					adjacency_list[S1].push_back(neighbor(downNeighborNorthVertexIndex1, 1));
					if (isStartCell)
						adjacency_list[CV].push_back(neighbor(S1, 1));
					if (isEndCell)
						adjacency_list[S3].push_back(neighbor(CV, 1));
					total++;
				}
				if (leftNeighborEastVertexIndex1 > -1 && seaPath->haveEdgeAt(SeaPath::Direction::WEST)){
					adjacency_list[W1].push_back(neighbor(leftNeighborEastVertexIndex1, 1));
					if (isStartCell)
						adjacency_list[CV].push_back(neighbor(W1, 1));
					if (isEndCell)
						adjacency_list[W3].push_back(neighbor(CV, 1));
					total++;
				}
				if (rightNeighborWestVertexIndex3 > -1 && seaPath->haveEdgeAt(SeaPath::Direction::EAST)){
					adjacency_list[E3].push_back(neighbor(rightNeighborWestVertexIndex3, 1));
					if (isStartCell)
						adjacency_list[CV].push_back(neighbor(E3, 1));
					if (isEndCell)
						adjacency_list[E1].push_back(neighbor(CV, 1));
					total++;
				}

				//Inner Edges
				if (seaPath->haveEdgeAt(SeaPath::Direction::NORTH_EAST)){ /*   \   */
					adjacency_list[N1].push_back(neighbor(E3, 1));
					adjacency_list[E1].push_back(neighbor(N3, 1));
					total++;
				}
				if (seaPath->haveEdgeAt(SeaPath::Direction::EAST_SOUTH)){ /*   /   */
					adjacency_list[E1].push_back(neighbor(S1, 1));
					adjacency_list[S3].push_back(neighbor(E3, 1));
					total++;
				}
				if (seaPath->haveEdgeAt(SeaPath::Direction::SOUTH_WEST)){ /*   \   */
					adjacency_list[W3].push_back(neighbor(S1, 1));
					adjacency_list[S3].push_back(neighbor(W1, 1));
					total++;
				}
				if (seaPath->haveEdgeAt(SeaPath::Direction::WEST_NORTH)){ /*   /   */
					adjacency_list[N1].push_back(neighbor(W1, 1));
					adjacency_list[W3].push_back(neighbor(N3, 1));
					total++;
				}
				if (seaPath->haveEdgeAt(SeaPath::Direction::NORTH_SOUTH)){ /*   |   */
					adjacency_list[N1].push_back(neighbor(S1, 1));
					adjacency_list[S3].push_back(neighbor(N3, 1));
					total++;
				}
				if (seaPath->haveEdgeAt(SeaPath::Direction::WEST_EAST)){ /*  -- */
					adjacency_list[E1].push_back(neighbor(W1, 1));
					adjacency_list[W3].push_back(neighbor(E3, 1));
					total++;
				}
			}
		}
	}

	std::vector<double> min_distance;
	std::vector<int> previous;
	total++;
	DijkstraComputePaths(startVertexIndex, adjacency_list, min_distance, previous);

	std::list<int> path = DijkstraGetShortestPathTo(endVertexIndex, previous);

	std::vector<GridElement*> gridElements;
	Vec2 lastPos(-1,-1);
	for (std::list<int>::iterator it = path.begin(); it != path.end(); it++){
		Vec2 pos = vertexesLocation[*it];
		if (!lastPos.equals(pos)){
			GridElement* ge = new GridElement();
			ge->col = pos.x;
			ge->row = pos.y;
			gridElements.push_back(ge/*elementAt(pos.x, pos.y)*/);
			lastPos = pos;
		}
	}

	return gridElements;
}
