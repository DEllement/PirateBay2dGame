#pragma once

#include "cocos2d.h"
#include "PopUpViewBase.h"
#include "PopUpInfos.h"
#include "ObjectivePopUpInfos.h"

USING_NS_CC;

class LevelFailedPopUpView : public PopUpViewBase
{
public:
	LevelFailedPopUpView();
	~LevelFailedPopUpView();

	static LevelFailedPopUpView* create(PopUpInfos* popupInfos);

protected:
	virtual bool initWithInfos(PopUpInfos* popupInfos) override;
};

