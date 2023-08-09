#pragma once
#ifndef LEVELUPEFFECT_H_INCLUDE
#define LEVELUPEFFECT_H_INCLUDE
#include "ComponentInclude.h"
#include "../GameObject.h"
#include "../EffectList.h"

class LevelUpLogo
	:public Component
{
public:
	GameObjectPtr target;
	float upPos = 32;
	LevelUpLogo(GameObjectPtr gameObject)
		:Component(gameObject) {
		name = "leveluplogo";
	}
	virtual ~LevelUpLogo() = default;

	virtual void Start()override
	{
		target = gameObject->engine->FindWithGameObjectName("player");
		gameObject->AddSpriteTexture(
			MakeTexcoord(288, 640, 96, 32,
				IMG_OBJECTS_SIZE, IMG_OBJECTS_SIZE),
			gameObject->engine->FindWithTexture("Img/objects.tga"));

		auto anim = gameObject->AddComponent<Animator2D>();
		anim->clip.resize(7);
		for (int i = 0; i < 7; i++)
		{
			anim->AddSpriteTexture(i, MakeTexcoord(288, 640 + (i * 32), 96, 32,
				IMG_OBJECTS_SIZE, IMG_OBJECTS_SIZE)
			);
		}
		anim->killWhenEnd = true; anim->loopTime = false;

		Reload();
	}
	virtual void Update(float deltaTime)override
	{
		gameObject->position = MyMath::vec4(
			target->position.x,
			target->position.y + upPos,
			0,0);
		upPos += 32 * deltaTime;
	}
};

class LevelUpEffect
	:public Component
{
public:
	GameObjectPtr target;
	float upPos = 32;
	LevelUpEffect(GameObjectPtr gameObject)
		:Component(gameObject) {
		name = "LevelUpEffect";
	}
	virtual ~LevelUpEffect() = default;

	virtual void Start()override
	{
		target = gameObject->engine->FindWithGameObjectName("player");
		auto tcList = EffectTCList<Effect13>();
		gameObject->AddSpriteTexture(tcList[0],
			gameObject->engine->FindWithTexture("Img/objects.tga"));
		auto anim = gameObject->AddComponent<Animator2D>();
		anim->clip.resize(tcList.size());
		for (int i = 0; i < tcList.size(); i++)
		{
			anim->AddSpriteTexture(i, tcList[i],2);
		}
		anim->killWhenEnd = true; anim->loopTime = false;

		Reload();
	}
	virtual void Update(float deltaTime)override
	{
		gameObject->position = MyMath::vec4(
			target->position.x + 48,
			target->position.y + upPos,
			0, 0);
		upPos += 32 * deltaTime;
	}
};


#endif // !LEVELUPEFFECT_H_INCLUDE