#pragma once
#ifndef ATTACHBOSS1_H_INCLUDE
#define ATTACHBOSS1_H_INCLUDE
#include "ComponentInclude.h"
#include "../GameObject.h"

#include "../CharacterList.h"
#include "../EasyAudio.h"
#include "../AudioSettings.h"

class AttachBoss1 :
	public Component
{
public:
	std::shared_ptr<Animator2D> anim;
	std::shared_ptr<BulletWays> bullet;
	std::shared_ptr<Health> health;
	side vec = side::up;
	MovePram move;
	GameObjectPtr target;
	float shotInterval = 0.7f;
	float rapidInterval = 0.0f;
	float shotTimer = 2.0f;
	float rapidTimer = 2.0f;
	int rapidCount = 0;
	int BulletOfNum = 1;
	bool isFire;
	bool isHerf = false;
	int nowSequence = 0, oldSequence = 0;
	float sequenceInterval = 5.0f;
	float sequenceTiemmr = 2.0f;
	int count = 5;
public:
	AttachBoss1(GameObjectPtr gameObject)
		:Component(gameObject) {name = "AttachBoss1";}
	virtual ~AttachBoss1() = default;

	virtual void Start()override;

	virtual void Update(float deltaTime)override;
	
};

#endif // !ATTACHBOSS1_H_INCLUDE