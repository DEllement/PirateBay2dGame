#include "TileRope.h"

TileRope::TileRope()
{
}


TileRope::~TileRope()
{

}

void TileRope::updateTextureToType(const int ropeType){
	this->setTexture("TileNode_" + std::to_string(ropeType) + ".png");
	this->setDirty(true);
}

TileRope* TileRope::createFromType(const int ropeType){

	TileRope * tileRope = new TileRope();
	if (tileRope && tileRope->initWithFile("TileNode_" + std::to_string(ropeType) + ".png")){
		tileRope->tileType = ropeType;
		tileRope->autorelease();
		return tileRope;
	}
	CC_SAFE_DELETE(tileRope);
	return nullptr;
}
TileRope* TileRope::create(const char* name)
{
	TileRope* spr = new TileRope();
	if (spr->initWithFile(name))
	{
		// your own stuff
		return spr;
	}
	CC_SAFE_RELEASE(spr);
	return nullptr;
}


