#include "LevelSuccessPopUpView.h"
#include "ObjectivePopUpInfos.h"

LevelSuccessPopUpView::LevelSuccessPopUpView(){


}
LevelSuccessPopUpView::~LevelSuccessPopUpView(){


}

bool LevelSuccessPopUpView::initWithInfos(PopUpInfos* popupInfos){
	PopUpViewBase::initWithInfos(popupInfos);

	auto objectivePopUpInfos = (ObjectivePopUpInfos*)popupInfos;

	_background->setTexture("pop_mainback.png");
	_btnCancelSprite->setTexture("pop_back_button.png");

	return true;
}

LevelSuccessPopUpView* LevelSuccessPopUpView::create(PopUpInfos* popupInfos){

	LevelSuccessPopUpView* view = new LevelSuccessPopUpView();
	if (view && view->initWithInfos(popupInfos))
	{
		view->autorelease();
		return view;
	}
	CC_SAFE_RELEASE(view);
	return nullptr;
}



