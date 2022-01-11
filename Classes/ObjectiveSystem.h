#pragma once

#include "artemis/artemis.h"

#include "Constants.h"
#include "ObjectiveData.h"
#include "GridElement.h"
#include "ShipData.h"

class ObjectiveSystem : public artemis::EntityProcessingSystem
{
public:
	ObjectiveSystem();
	~ObjectiveSystem();

	virtual void begin() override;
	virtual void end() override;
	virtual void initialize() override;
	virtual void added(artemis::Entity& entity) override;
	virtual void removed(artemis::Entity& entity) override;
	//ObjectiveSystem::processEntities(artemis::Bag<artemis::Entity&> entities);
	virtual void processEntity(artemis::Entity& entity) override;
	virtual bool checkProcessing() override;

private:
	artemis::Entity* _player;
	artemis::Entity* _level;
	artemis::Bag<artemis::Entity*> objectives;
	//artemis::Entity* _currentMainObjective;

	float currentTime;
	int nbObjectiveCompleted;
	int nbObjectiveSucceed;

	bool levelPassed = false;;

	artemis::ComponentMapper<ObjectiveData> _objectiveDatasMapper;
	artemis::ComponentMapper<GridElement> _gridElementsMapper;
	artemis::ComponentMapper<ShipData> _shipDatasMapper;

	void startObjective(artemis::Entity& entity);

	//Objective Logics
	void executeReachTheMarkLogic(artemis::Entity& entity);
	void executeBoatMustSurviveLogic(artemis::Entity& entity);
	void executeDefeatEnnemyLogic(artemis::Entity& entity);
	void executeItemUsageLogic(artemis::Entity& entity);
};

