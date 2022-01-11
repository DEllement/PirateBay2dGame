#pragma once

#include "cocos2d.h"

USING_NS_CC;

class Vortex : public Node
{
public:
	Vortex();
	~Vortex();

	bool init();
	CREATE_FUNC(Vortex);

	void setInnerSpriteOffetPosition(Vec2 pos);

	const Size& getContentSize() const;
private:
	Sprite* _vortexSprite;
	Action* _rotateAction;
};

