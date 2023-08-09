#pragma once
#ifndef LASEROUTLINE_H_INCLUDE
#define LASEROUTLINE_H_INCLUDE
#include "../GameObject.h"
#include "../Component.h"
#include "ComponentInclude.h"
#include "BulletList.h"

class LaserOutLineSmall
	:public Component
{
public:
	side vec;	
	float activityTime = 0.8f;
	LaserOutLineSmall(GameObjectPtr gameObject)
		:Component(gameObject) {
		name = "LaserOutLineSmall";
	}
	virtual ~LaserOutLineSmall() = default;

	virtual void Start()override
	{
		gameObject->layer = prioLaser;
		std::vector<Texcoord> tcList;
		LaserRedSmall data;
		//スプライト
		{
			tcList = BulletTCList<LaserRedSmall>();

			gameObject->AddSpriteTexture(*tcList.begin(),
				gameObject->engine->FindWithTexture("Img/EffectsOutLine.tga"));
			auto anim = gameObject->AddComponent<Animator2D>();
			anim->loopTime = true;
			anim->clip.resize(1);
			if (vec == side::down) {
				for (int i = 0; i <1; i++) {
					anim->AddSpriteTexture(i, tcList[i], 2.0f, 0, -data.size.y);
				}
			}
			if (vec == side::up) {
				for (int i = 0; i < 1; i++) {
					anim->AddSpriteTexture(i, tcList[i], 2.0f, 0, data.size.y, MyMath::Radian2Degree(180));
				}
			}
			if (vec == side::right) {
				for (int i = 0; i < 1; i++) {
					anim->AddSpriteTexture(i, tcList[i], nullptr, data.size.y * 1.5,
						0, 3, 1, MyMath::Radian2Degree(-90));
				}
			}
			if (vec == side::left) {
				for (int i = 0; i <1; i++) {
					anim->AddSpriteTexture(i, tcList[i], nullptr,
						-data.size.y * 1.5, 0, 3, 1, MyMath::Radian2Degree(90));
				}
			}
		}
		Reload();
	}

	virtual void Update(float deltaTime)override
	{
		if (activityTime <= 0)gameObject->isDead = true;
		activityTime -= deltaTime;
	}
};

class LaserOutLineBig
	:public Component
{
public:
	side vec;
	float activityTime = 1.2f;
	LaserOutLineBig(GameObjectPtr gameObject)
		:Component(gameObject) {
		name = "LaserOutLineBig";
	}
	virtual ~LaserOutLineBig() = default;

	virtual void Start()override
	{
		std::vector<Texcoord> tcList;
		LaserPinkBig data;
		gameObject->layer = prioLaser;
		//スプライト
		{
			tcList = BulletTCList<LaserPinkBig>();

			gameObject->AddSpriteTexture(*tcList.begin(),
				gameObject->engine->FindWithTexture("Img/EffectsOutLine.tga"));
			auto anim = gameObject->AddComponent<Animator2D>();
			anim->loopTime = true;
			anim->clip.resize(1);
			if (vec == side::down) {
				for (int i = 0; i < 1; i++) {
					anim->AddSpriteTexture(i, tcList[i], 2.0f, 0, -data.size.y);
				}
			}
			if (vec == side::up) {
				for (int i = 0; i <1; i++) {
					anim->AddSpriteTexture(i, tcList[i], 2.0f, 0, data.size.y, MyMath::Radian2Degree(180));
				}
			}
			if (vec == side::right) {
				for (int i = 0; i <1; i++) {
					anim->AddSpriteTexture(i, tcList[i], nullptr, data.size.y * 1.5,
						0, 3, 1, MyMath::Radian2Degree(-90));
				}
			}
			if (vec == side::left) {
				for (int i = 0; i < 1; i++) {
					anim->AddSpriteTexture(i, tcList[i], nullptr,
						-data.size.y * 1.5, 0, 3, 1, MyMath::Radian2Degree(90));
				}
			}
		}
		Reload();
	}

	virtual void Update(float deltaTime)override
	{
		if (activityTime <= 0)gameObject->isDead = true;
		activityTime -= deltaTime;
	}
};

#endif // !LASEROUTLINE_H_INCLUDE
