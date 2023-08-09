#pragma once
#ifndef ENDSPAWN_H_INCLUDE
#define ENDSPAWN_H_INCLUDE
#include "ComponentInclude.h"
#include "../GameObject.h"
#include "../EasyAudio.h"
#include "../AudioSettings.h"
#include "../EffectList.h"
#include "../CharacterList.h"
#include "BulletList.h"

enum SpawnType
{
	enemyspawn,
	recoveryspawn,
	objectspawn,
	bulletspawn,
	weaponspawn,
};

class EndSpawn
	:public Component
{
public:
	int startEffectType = 0;
	int size = 3;
	int type = 0;
	int life = 20;
	float shotInterval = 0.5f;
	float activityTime = 3.0f;
	MovePram move;
	side vec;
	TileMapScrollPtr scrolldata;
	SpawnType spawnType = enemyspawn;
	bool enemyMagic = true;
	bool isStartUp = true;
	bool randomRecovery = false;
	bool randomWeapon = false;
	EndSpawn(GameObjectPtr gameObject)
		:Component(gameObject) {
		name = "EndSpawn";
	}
	virtual ~EndSpawn() = default;

	virtual void Start()override;
	
	virtual void OnDestroy()override;
	
};

#endif // !ENDSPAWN_H_INCLUDE