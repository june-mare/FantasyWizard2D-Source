#pragma once
#ifndef MOVE_H_INCLUDE
#define MOVE_H_INCLUDE
#include "../Component.h"
#include "../GameObject.h"

class Move :public Component
{
public:
	float vx = 0;
	float vy = 0;
	float rad = 0;
	int type = 0;
	Move(GameObjectPtr gameObject)
		:Component(gameObject) {
		name = "Move";
	};
	virtual ~Move() = default;
	virtual void Update(float deltaTime) override
	{
		switch (type)
		{
		case 0:
			gameObject->position.x += vx * deltaTime;
			gameObject->position.y += vy * deltaTime;
			break;
		case 1:
			gameObject->position.y += (vy * deltaTime);
			gameObject->position.x += sin(rad) * (vx * deltaTime);
		}
	}
};

#endif // !MOVE_H_INCLUDE