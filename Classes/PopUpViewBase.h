#pragma once

#include "cocos2d.h"
#include "PopUpInfos.h"

USING_NS_CC;

class PopUpViewBase : public Layer
{
public:
	PopUpViewBase();
	~PopUpViewBase();

	static const char* CLOSE;
	static PopUpViewBase* create(PopUpInfos* popupInfos);

	void onCancelButtonClick(CCObject* sender);
	void onOkButtonClick(CCObject* sender);
	virtual void setOpacity(GLubyte opacity) override;

protected:
	PopUpInfos* _popupInfos;
	virtual bool initWithInfos(PopUpInfos* popupInfos);

	Sprite * _background;
	Label* _titleLabel;
	Label* _contentLabel;
	Sprite*	_btnCancelSprite;
	Sprite*	_btnOkSprite;
};

