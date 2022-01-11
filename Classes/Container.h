#pragma once

#include "cocos2d.h"


class Container : public cocos2d::Layer
{
public:
	Container();
	~Container();

public: // cocos2dx

	virtual void onEnter() override;
	virtual void onExit() override;

public: // interface
	//virtual DPadLayer* getInput() const = 0;
	virtual cocos2d::Node* getGraphics() const = 0;
	virtual cocos2d::Node* getGUI() const = 0;

};

