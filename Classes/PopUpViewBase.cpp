#include "PopUpViewBase.h"

const char* PopUpViewBase::CLOSE = "CLOSE";

PopUpViewBase::PopUpViewBase()
{
}


PopUpViewBase::~PopUpViewBase()
{
}

bool PopUpViewBase::initWithInfos(PopUpInfos* popupInfos){
	Layer::init();

	_popupInfos = popupInfos;

	_background = Sprite::create();

	_titleLabel = Label::createWithTTF(popupInfos->title, "fonts/MorrisRoman-Black.ttf", 60, Size(400, 35), TextHAlignment::RIGHT);
	_titleLabel->setColor(Color3B(255, 255, 255));
	_titleLabel->enableShadow(Color4B::BLACK, Size(2, -2), .5);

	_contentLabel = Label::createWithTTF(popupInfos->content, "fonts/ariblk.ttf", 16, Size(325, 75), TextHAlignment::LEFT, TextVAlignment::CENTER);
	_contentLabel->setColor(Color3B::BLACK);

	this->addChild(_background);
	this->addChild(_titleLabel);
	this->addChild(_contentLabel);

	

	return true;
}

void PopUpViewBase::onCancelButtonClick(CCObject* sender){

	if (_popupInfos->button1_callbackFunc)
		_popupInfos->button1_callbackFunc();

	this->getEventDispatcher()->dispatchCustomEvent(CLOSE, this);
}

void PopUpViewBase::onOkButtonClick(CCObject* sender){
	if (_popupInfos->button2_callbackFunc)
		_popupInfos->button2_callbackFunc();

	this->getEventDispatcher()->dispatchCustomEvent(CLOSE, this);
}

void PopUpViewBase::setOpacity(GLubyte opacity){

	Layer::setOpacity(opacity);

	for (Node* node : this->getChildren()){
		node->setOpacity(opacity);
	}
}


PopUpViewBase* PopUpViewBase::create(PopUpInfos* popupInfos){

	PopUpViewBase* view = new PopUpViewBase();
	if (view && view->initWithInfos(popupInfos))
	{
		view->autorelease();
		return view;
	}
	CC_SAFE_RELEASE(view);
	return nullptr;
}
