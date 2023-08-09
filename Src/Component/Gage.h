#pragma once
#ifndef GAGE_H_INCLUDE
#define GAGE_H_INCLUDE
#include "../GameObject.h"
#include "../Sprite.h"
#include "../Component.h"

class Gage :public Component
{
public:
	float sizex = 0;
	float baseData = 0;
	float startPos;
	float stopPos = 0;
	float movePram = 0;
	Gage(GameObjectPtr gameObject)
		:Component(gameObject) {
		name = "gage";
	}
	virtual ~Gage() = default;

	virtual void Start()override
	{
		sizex = gameObject->engine->FindWithTexture("Img/expGage.tga")->width;
		startPos = gameObject->position.x;
	}

	virtual void Update(float deltaTime)override
	{
		stopPos = (baseData * sizex);
		if (stopPos + startPos != gameObject->position.x)
		{
			gameObject->position.x +=
				(baseData * sizex) * deltaTime;

			if (stopPos + startPos + 2 > gameObject->position.x
				&& stopPos + startPos - 2 < gameObject->position.x )
				gameObject->position.x = stopPos + startPos;
		}
		if ((0.99 * sizex) + startPos < gameObject->position.x)
		{
			gameObject->position.x = startPos;
		}		
	}
};

#endif // !GAGE_H_INCLUDE