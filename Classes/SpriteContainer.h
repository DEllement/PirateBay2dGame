#pragma once

#include "cocos2d.h"

USING_NS_CC;

class SpriteContainer : public Sprite
{
public:
	SpriteContainer();
	~SpriteContainer();

	virtual void setOpacity(GLubyte opacity) override;

	CREATE_FUNC(SpriteContainer);
};

