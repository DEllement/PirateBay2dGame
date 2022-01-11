#include "ObjectiveSystem.h"

#include "PlayerData.h"
#include "LevelData.h"
#include "EconomicAsset.h"
#include "ObjectiveData.h"
#include "EntityFactory.h"
#include "PopupInfos.h"

ObjectiveSystem::ObjectiveSystem() :_level(nullptr), _player(nullptr), nbObjectiveSucceed(0), nbObjectiveCompleted(0), currentTime(0)
{
	addComponentType<ObjectiveData>();
}

ObjectiveSystem::~ObjectiveSystem()
{
}

void ObjectiveSystem::initialize(){

	_objectiveDatasMapper.init(*world);
	_gridElementsMapper.init(*world);
	_shipDatasMapper.init(*world);

}

void ObjectiveSystem::begin(){

	if (_level == nullptr && _player == nullptr){
		_level = &world->getTagManager()->getEntity(Constants::Tags::LEVEL);
		_player = &world->getTagManager()->getEntity(Constants::Tags::PLAYER);
		
		LevelData* levelData = (LevelData*)_level->getComponent<LevelData>();

		//Map objectives Entity
		artemis::ImmutableBag<artemis::Entity*>* _objectives = world->getGroupManager()->getEntities(Constants::Groups::OBJECTIVES);
		for (int i = 0; i < _objectives->getCount(); i++){
			objectives.add(_objectives->get(i));
		}

		//Create Level Objective PopUp

	}
}

void ObjectiveSystem::added(artemis::Entity& entity){

}
void ObjectiveSystem::removed(artemis::Entity& entity){

	EconomicAsset* playerEconomicAsset = (EconomicAsset*)_player->getComponent<EconomicAsset>();
	ObjectiveData* objectiveData = _objectiveDatasMapper.get(entity);

	//Reward 
	if (objectiveData->isCompleted){
	
		if (objectiveData->isPassed){
			//Add asset to player's assets
			EconomicAsset* objectiveEconomicAsset = (EconomicAsset*)entity.getComponent<EconomicAsset>();
			EconomicAsset* playerEconomicAsset = (EconomicAsset*)_player->getComponent<EconomicAsset>();
			playerEconomicAsset->add(objectiveEconomicAsset);

			//Trigger the Event that Assets has been added

		}
		if (objectiveData->isFailed){
		
		
		
		
		}
	}

	//Level Completed?
	if (nbObjectiveCompleted == objectives.getCount()){

		//Give additional reward based on objectives success vs failed
		if (nbObjectiveSucceed == objectives.getCount()){ //Perfect Level!

			EconomicAsset* perfectReward = (EconomicAsset*)_level->getComponent<EconomicAsset>();
			playerEconomicAsset->add(perfectReward);
		}
		else if (nbObjectiveSucceed > 0){ //Partialy Completed Level



		}
		else if (nbObjectiveSucceed == 0){ //Failed Level!

			//Remove a life?
			playerEconomicAsset->lifes--;
		}

		// TODO: Trigger Completion/Results/Stats Screen?
	}
	else{ //Add Next Objective...
		//objectives.get(nbObjectiveCompleted)->refresh();
	}

}

/*void ObjectiveSystem::processEntities(artemis::Bag<artemis::Entity&> entities){

}*/

void ObjectiveSystem::processEntity(artemis::Entity& entity){

	currentTime += world->getDelta();

	if (_level == nullptr)
		return;

	ObjectiveData* objectiveData = _objectiveDatasMapper.get(entity);
	if (!objectiveData->isActive || objectiveData->isCompleted)
		return;

	//Process Logic by Type
	switch (objectiveData->objectiveType){
		case ObjectiveData::ObjectiveType::REACH_THE_MARK:
			executeReachTheMarkLogic(entity);
			break;
		case ObjectiveData::ObjectiveType::BOATS_MUST_SURVIVE:
			executeBoatMustSurviveLogic(entity);
			break;
		case ObjectiveData::ObjectiveType::DEFEAT_ENNEMY:
			executeDefeatEnnemyLogic(entity);
			break;
		case ObjectiveData::ObjectiveType::ITEM_USAGE:
			executeItemUsageLogic(entity);
			break;
	}

	//Reward if Passed
	if (objectiveData->isPassed && !objectiveData->isRewarded){
		//Add asset to player's assets
		EconomicAsset* objectiveEconomicAsset = (EconomicAsset*)entity.getComponent<EconomicAsset>();
		EconomicAsset* playerEconomicAsset = (EconomicAsset*)_player->getComponent<EconomicAsset>();
		playerEconomicAsset->add(objectiveEconomicAsset);
		objectiveData->isRewarded = true;
		objectiveData->justRewarded = true;
		//TODO: Trigger the Event that Assets has been added OR 
		//just create a special entity to represent the addition on the hud (ex: EntityFactory::createEconomicAssetEntity(Vec2 gridPos, EconomicAsset* objectiveEconomicAsset);)

	}

}

void ObjectiveSystem::end(){
	
	if (levelPassed){
	
	
	}
	else{
	
	}
}

bool ObjectiveSystem::checkProcessing(){
	return true;
}


void ObjectiveSystem::startObjective(artemis::Entity& entity){
	
	ObjectiveData* objectiveData = _objectiveDatasMapper.get(entity);
	EconomicAsset* economicAsset = (EconomicAsset*)entity.getComponent<EconomicAsset>();

	auto popupInfos = new PopUpInfos();
	popupInfos->title = "Side Goal!";
	popupInfos->content = "Would you like to try to reach the mark on time? You have "+ std::to_string((int)objectiveData->limitTime) + " seconds";
	popupInfos->gamePopUp = Constants::GamePopUp::GAME_OBJECTIVE;
	popupInfos->button1_callbackFunc = [objectiveData](){
		objectiveData->isActive = true;
		objectiveData->isSkipped = true;
	};
	popupInfos->button2_callbackFunc = [objectiveData](){
		objectiveData->currentCountDown = objectiveData->limitTime;
		objectiveData->isActive = true;
		objectiveData->justStarted = true;
	};

	Director::getInstance()->getEventDispatcher()->dispatchCustomEvent(Constants::GameEvent::SHOW_UI_POPUP, popupInfos);
}

//Logics

void ObjectiveSystem::executeReachTheMarkLogic(artemis::Entity& entity){

	//Note: Maybe cache the ship?
	ImmutableBag<Entity*>* ships = world->getGroupManager()->getEntities(Constants::Groups::PLAYER_SHIP);

	auto objectiveData = _objectiveDatasMapper.get(entity);
	if ( objectiveData->justStarted)
		 objectiveData->justStarted = false; //unflag justStarted

	//start the objective
	if (!objectiveData->isStarted && !objectiveData->isCompleted && !objectiveData->isSkipped){
		objectiveData->isStarted = true;
		objectiveData->currentCountDown = objectiveData->limitTime;
		objectiveData->justStarted = true;
		objectiveData->totalTargets = objectiveData->targets.size();

		//Assign the shipEntityId
		for (int j = 0; j < ships->getCount(); j++){
			auto ship = ships->get(j);
			auto shipData = (ShipData*)ship->getComponent<ShipData>();
			if (shipData->id == objectiveData->shipId)
				objectiveData->shipEntityId = ship->getId();
		}
	}
	//create a new TargetMark Entity
	if (!objectiveData->isSkipped && objectiveData->currentTargetMark == nullptr){
		Vec2 target = objectiveData->targets.at(objectiveData->currentTargetIndex);
		objectiveData->currentTargetMark = &EntityFactory::createTargetMark(*world, target.x, target.y);
	}
	//process to detection of ship/target collision
	else if (!objectiveData->isSkipped && objectiveData->currentCountDown > 0){
		objectiveData->currentCountDown -= world->getDelta();

		//Check all ships
		for (int i = 0; i < ships->getCount(); i++){
			GridElement* shipGridElement = _gridElementsMapper.get(*ships->get(i));
			GridElement* targetGridElement = _gridElementsMapper.get(*(objectiveData->currentTargetMark));
			ShipData* shipData = _shipDatasMapper.get(*ships->get(i));

			//Ship Selection trigger TargetMark additional cue for the player
			if (shipData->id == objectiveData->shipId){
				auto targetMark = ((TargetMark*)objectiveData->currentTargetMark->getComponent<TargetMark>());
				if(shipData->isBoatJustSelected)
					targetMark->associatedShipJustSelected = true;
				if (shipData->isJustDeselected)
					targetMark->associatedShipJustDeSelected = true;
			}
			
			//check if the ship as reach the objective
			if (shipData->health > 0 && shipGridElement->col == targetGridElement->col && shipGridElement->row == targetGridElement->row){
				
				Position* targetWorldPos = (Position*)objectiveData->currentTargetMark->getComponent<Position>();

				objectiveData->totalTargetsReached++;
				objectiveData->currentTargetIndex++;
				objectiveData->rewardWorldPos = Vec2(targetWorldPos->worldX, targetWorldPos->worldY);
				objectiveData->currentTargetMark->remove();
				objectiveData->currentTargetMark = nullptr;
				
				//if the objective have meet the min requirement to be completed
				if (!objectiveData->isPassed && objectiveData->totalTargetsReached == objectiveData->minTargetRequired){
					nbObjectiveSucceed++; // Should moe that in levelData instead...
					nbObjectiveCompleted++;// Should moe that in levelData instead...
					
					objectiveData->isPassed = true;
					objectiveData->isFailed = false;
				}

				//When we reach all the target
				if (objectiveData->totalTargetsReached == objectiveData->totalTargets){
					objectiveData->isActive = false;
					objectiveData->isStarted = false;
					objectiveData->isCompleted = true;
				}

				break;
			}
		}
	}
	//time out!
	else if (objectiveData->currentCountDown <= 0){
		
		nbObjectiveCompleted++;// Should moe that in levelData instead...
		objectiveData->isActive = false;
		objectiveData->isStarted = false;
		objectiveData->isCompleted = true;
		objectiveData->isPassed = false;
		objectiveData->isFailed = objectiveData->isSkipped ? false : true;
	}
}

void ObjectiveSystem::executeBoatMustSurviveLogic(artemis::Entity& entity){

}

void ObjectiveSystem::executeDefeatEnnemyLogic(artemis::Entity& entity){

}

void ObjectiveSystem::executeItemUsageLogic(artemis::Entity& entity){



}