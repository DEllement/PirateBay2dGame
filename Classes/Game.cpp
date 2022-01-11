#include "Game.h"
#include "Game.inl"
#include <time.h>

#include "LevelData.h"
#include "PopUpInfos.h"
#include "PopUpFactory.h"
#include "ObjectivePopUpInfos.h"

/* @Constructor */
Game::Game() :
_world(nullptr),
_graphics(nullptr),
_gui(nullptr),
_hudRenderSystem(nullptr),
_gridRenderSystem(nullptr),
_gridSystem(nullptr),
_dragAndDropSystem(nullptr),
pauseLogic(false)
{
	
}
/* @Destructor */
Game::~Game(){
	CC_SAFE_RELEASE(_world);
}




void Game::start(){

	auto game = Game::create();

	game->initArtemisWorld();
	game->initEventHandlers();
	game->initEntities();
	Director::getInstance()->runWithScene(game->loadScene(Constants::GameScene::GAME_SESSION));
}
void Game::restartLevel(){
	
	//TODO:

}

bool Game::init(){

	do{
		CC_BREAK_IF(!Layer::init());

		_graphics = Layer::create();
		_graphics->setPosition(Point::ZERO);
		this->addChild(_graphics);

		_gui = Layer::create();
		_gui->setPosition(Point::ZERO);
		this->addChild(_gui);

		_input = Layer::create();
		_input->setPosition(Point::ZERO);

		return true;
	} while (0);
	return false;
}

void Game::initEventHandlers(){
	
	auto globalEventDispatcher = Director::getInstance()->getEventDispatcher();

	globalEventDispatcher->addCustomEventListener(Constants::GameEvent::SHOW_UI_POPUP, CC_CALLBACK_1(Game::onShowUIPopUp, this));
	globalEventDispatcher->addCustomEventListener(Constants::GameEvent::REQUEST_SCENE, CC_CALLBACK_1(Game::onRequestScene, this));
}

void Game::initEntities(){

	std::srand(time(0));

	artemis::World& world = _world->getWorld();
	
	artemis::Entity& playerData = EntityFactory::createPlayer(world);
	
	playerData.refresh(); //???
}

void Game::initArtemisWorld(/* cocos2d::PhysicsWorld* world */){

	_world = new artemis::ArtemisWorld();
	_world->setSystem(_movementSystem = new MovementSystem());
	_world->setSystem(_dragAndDropSystem = new DragAndDropSystem());
	_world->setSystem(new ShipSystem());
	_world->setSystem(_gridSystem = new GridSystem());
	_world->setSystem(_hudRenderSystem = new HudRenderSystem());
	_world->setSystem(_gridRenderSystem = new GridRenderSystem());
	_world->setSystem(_waterRenderSystem = new WaterRenderSystem());
	_world->setSystem(_enemiesSystem = new EnemiesSystem());
	_world->setSystem(_randomMovingSeaPathSystem = new RandomMovingSeaPathSystem());
	_world->setSystem(_objectiveSystem = new ObjectiveSystem());

	_world->init();

	this->addComponent(_world);


}
void Game::onEnter()
{
	Container::onEnter();

	_graphics->addChild(_waterRenderSystem->getCanvas(), (int)Constants::ZOrder::Water);
	_graphics->addChild(_hudRenderSystem->getCanvas(), (int)Constants::ZOrder::Hud);
	_graphics->addChild(_gridRenderSystem->getCanvas(), (int)Constants::ZOrder::TileBoard);
	_graphics->addChild(_dragAndDropSystem->getDragLayer(), (int)Constants::ZOrder::DragLayer);

	//this->showLevelSummaryPopUp();

	this->scheduleUpdate();
	//this->setKeyboardEnabled(true);
}

void Game::onExit()
{
	Container::onExit();
	_graphics->removeChild(_waterRenderSystem->getCanvas());
	_graphics->removeChild(_hudRenderSystem->getCanvas());
	_graphics->removeChild(_gridRenderSystem->getCanvas());
	_graphics->removeChild(_dragAndDropSystem->getDragLayer());

	this->unscheduleUpdate();
	//this->setKeyboardEnabled(false);
}

void Game::update(float delta)
{
	//	float x, y,z;
	//	_graphics->getCamera()->getEye(&x, &y, &z);
	//	_graphics->getCamera()->setEye(x, y, ++z);

	Container::update(delta);
	//Maybe here make a switch to update required systems only
	if (!pauseLogic){
		_movementSystem->process();
		_gridSystem->process();
		_enemiesSystem->process();
		//_randomMovingSeaPathSystem->process();
		_objectiveSystem->process();
	}
	_gridRenderSystem->process();
	_hudRenderSystem->process();

	//Added this over here for now...
	if (levelJustStarted){
		levelJustStarted = false;
		showLevelObjectivesPopUp();
	}

}

Scene* Game::loadScene(Constants::GameScene gameScene){

	auto scene = Scene::create();
	
	switch (gameScene){
	case Constants::GameScene::INTRO: break;
	case Constants::GameScene::MAIN_MENU: break;
	case Constants::GameScene::PUBLISHER_PAGE: break;
	case Constants::GameScene::PLAYHEAVEN: break;
	case Constants::GameScene::OPTIONS: break;
	case Constants::GameScene::SUBSCRIBE: break;
	case Constants::GameScene::MAP: break;
	case Constants::GameScene::TROPHY_ROOM: break;
	case Constants::GameScene::GAME_SESSION:{

		artemis::Entity& levelEntity = EntityFactory::createLevel(_world->getWorld());
		levelJustStarted = true;

		break;
	}
	case Constants::GameScene::CUTSCENE:
		
		
		
		
		break;
	}

	
	scene->addChild(this);
	return scene;
}

void Game::showPopup(PopUpInfos* popupInfos){

	auto director = Director::getInstance();
	Size size = director->getWinSize();
	Vector<Node*> pausedActions;
	
	//Pause logic if model
	if (popupInfos->isModal){
		pauseLogic = true;
		pausedActions = director->getActionManager()->pauseAllRunningActions();
	}

	//Create the PopUp
	PopUpViewBase* popUpView = PopUpFactory::create(popupInfos);
	popUpView->setPosition(Vec2(size.width / 2, size.height / 2));
	popUpView->getEventDispatcher()->addCustomEventListener(PopUpViewBase::CLOSE, [this, popUpView, director, pausedActions](EventCustom* eCustom)
	{
		director->getActionManager()->resumeTargets(pausedActions);
		popUpView->runAction(Sequence::createWithTwoActions(FadeOut::create(.5), CallFunc::create(popUpView, callfunc_selector(Layer::removeFromParent))));
		pauseLogic = false;
	});

	//TODO: Create a popup Stack...
	//this->popupsStack->push_back(p_layer);
	director->getEventDispatcher()->dispatchCustomEvent(Constants::GameEvent::PAUSE_LOGIC);
	popUpView->setOpacity(0);
	director->getRunningScene()->addChild(popUpView, (int)Constants::ZOrder::PopUp);
	popUpView->runAction(FadeIn::create(1));
}


void Game::showLevelObjectivesPopUp(){

	artemis::Entity& levelEntity = _world->getWorld().getTagManager()->getEntity(Constants::Tags::LEVEL);
	auto levelData = (LevelData*)levelEntity.getComponent<LevelData>();

	artemis::ImmutableBag<artemis::Entity*>* objectives = _world->getWorld().getGroupManager()->getEntities(Constants::Groups::OBJECTIVES);
	std::vector<artemis::Entity*> objectivesEntities;
	for (int i = 0; i < objectives->getCount(); i++){
		objectivesEntities.push_back(objectives->get(i));
	}

	//create Summary PopUp
	auto summaryPopUpInfo = new ObjectivePopUpInfos();
	summaryPopUpInfo->title = "Level Objectives";
	summaryPopUpInfo->objectivesEntities = &objectivesEntities;
	summaryPopUpInfo->gamePopUp = Constants::GamePopUp::GAME_OBJECTIVE;
	summaryPopUpInfo->button1_callbackFunc = [this, objectives, &levelEntity](){
		//TODO: Return to map
	};
	summaryPopUpInfo->button2_callbackFunc = [this, objectives](){

		playGetSetAnimation();

	};
	showPopup(summaryPopUpInfo);
}

void Game::showLevelSummaryPopUp(){

	artemis::Entity& levelEntity = _world->getWorld().getTagManager()->getEntity(Constants::Tags::LEVEL);
	auto levelData = (LevelData*)levelEntity.getComponent<LevelData>();

	bool levelSuccess = true;
	artemis::ImmutableBag<Entity*>* objectives = _world->getWorld().getGroupManager()->getEntities(Constants::Groups::OBJECTIVES);
	for (int i = 0; i < objectives->getCount(); i++){

		auto objectiveData = (ObjectiveData*)objectives->get(i)->getComponent<ObjectiveData>();
		if (!objectiveData->isOptional && !objectiveData->isPassed){
			levelSuccess = false;
		}
	}

	//create Summary PopUp
	auto summaryPopUpInfo = new PopUpInfos();
	summaryPopUpInfo->title = "Level " + levelSuccess ? "Completed!" : "Failed!";
	summaryPopUpInfo->content = "";
	summaryPopUpInfo->gamePopUp = levelSuccess ? Constants::GamePopUp::LEVEL_SUCCESS : Constants::GamePopUp::LEVEL_FAILED;
	summaryPopUpInfo->button1_callbackFunc = [this, objectives, &levelEntity](){

		//TODO: Return to map
		restartLevel();
	};
	summaryPopUpInfo->button2_callbackFunc = [this, objectives](){

		restartLevel();

	};
	showPopup(summaryPopUpInfo);
}

void Game::playGetSetAnimation(){

	pauseLogic = true;

	Size size = Director::getInstance()->getWinSize();

	SpriteContainer* container = SpriteContainer::create();
	Sprite* pirate = Sprite::create("pirate_character.png");
	pirate->setPositionX((-size.width / 2) + 300);

	Label * label = Label::createWithTTF("", "fonts/ariblk.ttf", 30, Size(300, 50), TextHAlignment::CENTER, TextVAlignment::CENTER);
	label->setColor(Color3B::RED);
	label->enableShadow(Color4B::BLACK, Size(2, -2), .5);

	container->addChild(pirate);
	container->addChild(label);

	Vector<FiniteTimeAction*> actions;
	actions.pushBack(MoveBy::create(1, Vec2(0, 300)));
	actions.pushBack(CCCallFunc::create([label]{
		label->setScale(1);
		label->setString("Get Set...");
		label->runAction(ScaleTo::create(1, 1.5));
	}));
	actions.pushBack(DelayTime::create(1));
	actions.pushBack(CCCallFunc::create([label]{
		label->setScale(1);
		label->setString("Ready...");
		label->runAction(ScaleTo::create(1, 1.5));
	}));
	actions.pushBack(DelayTime::create(1));
	actions.pushBack(CCCallFunc::create([label]{
		label->setScale(1);
		label->setString("GO!");
		label->runAction(Sequence::createWithTwoActions(ScaleTo::create(1, 3), FadeOut::create(0)));
	}));
	actions.pushBack(DelayTime::create(1));
	actions.pushBack(MoveBy::create(.5, Vec2(0, -1000)));
	actions.pushBack(CCCallFunc::create([this]{
		pauseLogic = false;

		//Activate all objective... or simply the objective System?
		artemis::ImmutableBag<artemis::Entity*>* objectives = _world->getWorld().getGroupManager()->getEntities(Constants::Groups::OBJECTIVES);
		for (int i = 0; i < objectives->getCount(); i++)
			((ObjectiveData*)objectives->get(i)->getComponent<ObjectiveData>())->isActive = true;

	}));
	actions.pushBack(CCCallFunc::create(container, callfunc_selector(SpriteContainer::removeFromParent)));

	Director::getInstance()->getRunningScene()->addChild(container, (int)Constants::ZOrder::PopUp);

	container->runAction(Sequence::create(actions));
	container->setPosition(Vec2(size.width/2, 0));
}


//Game EventHandlers
void Game::onShowUIPopUp(EventCustom * eventCustom){

	PopUpInfos* popupInfos = static_cast<PopUpInfos*>(eventCustom->getUserData());
	showPopup(popupInfos);

}
void Game::onRequestScene(EventCustom * eventCustom){

	auto gameScene = static_cast<Constants::GameScene>((int)eventCustom->getUserData());
	loadScene(gameScene);
}

//static functions
/*void Game::sendNotification(EventCustom* eventCustom){

}*/