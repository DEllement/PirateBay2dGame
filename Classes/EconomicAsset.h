#pragma once

#include "cocos2d.h"

#include "Artemis\Artemis.h"

#include "SeaPath.h"
#include "Item.h"

USING_NS_CC;

class EconomicAsset : public artemis::Component
{
public:
	EconomicAsset();
	~EconomicAsset();

	int goldCoins;
	int jewels;
	int lifes;

	bool isDirty; //All
	bool isGoldCoinsDirty;
	bool isJewelsDirty;
	bool isLifesDirty;

	void add(EconomicAsset* economicAsset);
	void setGoldCoinsDirty(bool value);
	void setJewelsDirty(bool value);
	void setLifesDirty(bool value);
	//Vector<Tile*> bonusTile; //could all be replaced with Vector<Entity*> misc;
	//Vector<Item*> items;
};

