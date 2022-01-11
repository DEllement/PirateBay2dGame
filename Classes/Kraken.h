#pragma once

#include "cocos2d.h"
#include  "artemis/Artemis.h"

USING_NS_CC;

class Kraken : public Sprite, public artemis::Component
{
public:
	Kraken();
	~Kraken();

	static Kraken* create();
};

