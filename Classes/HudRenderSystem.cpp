#include "HudRenderSystem.h"

//visual 
#include "TileDistributer.h"
#include "SpriteContainer.h"

//components
#include "SeaPath.h"
#include "LevelData.h"
#include "ObjectiveData.h"
#include "ShipData.h"

HudRenderSystem::HudRenderSystem() : _player(nullptr), _canvas(nullptr), totalSecondObjective(0)
{
	//addComponentType<LevelData>();
	addComponentType<EconomicAsset>();

	_canvas = Layer::create();
	_canvas->retain();
}


HudRenderSystem::~HudRenderSystem()
{
	CC_SAFE_RELEASE(_canvas);
}

void HudRenderSystem::ensurePlayerEntity()
{
	if (nullptr == _player || !_player->isActive())
	{
		_player = &(world->getTagManager()->getEntity(Constants::Groups::PLAYER));
	}
}

void HudRenderSystem::initialize()
{
	_economicAssetMapper.init(*world);
	_levelDataMapper.init(*world);

	this->ensurePlayerEntity();

	//Create SeaPaths Distributer
	int availableTypes[8] = { 1, 2, 3, 4, 5, 6, 7, 8 };
	_seaPathDistributer = TileDistributer::create();
	_seaPathDistributer->setPosition(Vec2(195, 350));
	_seaPathDistributer->tileSize = CCSize(70, 70);
	_seaPathDistributer->distributeTiles(availableTypes, 8, 4);
	
	//Gold Coin

	/*auto goldCoinSprite = Sprite::create("gold_coin.png");
	goldCoinSprite->setPosition(Vec2(450, 700));*/

	_goldCoinsLabel = LabelTTF::create("0", "Arial Black", 15);
	_goldCoinsLabel->setColor(Color3B(255, 255, 255));
	_goldCoinsLabel->enableShadow(Size(2, -2), .5, .5);
	_goldCoinsLabel->setPosition(Vec2(405, 854));

	//Jewels

	auto jewelsSprite = Sprite::create("jewels.png");
	jewelsSprite->setPosition(Vec2(600, 700));

	_jewelsLabel = LabelTTF::create("0", "Arial Black", 15);
	_jewelsLabel->setColor(Color3B(255, 255, 255));
	_jewelsLabel->enableShadow(Size(2, -2), .5, .5);
	_jewelsLabel->setPosition(Vec2(600, 854));

	//Lifes
	_lifesLabel = LabelTTF::create("0", "Arial Black", 15);
	_lifesLabel->setColor(Color3B(255, 255, 255));
	_lifesLabel->enableShadow(Size(2, -2), .5, .5);
	_lifesLabel->setPosition(Vec2(930, 854));

	//CountDown Label
	_countDownTimerLabel = LabelTTF::create("0", "Arial Black", 20);
	_countDownTimerLabel->setColor(Color3B(255, 255, 255));
	_countDownTimerLabel->enableShadow(Size(2, -2), .5, .5);
	_countDownTimerLabel->setPosition(Vec2(735, 52));

	_canvas->addChild(_goldCoinsLabel);
	_canvas->addChild(_jewelsLabel);
	_canvas->addChild(_lifesLabel);
	_canvas->addChild(_seaPathDistributer);
	_canvas->addChild(_countDownTimerLabel);

	//Debug label
	_debugLabel = LabelTTF::create("0", "Arial Black", 15);
	_debugLabel->setColor(Color3B(0, 255, 0));
	_debugLabel->enableShadow(Size(2, -2), .5, .5);
	_debugLabel->setPosition(Vec2(730, 800));
	_debugLabel->setOpacity(0);
	 _canvas->addChild(_debugLabel);

	auto globalEventDispatcher = cocos2d::Director::getInstance()->getEventDispatcher();
	globalEventDispatcher->addCustomEventListener(Constants::GameEvent::DEBUG_LOG, CC_CALLBACK_1(HudRenderSystem::onDebugLog, this));
}

void HudRenderSystem::onDebugLog(EventCustom* eventCustom){

	_debugLabel->setString((char*)eventCustom->getUserData());
}

bool HudRenderSystem::checkProcessing()
{
	return true;
}
void HudRenderSystem::begin()
{
}

void HudRenderSystem::end()
{
}

void HudRenderSystem::processEntities(artemis::ImmutableBag<artemis::Entity*>& bag)
{
	this->ensurePlayerEntity();

	if (nullptr != _player)
	{
		//Retreive the economicAsset of the Player Entity
		auto economicAsset = _economicAssetMapper.get(*_player);
		if ( economicAsset->isGoldCoinsDirty){
			_goldCoinsLabel->setString(std::to_string(economicAsset->goldCoins));
			economicAsset->isGoldCoinsDirty = false;
		}
		if (economicAsset->isJewelsDirty){
			_jewelsLabel->setString(std::to_string(economicAsset->jewels));
			economicAsset->isJewelsDirty = false;
		}
		if (economicAsset->isLifesDirty){
			_lifesLabel->setString(std::to_string(economicAsset->lifes));
			economicAsset->isLifesDirty = false;
		}
		
	}

	EntityProcessingSystem::processEntities(bag);
}

void HudRenderSystem::processEntity(artemis::Entity& e){

	if(e.getComponent<ObjectiveData>() != nullptr){
		ObjectiveData* objectiveData = (ObjectiveData*)e.getComponent<ObjectiveData>();

		int sec = floorf(objectiveData->currentCountDown);
		int dec = (objectiveData->currentCountDown - sec) * 10;

		//PRIMATY OBJECTIVE
		if ( !objectiveData->isOptional){

			if (objectiveData->currentCountDown <= 0)
				_countDownTimerLabel->setString("0");
			else
				_countDownTimerLabel->setString(std::to_string(sec) + "." + std::to_string(dec));
		}
		//SECONDARY OBJECTIVE
		/*else{
		
			if (objectiveData->isActive && !objectiveData->isSkipped){
				
				auto secondObjectiveCountDownLabel = (LabelTTF*)this->getCanvas()->getChildByName("secondObjectiveCountDownLabel" + std::to_string(e.getId()));
				if (secondObjectiveCountDownLabel == nullptr){
					//Add the label (indicator)
					addSecondaryObjectiveIndicator(e);
				}
				else{
					secondObjectiveCountDownLabel->setString(objectiveData->currentCountDown <= 0 ? "0" : std::to_string(sec) + "." + std::to_string(dec));
				}
			}
		}*/

		switch (objectiveData->objectiveType){
		case ObjectiveData::ObjectiveType::REACH_THE_MARK:
			processReachTheTargetObjectiveIndicator(e);
			break;
		}

		//Reward
		if (objectiveData->justRewarded){
			objectiveData->justRewarded = false;
			EconomicAsset* objectiveEconomicAsset = (EconomicAsset*)e.getComponent<EconomicAsset>();
			EconomicAsset* playerEconomicAsset = (EconomicAsset*)_player->getComponent<EconomicAsset>();

			playerEconomicAsset->isDirty = false; //to avoid direct update of the number
			if (objectiveData->isPassed){
				//Show rewarded asset to player
				EconomicAsset* objectiveEconomicAsset = (EconomicAsset*)e.getComponent<EconomicAsset>();
				

				//Gold Coin
				auto goldSpriteContainer = SpriteContainer::create();
				auto goldCoinSprite = Sprite::create("gold_coin.png");
				goldCoinSprite->setPosition(Vec2(0, 0));

				auto goldCoinsLabel = LabelTTF::create("0", "Arial Black", 15);
				goldCoinsLabel->setColor(Color3B(255, 255, 255));
				goldCoinsLabel->enableShadow(Size(2, -2), .5, .5);
				goldCoinsLabel->setPosition(Vec2(50, 0));
				goldCoinsLabel->setString(std::to_string(objectiveEconomicAsset->goldCoins));

				goldSpriteContainer->addChild(goldCoinSprite);
				goldSpriteContainer->addChild(goldCoinsLabel);

				//Jewels
				auto jewelsSpriteContainer = SpriteContainer::create();
				auto jewelsSprite = Sprite::create("jewels.png");
				jewelsSprite->setPosition(Vec2(0, 0));

				auto jewelsLabel = LabelTTF::create("0", "Arial Black", 15);
				jewelsLabel->setColor(Color3B(255, 255, 255));
				jewelsLabel->enableShadow(Size(2, -2), .5, .5);
				jewelsLabel->setPosition(Vec2(50, 0));
				jewelsLabel->setString(std::to_string(objectiveEconomicAsset->jewels));

				jewelsSpriteContainer->addChild(jewelsSprite);
				jewelsSpriteContainer->addChild(jewelsLabel);
				//Need to fix the positioning...
				if (objectiveData->rewardWorldPos != cocos2d::Vec2::ZERO){
					goldSpriteContainer->setPosition(objectiveData->rewardWorldPos.x, objectiveData->rewardWorldPos.y); // +30 = tileSize/2, 100 = tileBoard offset...
					jewelsSpriteContainer->setPosition(objectiveData->rewardWorldPos.x, objectiveData->rewardWorldPos.y);
				}
				executeAddAssetAnimation(goldSpriteContainer, _goldCoinsLabel, 0, CallFunc::create(CC_CALLBACK_0(HudRenderSystem::setGoldCoinsDirty, this)));
				executeAddAssetAnimation(jewelsSpriteContainer, _jewelsLabel, 1, CallFunc::create(CC_CALLBACK_0(HudRenderSystem::setJewelsDirty, this)));

			}
			if (objectiveData->isFailed){




			}
		}
	}
}

void HudRenderSystem::added(artemis::Entity &e){

	this->ensurePlayerEntity();

	auto objectiveData = (ObjectiveData*)e.getComponent<ObjectiveData>();
	if (objectiveData != nullptr){
		switch (objectiveData->objectiveType){
		case ObjectiveData::ObjectiveType::REACH_THE_MARK:
			addReachTheTargetObjectiveIndicator(e);
			break;
		}
	}
}
void HudRenderSystem::removed(artemis::Entity &e)
{
	//_canvas->removeChildByTag(e.getId());
	//_barDictionary->removeObjectForKey(e.getId());

	EconomicAsset* playerEconomicAsset = (EconomicAsset*)_player->getComponent<EconomicAsset>();
	ObjectiveData* objectiveData = (ObjectiveData*)e.getComponent<ObjectiveData>();
	ShipData* shipData = (ShipData*)e.getComponent<ShipData>();

	//Handle On Ship Get Destroyed
	if (shipData != nullptr){
		if (shipData->toBeRemoved){
		
			EconomicAsset* playerEconomicAsset = (EconomicAsset*)_player->getComponent<EconomicAsset>();
			playerEconomicAsset->lifes--;
			playerEconomicAsset->setLifesDirty(true);
		}
	}

}

void HudRenderSystem::executeAddAssetAnimation(SpriteContainer* spriteToAnimate, Node* targetNode, float delay, CallFunc* callback){

	spriteToAnimate->setScale(.6);
	spriteToAnimate->setOpacity(0);

	this->_canvas->addChild(spriteToAnimate);

	Vector<FiniteTimeAction*> actions;
	if (delay > 0)
		actions.pushBack(DelayTime::create(delay));
	actions.pushBack(FadeTo::create(.5, 255));
	actions.pushBack(ScaleTo::create(.5, 1.5));
	actions.pushBack(ScaleTo::create(.5, .6));
	actions.pushBack(MoveTo::create(.5, targetNode->getPosition()));
	actions.pushBack(FadeTo::create(.5, 0));
	actions.pushBack(CallFunc::create(spriteToAnimate, callfunc_selector(Node::removeFromParent)));
	actions.pushBack(callback);

	spriteToAnimate->runAction(Sequence::create(actions));
}

void HudRenderSystem::setGoldCoinsDirty(){
	EconomicAsset* playerEconomicAsset = (EconomicAsset*)_player->getComponent<EconomicAsset>();
	playerEconomicAsset->isGoldCoinsDirty = true;
}
void HudRenderSystem::setJewelsDirty(){
	EconomicAsset* playerEconomicAsset = (EconomicAsset*)_player->getComponent<EconomicAsset>();
	playerEconomicAsset->isJewelsDirty = true;
}


void HudRenderSystem::addSecondaryObjectiveIndicator(artemis::Entity& entity){

	auto objectiveData = (ObjectiveData*)entity.getComponent<ObjectiveData>();
	int sec = floorf(objectiveData->currentCountDown);
	int dec = (objectiveData->currentCountDown - sec) * 10;

	//Secondary CountDown Label
	auto secCountDownTimerLabel = LabelTTF::create("0", "Arial Black", 20);
	secCountDownTimerLabel->setColor(Color3B(255, 255, 255));
	secCountDownTimerLabel->enableShadow(Size(2, -2), .5, .5);
	secCountDownTimerLabel->setPosition(Vec2(300, 200+ (this->totalSecondObjective*100) ));
	secCountDownTimerLabel->setName("secondObjectiveCountDownLabel" + std::to_string(entity.getId()));
	secCountDownTimerLabel->setString(std::to_string(sec) + "." + std::to_string(dec));

	this->getCanvas()->addChild(secCountDownTimerLabel);
	this->totalSecondObjective++;
}

void HudRenderSystem::addReachTheTargetObjectiveIndicator(artemis::Entity& entity){

	auto objectiveData = (ObjectiveData*)entity.getComponent<ObjectiveData>();

	auto label = LabelTTF::create("0", "Arial Black", 14, Size(100,60), TextHAlignment::CENTER);
	label->setColor(Color3B(255, 255, 255));
	label->enableShadow(Size(2, -2), .5, .5);
	label->setPosition(Vec2(100, 800 + (this->totalSecondObjective * 100)));
	label->setName("reachTheTargetLabel" + std::to_string(entity.getId()));
	label->setVisible(false);

	this->getCanvas()->addChild(label);

}
void HudRenderSystem::processReachTheTargetObjectiveIndicator(artemis::Entity& entity) {

	auto objectiveData = (ObjectiveData*)entity.getComponent<ObjectiveData>();
	auto label = (LabelTTF*)this->getCanvas()->getChildByName("reachTheTargetLabel" + std::to_string(entity.getId()));
	label->setString("Reach the Target\n" + std::to_string(objectiveData->totalTargetsReached) + " of " + std::to_string(objectiveData->isPassed ? objectiveData->totalTargets : objectiveData->minTargetRequired));
	if (objectiveData->isPassed)
		label->setColor(Color3B(0, 255, 0));
}
void HudRenderSystem::removeReachTheTargetObjectiveIndicator(artemis::Entity& entity){
	auto label = this->getCanvas()->getChildByName("reachTheTargetLabel" + std::to_string(entity.getId()));

}

void HudRenderSystem::removeSecondaryObjectiveIndicator(artemis::Entity& entity){

	auto secCountDownTimerLabel = this->getCanvas()->getChildByName("secondObjectiveCountDownLabel" + std::to_string(entity.getId()));
	secCountDownTimerLabel->runAction(Sequence::createWithTwoActions(FadeOut::create(1), CallFunc::create(secCountDownTimerLabel,callfunc_selector(Node::removeFromParent))));
	this->totalSecondObjective--;

}