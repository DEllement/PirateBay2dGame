#include "Position.h"

// cocos2dx
#include "base/ccMacros.h"

Position::Position()
: _x(0.0f)
, _y(0.0f)
, _rotation(0.0f)
{

}

Position::Position(float x, float y)
: _x(x)
, _y(y)
, _rotation(0.0f)
{

}

Position::Position(float x, float y, float rotation)
: _x(x)
, _y(y)
, _rotation(rotation)
{

}

void Position::addX(float x)
{
	this->_x += x;
}

void Position::addY(float y)
{
	this->_y += y;
}

float Position::getX() const
{
	return this->_x;
}

void Position::setX(float x)
{
	this->_x = x;
}

float Position::getY() const
{
	return _y;
}

void Position::setY(float y)
{
	this->_y = y;
}

void Position::setLocation(float x, float y)
{
	this->_x = x;
	this->_y = y;
}

float Position::getRotation() const
{
	return _rotation;
}

void Position::setRotation(float rotation)
{
	this->_rotation = rotation;
}

void Position::addRotation(float angle)
{
	this->_rotation = int(_rotation + angle) % 360;
}

float Position::getRotationAsRadians() const
{
	return CC_DEGREES_TO_RADIANS(_rotation);
}
