#pragma once
#ifndef MOVEANDSTOP_H_INCLUDE
#define MOVEANDSTOP_H_INCLUDE

#include "../Component.h"
#include "../GameObject.h"

class MoveAndStop :public Component
{
	/*�~�܂��Ă���Ԃ̃J�E���^�[*/
	float stopCounter = 0.0f;
	/*�����Ă�Ԃ̃J�E���^�[*/
	float moveCounter = 0.0f;
public:
	/*�~�܂��Ă��Ăق�������(�b)*/
	float stopTime = 0.0f;
	/*�����Ă��Ăق�������(�b)*/
	float moveTime = 0.0f;
	/*�i�s�����ƈړ����x*/
	float vx = 0, vy = 0;
	/*
	*�J�n���͎~�܂邩�����Ă��邩
	*@ref false �~�܂��Ă���
	*@ref true  �����Ă���
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
