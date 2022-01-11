#include "WaterRenderSystem.h"

#include "CCRippleSprite.h"

WaterRenderSystem::WaterRenderSystem()
{
	_canvas = Layer::create();
	_canvas->retain();
}


WaterRenderSystem::~WaterRenderSystem()
{
}

void WaterRenderSystem::initialize(){

	auto ripple = Sprite::create("back_10x8.png");
	_canvas->addChild(ripple);
	//CCAction * moveBackgroundForever = CCRepeatForever::create(CCSequence::createWithTwoActions(CCMoveBy::create(5.0, Vec2(100, 50)),
	//	CCMoveBy::create(5.0, Vec2(-100, -50))));
	ripple->setPosition(735,450);
	/*ripple->setPosition(-100, -20);
	ripple->runAction(moveBackgroundForever);*/
	/*SpriteFrameCache::getInstance()->addSpriteFramesWithFile("wave.plist", "wave.png");


	cocos2d::Vector<cocos2d::SpriteFrame *> frames;
	SpriteFrameCache *frameCache = SpriteFrameCache::getInstance();

	char file[100] = { 0 };

	for (int i = 0; i < 26; i++) {
		sprintf(file, "gfx_wave_anim_00%02d.png", i);
		SpriteFrame *frame = frameCache->getSpriteFrameByName(file);
		frames.pushBack(frame);
	}

	Animation *animation = Animation::createWithSpriteFrames(frames, 0.10);
	

	
	//To move in a function to create many
	Animate *animate = Animate::create(animation);
	RepeatForever *repeat = RepeatForever::create(animate);
	Sprite* sp = Sprite::create();
	sp->setPosition(Vec2(420 + 350, 150 + 350));
	sp->setContentSize(Size(100,100));
	sp->setScale(2);
	_canvas->addChild(sp);
	sp->runAction(repeat);*/

}

void WaterRenderSystem::begin(){

}
void WaterRenderSystem::end(){}
void WaterRenderSystem::removed(artemis::Entity &e){}
void WaterRenderSystem::added(artemis::Entity &e){

	//i could probably add a ripple when we drop an item in the water?
	//ripple->addRipple...

}
void WaterRenderSystem::processEntities(artemis::ImmutableBag<artemis::Entity*> &bag){


}
void WaterRenderSystem::processEntity(artemis::Entity &e){


}
bool WaterRenderSystem::checkProcessing(){
	return true;
}
