#pragma once
#ifndef WOLLOBJECT_H_INCLUDE
#define WOLLOBJECT_H_INCLUDE
#include "ComponentInclude.h"
#include "../GameObject.h"
#include "../EasyAudio.h"
#include "../AudioSettings.h"
class WollObject :
	public Component
{
public:
	int life = 100;
	WollObject(GameObjectPtr gameObject) 
		:Component(gameObject) {
		name = "WollObject";
	}

	virtual void Start()override
	{
		auto tc = MakeTexcoord(512, 1183, 32, 96, 1536,1536);
		gameObject->AddSpriteTexture(tc, gameObject->engine->FindWithTexture("Img/mapObject.tga"), 0, 0, 1);
		gameObject->layer = prioObject2;
		auto helth = gameObject->AddComponent<Health>();
		helth->health = life;
		auto effect = gameObject->AddComponent<EndEffect>();
		effect->size = 2; effect->count = 5;
		auto audio = gameObject->AddComponent<AudioEvent>();
		audio->files.onTakeDamage = AudioPlayer::SE::enemyExplosionS;
		audio->files.onDestroy = AudioPlayer::SE::enemyExplosionL;
		auto collision = gameObject->AddComponent<Box2DCollision>();
		collision->AddCollision({ -24.0f,-48.0f,24.0f,42.0f });
		Reload();
	}
};


#endif // !WOLLOBJECT_H_INCLUDE