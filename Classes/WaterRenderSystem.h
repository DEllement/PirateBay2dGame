#pragma once
#include "Artemis/Artemis.h"

#include "cocos2d.h"

USING_NS_CC;

class WaterRenderSystem : public artemis::EntityProcessingSystem
{
public:
	WaterRenderSystem();
	~WaterRenderSystem();

	virtual void initialize() override;
	virtual void begin() override;
	virtual void processEntities(artemis::ImmutableBag<artemis::Entity*> &bag) override;
	virtual void processEntity(artemis::Entity &e) override;
	virtual void end() override;

	virtual void removed(artemis::Entity &e) override;
	virtual void added(artemis::Entity &e) override;

	virtual bool checkProcessing() override;

	Node* getCanvas() const{
		return _canvas;
	}

private:
	Node* _canvas;
};

