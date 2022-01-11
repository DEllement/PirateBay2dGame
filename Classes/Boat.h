#pragma once
#include "cocos2d.h"

#include "Artemis/Artemis.h"

USING_NS_CC;

class Boat : public Node
{
public:
	Boat();
	~Boat();

	bool init();

	CREATE_FUNC(Boat);

	Sprite* boatSprite;
	Sprite* selectedSprite;

	bool isFloating;
	bool isBalancing;
	bool isSelected;

	void runShipSelectedAnimation();
	void runShipDeselectedAnimation();
	void runFloatShipAnimation();
	void runBalanceShipAnimation();

private: 
	
	Action* floatShip;
	Action* balanceShip;
};

