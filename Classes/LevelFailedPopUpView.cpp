#include "LevelFailedPopUpView.h"

LevelFailedPopUpView::LevelFailedPopUpView(){


}
LevelFailedPopUpView::~LevelFailedPopUpView(){


}

bool LevelFailedPopUpView::initWithInfos(PopUpInfos* popupInfos){
	PopUpViewBase::initWithInfos(popupInfos);

	_background->setTexture("pop_mainback.png");
	_btnCancelSprite->setTexture("pop_back_button.png");

	return true;
}

LevelFailedPopUpView* LevelFailedPopUpView::create(PopUpInfos* popupInfos){

	LevelFailedPopUpView* view = new LevelFailedPopUpView();
	if (view && view->initWithInfos(popupInfos))
	{
		view->autorelease();
		return view;
	}
	CC_SAFE_RELEASE(view);
	return nullptr;
}



