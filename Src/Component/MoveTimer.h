#pragma once
#ifndef MOVETIMER_H_INCLUDE
#define MOVETIMER_H_INCLUDE
#include "../GameObject.h"
#include "../Component.h"

class MoveTimer
	:public Component
{
	bool stopFlg = false;
	bool fadeOut = false;
	float movePoint = 0;
	float startPos = 0;
public:
	bool trriger = false;
	float interval = 3;
	float moveSpeed = 300;

	MoveTimer(GameObjectPtr gameObject)
		:Component(gameObject) {
		name = "MoveTimer";
	}
	virtual ~MoveTimer() = default;

	virtual void Start()override
	{
		startPos = gameObject->position.x;
		movePoint = startPos - 500;
	}

	virtual void CustomEvent(float deltaTime)override
	{
		if (trriger) {
			if (!stopFlg) {
				gameObject->position.x -= moveSpeed * deltaTime;
				if (gameObject->position.x < movePoint)
					stopFlg = true;
			}
			if (stopFlg)interval -= deltaTime;
			if (interval < 0)
			{
				fadeOut = true;
			}
			if (fadeOut)
			{
				gameObject->position.x += moveSpeed * deltaTime;
			}
			if (gameObject->position.x > startPos && fadeOut)
			{
				trriger = false; stopFlg = false;
				fadeOut = false; interval = 3;
			}
		}
	}
};

#endif // !MOVETIMER_H_INCLUDE