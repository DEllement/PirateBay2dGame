#include "Boat.h"

#include "Constants.h"

Boat::Boat()
{
}


Boat::~Boat()
{
}

bool Boat::init(){

	boatSprite = Sprite::create("boat1_83x83.png");
	selectedSprite = Sprite::create("boat1_83x83_selected.png");
	boatSprite->addChild(selectedSprite);
	this->addChild(boatSprite);

	selectedSprite->setPosition(Vec2(boatSprite->getContentSize().width / 2, boatSprite->getContentSize().height / 2));
	selectedSprite->setOpacity(0);

	floatShip = CCRepeatForever::create(CCSequence::createWithTwoActions(CCMoveBy::create(.5, Vec2(0, 5)), CCMoveBy::create(.5, Vec2(0, -5))));
	balanceShip = CCRepeatForever::create(CCSequence::createWithTwoActions(CCRotateTo::create(1, -10), CCRotateTo::create(1, 5)));

	floatShip->retain();
	balanceShip->retain();

	runFloatShipAnimation();

	return true;
}

void Boat::runShipSelectedAnimation(){

	if (!isFloating)
		runFloatShipAnimation();
	selectedSprite->runAction(RepeatForever::create(Sequence::createWithTwoActions(FadeIn::create(1), FadeOut::create(1))));
	isSelected = true;
}

void Boat::runShipDeselectedAnimation(){
	selectedSprite->stopAllActions();
	isSelected = false;
	selectedSprite->setOpacity(0);
}

void Boat::runFloatShipAnimation(){

	boatSprite->stopAllActions();
	boatSprite->setRotation(0);
	boatSprite->setPositionY(0);
	boatSprite->runAction(floatShip);
	isFloating = true;
	isBalancing = false;
}
void Boat::runBalanceShipAnimation(){

	boatSprite->stopAllActions();
	boatSprite->setRotation(0);
	boatSprite->setPositionY(0);
	boatSprite->runAction(balanceShip);
	isBalancing = true;
	isFloating = false;
}