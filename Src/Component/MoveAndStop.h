#pragma once
#ifndef MOVEANDSTOP_H_INCLUDE
#define MOVEANDSTOP_H_INCLUDE

#include "../Component.h"
#include "../GameObject.h"

class MoveAndStop :public Component
{
	/*止まっている間のカウンター*/
	float stopCounter = 0.0f;
	/*動いてる間のカウンター*/
	float moveCounter = 0.0f;
public:
	/*止まっていてほしい時間(秒)*/
	float stopTime = 0.0f;
	/*動いていてほしい時間(秒)*/
	float moveTime = 0.0f;
	/*進行方向と移動速度*/
	float vx = 0, vy = 0;
	/*
	*開始時は止まるか動いているか
	*@ref false 止まっている
	*@ref true  動いている
	*/
	bool stertOrStop = false;
	MoveAndStop(GameObjectPtr gameObject)
		:Component(gameObject) { name = "MoveAndStop"; }
	virtual ~MoveAndStop() = default;
	virtual void Start()override
	{
		if (stertOrStop)
		{
			moveCounter = moveTime;
		}
		else
		{
			stopCounter = stopTime;
		}
		this->isStart = true;
	}
	virtual void Update(float deltaTime) override
	{
		if (!this->isStart)Start();
		
		if (stopCounter <= 0) {
			if (moveCounter > 0) {
				gameObject->position.x += vx * deltaTime;
				gameObject->position.y += vy * deltaTime;
				moveCounter -= deltaTime;
			}
			else
			{
				stopCounter = stopTime;
			}
		}
		else
		{
			stopCounter -= deltaTime;
			if (stopCounter <= 0)
			{
				moveCounter = moveTime;
			}
		}
	}
};

#endif // !MOVEANDSTOP_H_INCLUDE
