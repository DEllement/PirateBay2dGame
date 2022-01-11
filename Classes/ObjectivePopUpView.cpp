#include "ObjectivePopUpView.h"
#include "SpriteContainer.h"
#include "ObjectiveData.h"

ObjectivePopUpView::ObjectivePopUpView(){


}
ObjectivePopUpView::~ObjectivePopUpView(){


}

bool ObjectivePopUpView::initWithInfos(PopUpInfos* popupInfos){
	PopUpViewBase::initWithInfos(popupInfos);

	auto objectivePinfos = (ObjectivePopUpInfos*)popupInfos;

	_background->setTexture("pop_mainback.png");
	_btnCancelSprite = Sprite::create("pop_back_button.png");
	_btnOkSprite = Sprite::create();
	_titleLabel->setAlignment(TextHAlignment::CENTER);
	_titleLabel->setPosition(Vec2(0,100));
	_titleLabel->setVisible(false);

	CCMenuItemSprite *cancelButton = CCMenuItemSprite::create(_btnCancelSprite, _btnCancelSprite, _btnCancelSprite, this, menu_selector(PopUpViewBase::onCancelButtonClick));
	CCMenuItemSprite *okButton = CCMenuItemSprite::create(_btnOkSprite, _btnOkSprite, _btnOkSprite, this, menu_selector(PopUpViewBase::onOkButtonClick));

	cancelButton->setPosition(Vec2( 425 , 260 ));
	okButton->setPosition(Vec2( 0 , -202 ));
	okButton->setContentSize(Size(171, 83));

	CCMenu *menu = CCMenu::create(cancelButton, okButton, NULL);
	menu->setPosition(0, 0);
	this->addChild(menu);

	SpriteContainer* objectiveContainer = SpriteContainer::create();
	//Add the objectives
	for (int i = 0; i < objectivePinfos->objectivesEntities->size(); i++)
		objectiveContainer->addChild(createObjectiveRowAt(objectivePinfos->objectivesEntities->at(i), Vec2(0, i*50)));

	objectiveContainer->setPosition(Vec2(-300,0));
	this->addChild(objectiveContainer);

	return true;
}

ObjectivePopUpView* ObjectivePopUpView::create(PopUpInfos* popupInfos){

	ObjectivePopUpView* view = new ObjectivePopUpView();
	if (view && view->initWithInfos(popupInfos))
	{
		view->autorelease();
		return view;
	}
	CC_SAFE_RELEASE(view);
	return nullptr;
}

SpriteContainer* ObjectivePopUpView::createObjectiveRowAt(artemis::Entity* entity, Vec2 pos){

	auto objectiveData = (ObjectiveData*)entity->getComponent<ObjectiveData>();
	
	//Objective Star
	Sprite* pop_star = Sprite::create("pop_star.png");

	//Objective Label
	std::string str = "";
	switch (objectiveData->objectiveType){
	case ObjectiveData::ObjectiveType::REACH_THE_MARK:
		if (objectiveData->minTargetRequired < objectiveData->totalTargets)
			str.append("Reach " + std::to_string(objectiveData->minTargetRequired) + " out of " + std::to_string(objectiveData->totalTargets) + " targets.");
		else if (objectiveData->minTargetRequired == objectiveData->totalTargets)
			str.append("Reach all " + std::to_string(objectiveData->totalTargets) + " targets.");
		break;
	}
	Label * label = Label::createWithTTF(str, "fonts/ariblk.ttf", 25, Size(520, 50), TextHAlignment::LEFT, TextVAlignment::CENTER);
	label->setColor(Color3B::WHITE);
	label->enableShadow(Color4B::BLACK, Size(2, -2), .5);

	pop_star->setPosition(Vec2(0,0));
	label->setPosition(Vec2(300,0));

	SpriteContainer* container = SpriteContainer::create();
	container->addChild(pop_star);
	container->addChild(label);
	container->setPosition(pos);
	return container;
}

