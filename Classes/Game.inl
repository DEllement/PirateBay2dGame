#ifndef __GAME_INL__
#define __GAME_INL__

#include "Game.h"

/*DPadLayer* TankzGame::getInput() const
{
	return _input;
}*/

cocos2d::Node* Game::getGraphics() const
{
	return _graphics;
}
cocos2d::Node* Game::getGUI() const
{
	return _gui;
}

#endif // __GAME_INL__