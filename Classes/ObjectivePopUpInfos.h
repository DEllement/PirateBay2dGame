#pragma once

#include "PopUpInfos.h"

#include <vector>

class ObjectivePopUpInfos :
	public PopUpInfos
{
public:
	ObjectivePopUpInfos();
	~ObjectivePopUpInfos();

	std::vector<artemis::Entity*>* objectivesEntities;
};

