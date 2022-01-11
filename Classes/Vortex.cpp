#include "Vortex.h"


Vortex::Vortex()
{
}


Vortex::~Vortex()
{
}

bool Vortex::init(){
	_vortexSprite = Sprite::create("vortex.png");
	this->addChild(_vortexSprite);

	_rotateAction = CCRepeatForever::create(RotateBy::create(5, 360));
	_rotateAction->retain();

	_vortexSprite->runAction(_rotateAction);

	return true;
}

void Vortex::setInnerSpriteOffetPosition(Vec2 pos){
	_vortexSprite->setPosition(pos);
}

const Size& Vortex::getContentSize() const{
	return _vortexSprite->getContentSize();
}