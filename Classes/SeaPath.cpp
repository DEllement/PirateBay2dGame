#include "SeaPath.h"


SeaPath::SeaPath() :rotation(0), toBeRemoved(false), isAMovingSeaPath(false), isDirty(false)
{
}


SeaPath::~SeaPath()
{
}

void SeaPath::setTileType(int tileType){
	this->tileType = tileType;

	int * tempEdgesMask;

	switch (tileType){
	case 1:{
		//               N  E  S  W  WN NE ES SW NS WE
		int vertex1[] = { 1, 0, 1, 0, 0, 0, 0, 0, 1, 1 };
		tempEdgesMask = vertex1;
		break;
	}
	case 2:{
		//               N  E  S  W  WN NE ES SW NS WE
		int vertex2[] = { 1, 1, 1, 1, 1, 1, 1, 1, 0, 0 };
		tempEdgesMask = vertex2;
		break;
	}
	case 3:{
		//               N  E  S  W  WN NE ES SW NS WE
		int vertex3[] = { 1, 1, 1, 1, 0, 0, 0, 0, 1, 1 };
		tempEdgesMask = vertex3;
		break;
	}
	case 4: {
		//               N  E  S  W  WN NE ES SW NS WE
		int vertex4[] = { 0, 1, 0, 1, 0, 0, 0, 0, 0, 1 };
		tempEdgesMask = vertex4;
		break;
	}
	case 5: {
		//               N  E  S  W  WN NE ES SW NS WE
		int vertex5[] = { 1, 1, 1, 0, 0, 1, 1, 0, 1, 0 };
		tempEdgesMask = vertex5;
		break;
	}
	case 6: { 
		//               N  E  S  W  WN NE ES SW NS WE
		int vertex6[] = { 1, 1, 1, 1, 0, 1, 0, 1, 0, 0 };
		tempEdgesMask = vertex6;
		break;
	}
	case 7: {
		//               N  E  S  W  WN NE ES SW NS WE
		int vertex7[] = { 1, 1, 1, 0, 0, 1, 1, 0, 0, 0 };
		tempEdgesMask = vertex7;
		break;
	}
	case 8:{
		//               N  E  S  W  WN NE ES SW NS WE
		int vertex8[] = { 0, 0, 1, 1, 0, 0, 0, 1, 0, 0 };
		tempEdgesMask = vertex8;
		break;
	}
	}

	for (int i = 0; i < 10; i++)
		edgesMask[i] = tempEdgesMask[i];

}
bool SeaPath::haveEdgeAt(Direction direction){

	int edge = 0;
	int r = rotation >= 360 ? ((int)rotation % 360) : rotation;
	int offR = r / 90;
	switch (direction){
		case Direction::NORTH: //0
		case Direction::EAST: //1
		case Direction::SOUTH: //2
		case Direction::WEST: //3

			edge = direction -offR;
			if (edge < 0)
				edge = edge+4;

			break;
		case Direction::WEST_NORTH: //4
		case Direction::NORTH_EAST: //5
		case Direction::EAST_SOUTH: //6
		case Direction::SOUTH_WEST: //7
			//A bit hacky but it work without having to implement a matrix rotation!
			
			edge = direction - offR;
			if (edge < 4)
				edge = edge+4;

			break;
		case Direction::NORTH_SOUTH:
		case Direction::WEST_EAST:
			if (r == 0 || r == 180) // The same direction
				edge = direction;
			else
				edge = direction == Direction::WEST_EAST ? Direction::NORTH_SOUTH : Direction::WEST_EAST; //Just inverse
			break;

	}
	return edgesMask[edge] == 1;
}

