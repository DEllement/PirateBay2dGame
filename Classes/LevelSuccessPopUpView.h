#pragma once

#include "cocos2d.h"
#include "PopUpInfos.h"
#include "ObjectivePopUpInfos.h"
#include "PopUpViewBase.h"

USING_NS_CC;

class LevelSuccessPopUpView : public PopUpViewBase
{
public:
	LevelSuccessPopUpView();
	~LevelSuccessPopUpView();

	static LevelSuccessPopUpView* create(PopUpInfos* popupInfos);

protected:
	virtual bool initWithInfos(PopUpInfos* popupInfos) override;
};

