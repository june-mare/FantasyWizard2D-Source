#pragma once
#ifndef SHADOW_H_INCLUDE
#define SHADOW_H_INCLUDE
#include "../Component.h"
#include "../GameObject.h"
#include "ComponentInclude.h"
#include "../Sprite.h"

class Shadow
	:public Component
{
public:
	GameObjectPtr shadow;
	
	Shadow(GameObjectPtr gameObject)noexcept
		:Component(gameObject) {
		name = "Shadow";
	}
	virtual ~Shadow() = default;

	virtual void Start()override
	{
		shadow = gameObject->engine->Create<GameObject>("shadow",
			gameObject->position.x, gameObject->position.y);
		
		shadow->AddSpriteTexture(
			MakeTexcoord(0, 0, 32, 32, IMG_OBJECTS_SIZE, IMG_OBJECTS_SIZE),
			gameObject->engine->FindWithTexture("Img/objects.tga"),0.0f,0.0f,1.5
		);
		shadow->layer = prioShadow;
	}

	virtual void Update(float deltaTime)override
	{
		shadow->position = gameObject->position;
						
	}
	virtual void OnTakeDamage(GameObject& other,
		const Damage& damage)override
	{
		if (gameObject->isDead == true)
		{
			if (shadow == nullptr)return;
			shadow->isDead = true;
			shadow.reset();
		}
	}
	virtual void OnCollision(GameObject& other)override
	{
		if (gameObject->isDead == true)
		{
			if (shadow == nullptr)return;
			shadow->isDead = true;
			shadow.reset();
		}
	}
	virtual void OnDestroy()override
	{
		if (gameObject->isDead == true)
		{
			if (shadow == nullptr)return;
			shadow->isDead = true;
			shadow.reset();
		}
	}
};

#endif