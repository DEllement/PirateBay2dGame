#pragma once

#include "cocos2d.h"
#include "Artemis/artemis.h"
#include "PopUpInfos.h"
#include "PopUpViewBase.h"
#include "ObjectivePopUpInfos.h"
#include "SpriteContainer.h"

#include <vector>

USING_NS_CC;

class ObjectivePopUpView : public PopUpViewBase{

public:
	ObjectivePopUpView();
	~ObjectivePopUpView();

	static ObjectivePopUpView* create(PopUpInfos* popupInfos);

protected:
	virtual bool initWithInfos(PopUpInfos* popupInfos) override;

private:
	SpriteContainer* createObjectiveRowAt(artemis::Entity* entity, Vec2 pos);

};