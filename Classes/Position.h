#ifndef __Position_H__
#define __Position_H__

#include "Artemis\Artemis.h"

class Position : public artemis::Component
{
public:
	Position();
	Position(float x, float y);
	Position(float x, float y, float rotation);
	void addX(float x);
	void addY(float y);
	float getX() const;
	void setX(float x);
	float getY() const;
	void setY(float y);
	void setLocation(float x, float y);
	float getRotation() const;
	void setRotation(float rotation);
	void addRotation(float angle);
	float getRotationAsRadians() const;

	float worldX;
	float worldY;

private:
	float _x;
	float _y;
	float _rotation;
};

#endif // __Position_H__