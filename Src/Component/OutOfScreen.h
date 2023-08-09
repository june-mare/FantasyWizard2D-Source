#pragma once
#ifndef OUTOFSCREEN_H_INCLUDE
#define OUTOFSCREEN_H_INCLUDE

#include "../Component.h"
#include "../GameObject.h"

/*画面外と判定されたオブジェクトを削除する*/
class OutOfScreen :public Component
{
public:
	OutOfScreen(GameObjectPtr gameObject) 
		:Component(gameObject) { name = "OutOfScreen"; };
	virtual ~OutOfScreen() = default;

	virtual void Update(float deltaTime)override
	{
		const float xMin = -32;
		const float xMax = 40 * 32;
		const float yMin = -32;
		const float yMax = 25 * 32;

		if (gameObject->position.x <= xMin || gameObject->position.x >= xMax ||
			gameObject->position.y <= yMin || gameObject->position.y >= yMax)
		{
			gameObject->isDead = true;
		}
	}
};

#endif // !OUTOFSCREEN_H_INCLUDE
