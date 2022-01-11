#include "EconomicAsset.h"


EconomicAsset::EconomicAsset() :goldCoins(0), jewels(0), lifes(0),
isDirty(false),
isGoldCoinsDirty(false),
isJewelsDirty(false),
isLifesDirty(false)
{
}


EconomicAsset::~EconomicAsset()
{
}

void EconomicAsset::add(EconomicAsset* economicAsset){

	this->goldCoins += economicAsset->goldCoins;
	this->jewels += economicAsset->jewels;
	this->lifes += economicAsset->lifes;
	this->isDirty = true;
}

void EconomicAsset::setGoldCoinsDirty(bool value){
	isGoldCoinsDirty = value;
}
void EconomicAsset::setJewelsDirty(bool value){
	isJewelsDirty = value;
}
void EconomicAsset::setLifesDirty(bool value){
	isLifesDirty = value;
}