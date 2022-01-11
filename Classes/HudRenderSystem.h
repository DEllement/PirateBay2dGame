#pragma once

#include "cocos2d.h"

#include "Artemis\Artemis.h"

#include "Constants.h"

#include "TileDistributer.h"
#include "Position.h"
#include "EconomicAsset.h"
#include "LevelData.h"
#include "SpriteContainer.h"

class HudRenderSystem : public artemis::EntityProcessingSystem
{
public:
	HudRenderSystem();
	~HudRenderSystem();

	void ensurePlayerEntity();

	virtual void initialize() override;
	virtual void processEntities(artemis::ImmutableBag<artemis::Entity*>& bag) override;
	virtual void processEntity(artemis::Entity& entity) override;
	virtual bool checkProcessing() override;

	virtual void begin() override;
	virtual void end() override;

	virtual void removed(artemis::Entity &e) override;
	virtual void added(artemis::Entity &e) override;
	cocos2d::Node*			_canvas;
	cocos2d::Node* getCanvas() const{
		return _canvas;
	}

	void setGoldCoinsDirty();
	void setJewelsDirty();

	void onDebugLog(EventCustom* eventCustom);

protected:
	
	int totalSecondObjective;

	artemis::Entity*		_player;
	
	//CountDownTimer*			_countDownTimer;
	TileDistributer*		_seaPathDistributer;
	//BonusTileDistributer*	_bonusTileDistributer;

	LabelTTF* _goldCoinsLabel;
	LabelTTF* _jewelsLabel;
	LabelTTF* _debugLabel;
	LabelTTF* _countDownTimerLabel;
	LabelTTF* _lifesLabel;
	
	artemis::ComponentMapper<EconomicAsset> _economicAssetMapper;
	artemis::ComponentMapper<LevelData> _levelDataMapper;

	void executeAddAssetAnimation(SpriteContainer* spriteToAnimate, Node* targetNode, float delay, CallFunc* callback);

	void addSecondaryObjectiveIndicator(artemis::Entity& entity);
	void removeSecondaryObjectiveIndicator(artemis::Entity& entity);
	void addReachTheTargetObjectiveIndicator(artemis::Entity& entity);
	void processReachTheTargetObjectiveIndicator(artemis::Entity& entity);
	void removeReachTheTargetObjectiveIndicator(artemis::Entity& entity);
};

