#pragma once

#include <string>
#include <functional>

#include "Constants.h"

class PopUpInfos
{
public:
	PopUpInfos();
	~PopUpInfos();

	std::string title;
	std::string content;

	std::string button1Label;
	std::string button2Label;

	Constants::GamePopUp gamePopUp;

	std::function<void()> button1_callbackFunc;
	std::function<void()> button2_callbackFunc;
	
	bool isModal = true;
};

