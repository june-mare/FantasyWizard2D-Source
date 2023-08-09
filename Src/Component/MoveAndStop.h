#pragma once
#ifndef MOVEANDSTOP_H_INCLUDE
#define MOVEANDSTOP_H_INCLUDE

#include "../Component.h"
#include "../GameObject.h"

class MoveAndStop :public Component
{
	/*Ž~‚Ü‚Á‚Ä‚¢‚éŠÔ‚ÌƒJƒEƒ“ƒ^[*/
	float stopCounter = 0.0f;
	/*“®‚¢‚Ä‚éŠÔ‚ÌƒJƒEƒ“ƒ^[*/
	float moveCounter = 0.0f;
public:
	/*Ž~‚Ü‚Á‚Ä‚¢‚Ä‚Ù‚µ‚¢ŽžŠÔ(•b)*/
	float stopTime = 0.0f;
	/*“®‚¢‚Ä‚¢‚Ä‚Ù‚µ‚¢ŽžŠÔ(•b)*/
	float moveTime = 0.0f;
	/*is•ûŒü‚ÆˆÚ“®‘¬“x*/
	float vx = 0, vy = 0;
	/*
	*ŠJŽnŽž‚ÍŽ~‚Ü‚é‚©“®‚¢‚Ä‚¢‚é‚©
	*@ref false Ž~‚Ü‚Á‚Ä‚¢‚é
	*@ref true  “®‚¢‚Ä‚¢‚é
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
