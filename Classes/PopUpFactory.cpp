#include "PopUpFactory.h"

#include "ObjectivePopUpView.h"
#include "LevelSuccessPopUpView.h"
#include "LevelFailedPopUpView.h"
#include "Constants.h"

PopUpFactory::PopUpFactory()
{
}


PopUpFactory::~PopUpFactory()
{

}

PopUpViewBase* PopUpFactory::create(PopUpInfos* popupInfos){

	PopUpViewBase* popUpView = nullptr;
	switch (popupInfos->gamePopUp){
		case Constants::GamePopUp::IN_GAME_MENU:break;
		case Constants::GamePopUp::SHOP: break;
		case Constants::GamePopUp::GAME_OBJECTIVE:
			popUpView = ObjectivePopUpView::create(popupInfos);
			break;
		case Constants::GamePopUp::PAUSE: break;
		case Constants::GamePopUp::LEVEL_SUCCESS:
			popUpView = LevelSuccessPopUpView::create(popupInfos);
			break;
		case Constants::GamePopUp::LEVEL_FAILED:
			popUpView = LevelFailedPopUpView::create(popupInfos);
			break;
	}
	return popUpView;
}
