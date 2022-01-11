#pragma once

#include "cocos2d.h"

class GridEvent :
	public cocos2d::EventCustom
{
public:
	static std::string _TYPE;
	GridEvent();

	float x;
	float y;
	float row;
	float col;
	bool isCellAvailable;
	bool isInGridBoundary;
	bool canDropSeaPath;
	bool haveFixedSeaPathAround;
	bool haveMovingSeaPathAround;
	//Node* targetedNode;
};

