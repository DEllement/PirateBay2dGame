#pragma once

#include "PopUpInfos.h"
#include "PopUpViewBase.h"

class PopUpFactory
{
public:
	PopUpFactory();
	~PopUpFactory();

	static PopUpViewBase* create(PopUpInfos* popupInfos);
};

