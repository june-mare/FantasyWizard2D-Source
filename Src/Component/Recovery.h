#pragma once
#ifndef RECOVERY_H_INCLUDE
#define RECOVERY_H_INCLUDE
#include "../GameObject.h"
#include "../Component.h"
#include "ComponentInclude.h"
#include "../AudioSettings.h"

class Recovery :
	public Component
{
public:
	int size = 0;
	Recovery(GameObjectPtr gameObject)
		:Component(gameObject) {
		name = "Recovery";
		size = rand() % 3 + 1;
	}
	virtual ~Recovery() = default;

	virtual void Start()override
	{
		auto tc = MakeTexcoord(577, 25, 22, 22,IMG_OBJECTS_SIZE, IMG_OBJECTS_SIZE);
		gameObject->AddSpriteTexture(tc, gameObject->engine->FindWithTexture("Img/objects.tga"),0,0,size - 0.5);
		gameObject->layer = prioObject1;
		auto collision = gameObject->AddComponent<Box2DCollision>();
		collision->AddCollision({ -16.0f,-16.0f,16.0f,16.0f });	
		gameObject->AddComponent<Shadow>();
		
		Reload();
	}

	virtual void OnCollision(GameObject& other)override
	{
		if (other.name == "player")
		{
			auto health = other.GetConponent<Health>();
			health->health += size;
			this->gameObject->isDead = true;
			Audio::PlayOneShot(AudioPlayer::SE::cure);
		}
	}

};


#endif // !RECOVERY_H_INCLUDE