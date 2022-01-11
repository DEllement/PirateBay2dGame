#pragma once

#include <vector>

#include "artemis/Artemis.h"
#include "cocos2d.h"

#include "Constants.h"

class ObjectiveData : public artemis::Component
{
public:
	enum ObjectiveType{
		REACH_THE_MARK,
		ITEM_USAGE,
		BOATS_MUST_SURVIVE,
		DEFEAT_ENNEMY
	};
public:
	ObjectiveData();
	~ObjectiveData();

	float startTime;
	float limitTime;
	float currentCountDown;

	//Type
	ObjectiveType objectiveType;
	bool isMain;
	bool isOptional = false;

	//Objective Status
	bool justStarted = false;
	bool isActive = false;
	bool isStarted = false;
	bool isCompleted = false;
	bool isPassed = false;
	bool isFailed = false;
	bool isSkipped = false;
	bool isRewarded = false;
	bool justRewarded = false;

	//REACH_THE_MARK properties
	artemis::Entity* currentTargetMark = nullptr;
	std::vector<cocos2d::Vec2> targets;
	char shipId; //Letter Id
	int shipEntityId; //Artemis entity Id
	int minTargetRequired = 0;
	bool simultaneousTarget = false;
	int currentTargetIndex = 0;
	int totalTargets = 0;
	int totalTargetsReached = 0;


	//ITEM_USAGE properties
	Constants::ItemType itemType;
	bool mustUseItem;
	bool cantUseItem;
	int minItemUsageTimeRequired;
	int maxItemUsageLimit;
	
	//BOATS_MUST_SURVIVE
	bool allBoatRequired;
	int minBoatAlive;

	//DEFEAT_ENNEMY
	Constants::EnemyType enemyType;
	bool killThemAll;
	int nbOfEnnemiesToKill;

	//Reward
	cocos2d::Vec2 rewardWorldPos = cocos2d::Vec2::ZERO; //could be move in a separate component
};

