#ifndef _GAME_H_
#define _GAME_H_

// artemis
#include "ArtemisWorld.h"

// cocos2dx
#include "cocos2d.h"

#include "Artemis/Artemis.h"
#include "EntityFactory.h"

#include "Container.h"
#include "Constants.h"

#include "MovementSystem.h"
#include "HudRenderSystem.h"
#include "GridRenderSystem.h"
#include "GridSystem.h"
#include "DragAndDropSystem.h"
#include "WaterRenderSystem.h"
#include "ShipSystem.h"
#include "EnemiesSystem.h"
#include "ObjectiveSystem.h"
#include "RandomMovingSeaPathSystem.h"
#include "PopUpInfos.h"

USING_NS_CC;

class Game : public Container
{
public:
	Game();
	~Game();

	static void start();
	void restartLevel();
	
public: // cocos2d-x

	bool pauseLogic;

	CREATE_FUNC(Game);
	virtual bool init() override;
	virtual void onEnter() override;
	virtual void onExit() override;
	virtual void update(float delta) override;

public: // container
	//virtual DPadLayer* getInput() const override;
	virtual cocos2d::Node* getGraphics() const override;
	virtual cocos2d::Node* getGUI() const override;

public: // artemis
	void initArtemisWorld();
	//void initEdgeBox(cocos2d::PhysicsWorld* world);
	void initEntities();
	void initEventHandlers();
	cocos2d::Scene* loadScene(Constants::GameScene gameScene);
	void showPopup(PopUpInfos* popupInfos);
	void showLevelObjectivesPopUp();
	void showLevelSummaryPopUp();
	void playGetSetAnimation();

	//Game Events Handler
	void onShowUIPopUp(EventCustom* eventCustom);
	void onRequestScene(EventCustom* eventCustom);

private:
	bool levelJustStarted;

	cocos2d::Node* _graphics; //Render
	cocos2d::Node* _gui; //Hud
	cocos2d::Node* _input; //Touch Handling Catch all Layer

	artemis::ArtemisWorld* _world;

	MovementSystem* _movementSystem;
	HudRenderSystem* _hudRenderSystem;
	GridRenderSystem* _gridRenderSystem;
	GridSystem* _gridSystem;
	EnemiesSystem* _enemiesSystem;
	DragAndDropSystem* _dragAndDropSystem;
	WaterRenderSystem* _waterRenderSystem;
	ObjectiveSystem* _objectiveSystem;
	RandomMovingSeaPathSystem* _randomMovingSeaPathSystem;
};

#endif //_GAME_H_
