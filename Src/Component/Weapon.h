#pragma once
#ifndef WEAPON_H_INCLUDE
#define WEAPON_H_INCLUDE
#include "../GameObject.h"
#include "../Component.h"
#include "ComponentInclude.h"
#include "../AudioSettings.h"

class Weapon :
	public Component
{
public:
	int size = 1;
	int type = 0;

	Weapon(GameObjectPtr gameObject)
		:Component(gameObject) {
		name = "Weapon";
		type = rand() % 3;
	}
	virtual ~Weapon() = default;

	virtual void Start()override
	{
		switch (type)
		{
		case 2:
			type = 5;
			break;
		}
		const Texcoord tcList[] =
		{
			MakeTexcoord(506,0,32,132,IMG_OBJECTS_SIZE,IMG_OBJECTS_SIZE),
			MakeTexcoord(538,0,32,132,IMG_OBJECTS_SIZE,IMG_OBJECTS_SIZE),
			MakeTexcoord(570,0,32,132,IMG_OBJECTS_SIZE,IMG_OBJECTS_SIZE),
			MakeTexcoord(602,0,32,132,IMG_OBJECTS_SIZE,IMG_OBJECTS_SIZE),
			MakeTexcoord(634,0,32,132,IMG_OBJECTS_SIZE,IMG_OBJECTS_SIZE),
			MakeTexcoord(666,0,32,132,IMG_OBJECTS_SIZE,IMG_OBJECTS_SIZE),
			MakeTexcoord(698,0,32,132,IMG_OBJECTS_SIZE,IMG_OBJECTS_SIZE),
			MakeTexcoord(730,0,32,132,IMG_OBJECTS_SIZE,IMG_OBJECTS_SIZE),
		};

		gameObject->AddSpriteTexture(tcList[type],
			gameObject->engine->FindWithTexture("Img/Effects.tga"), 0, 24, size);
		gameObject->layer = prioObject1;
		auto collision = gameObject->AddComponent<Box2DCollision>();
		collision->AddCollision({ -16.0f,-16.0f,16.0f,40.0f });
		gameObject->AddComponent<Shadow>();

		Reload();
	}

	virtual void OnCollision(GameObject& other)override
	{
		if (other.name == "player")
		{
			auto bullet = other.GetConponent<BulletWays>();
			bullet->type = type;
			bullet->exp += 0.4;
			if (bullet->exp >= 1)
			{
				bullet->level++;
				bullet->LevelCalc();
				Audio::PlayOneShot(AudioPlayer::SE::gageUp);
				bullet->exp -= 1;
			}
			this->gameObject->isDead = true;
		}
	}

};
#endif // !WEAPON_H_INCLUDE