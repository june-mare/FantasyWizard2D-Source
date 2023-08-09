#pragma once
#ifndef SPRITESCALEROTATION_H_INCLUDE
#define SPRITESCALEROTATION_H_INCLUDE
#include "../Component.h"
#include "../GameObject.h"

class ScaleRotation :public Component
{
	float angle;
	float scale = 1.0f;
	float sizeX = 1.0f, sizeY = 1.0f;	
	int count;
public:
	GameObjectPtr target;
	ScaleRotation(GameObjectPtr gameObject)
		:Component(gameObject) { name = "ScaleRotation"; }
	virtual void Start()override
	{
		isStart = true;
	}

	virtual void Update(float deltaTime)override
	{
		/*‰½‚ç‚©‚ÌŒvŽZ*/
		if (target) 
		{
			MyMath::vec4 vector = (gameObject->position + (-target->position)).normal();
			//vector = MyMath::Distance(target->position.x, target->position.y,
				//gameObject->position.x,gameObject->position.y);
			angle = atan2(vector.x, vector.y);
			gameObject->rotation.forward =
				MyMath::vec3(vector.x, vector.y, vector.z);
		}
		
		gameObject->spriteList.begin()->spriteList.begin()->angle = angle;
		gameObject->spriteList.begin()->spriteList.begin()->scale = scale;
	}

	virtual void Debug()override
	{
		if (ImGui::CollapsingHeader(name.c_str()))
		{
			ImGui::DragFloat("scale", &scale);
			ImGui::DragFloat("angle", &angle);
		}
	}
};

#endif // !SPRITESCALEROTATION_H_INCLUDE
