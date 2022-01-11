#pragma once

class GameObject
{
public:
	GameObject();
	~GameObject();

	bool canMove;
	float x;
	float y;
	float z;

	char* id;
	char* name;

};

