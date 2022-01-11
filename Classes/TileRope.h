#pragma once

#include "cocos2d.h"

USING_NS_CC;

class TileRope :
	public Sprite
{
public:
	TileRope();
	~TileRope();

	int tileType;

	void updateTextureToType(const int ropeType);

	static TileRope* createFromType(const int type);
	static TileRope* create(const char* name);

};

