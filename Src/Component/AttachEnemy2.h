#pragma once
#ifndef ATTACHENEMY2_H_INCLUDE
#define ATTACHENEMY2_H_INCLUDE
#include "../Component.h"
#include "ComponentInclude.h"
#include "../GameObject.h"

#include "../CharacterList.h"
#include "BulletList.h"
#include "../EasyAudio.h"
#include "../AudioSettings.h"

class AttachEnemy2
	:public Component
{
public:
	std::shared_ptr<Animator2D> anim;
	std::shared_ptr<BulletWays> bullet;

	side vec = side::up;
	MovePram move;
	GameObjectPtr target;
	float shotInterval = 3.0f;
	float rapidInterval = 0.2f;
	float shotTimer = 2.0f;
	float rapidTimer = 0.1f;
	int rapidCount = 0;
	int BulletOfNum = 5;
	bool isFire;

	AttachEnemy2(GameObjectPtr gameObject)noexcept
		:Component(gameObject) {
		name = "AttachEnemy2";
	}
	virtual ~AttachEnemy2() = default;

	virtual void Start()override
	{
		//基本処理
		{
			target = gameObject->engine->FindWithGameObjectName("player");
			bullet = gameObject->AddComponent<BulletWays>();
			bullet->type = 1;
			gameObject->layer = prioEnemy;
			gameObject->tag = GameObjectTag::enemy;
			auto vx = target->position.x - gameObject->position.x;
			auto vy = target->position.y - gameObject->position.y;
			if (vx > 0)vec = side::right;
			else vec = side::left;
			if (vy > 0)vec = side::down;
			else vec = side::up;
		}

		auto tcList = CharacterTCList<Enemy2>();
		//画像処理
		{
			gameObject->AddSpriteTexture(tcList[0],
				gameObject->engine->FindWithTexture("Img/objects.tga"));
			anim = gameObject->AddComponent<Animator2D>();
			anim->clip.resize(tcList.size());
			for (int i = 0; i < tcList.size(); i++)
			{
				anim->AddSpriteTexture(i, tcList[i], 1.5);
			}
			anim->interval = 0.2f;
			anim->endCount = static_cast<int>(vec);
			anim->startCount = static_cast<int>(vec) - 3;
		}
		
		float collisionSize = (IMG_OBJECTS_SIZE * 0.4f);
		auto collition = gameObject->AddComponent<Box2DCollision>();
		collition->AddCollision({ -tcList[0].sx * collisionSize, -tcList[0].sy * collisionSize,
			tcList[0].sx * collisionSize, tcList[0].sy * collisionSize
			});

		auto helth = gameObject->AddComponent<Health>();
		helth->health = 8;

		auto explosion = gameObject->AddComponent<EndEffect>();
		explosion->count = 3;

		auto audio = gameObject->AddComponent<AudioEvent>();
		audio->files.onTakeDamage = AudioPlayer::SE::enemyExplosionS;

		gameObject->AddComponent<Shadow>();

		move.speed = 100.0f;
		move.acc = 50.0f;
		move.accIncrease = 0.5f;

		Reload();
	}

	virtual void Update(float deltaTime)override
	{
		auto vx = target->position.x - gameObject->position.x;
		auto vy = target->position.y - gameObject->position.y;
		side priv = vec;
		bool verOrHori = abs(vx) < abs(vy) ? true : false;
		if (vx > 0 && !verOrHori)
		{
			vec = side::right;
		}
		else if (vx < 0 && !verOrHori)vec = side::left;
		if (vy > 0 && verOrHori)vec = side::up;
		else if (vy < 0 && verOrHori)vec = side::down;
		if (priv != vec)move.Reset();
		move.Acceleration();
		anim->endCount = static_cast<int>(vec);
		anim->startCount = static_cast<int>(vec) - 3;
		//移動
		
		bullet->move.moveVector = move.moveVector;


		shotTimer -= deltaTime;/*キーが押されている間はタイマーを減らす*/
		if (shotTimer <= 0) {
			isFire = true;
			rapidCount = BulletOfNum;
			bullet->move.speed = 500;
			bullet->type = 1;
			bullet->count = 2;
			bullet->vec = vec;
			bullet->color = 4;
			bullet->EnemyBullet();
			bullet->AngleSet();
			shotTimer += shotInterval;/*弾の発射間隔を設定(秒)*/
		}
		if (!isFire)
		{
			return;
		}
		rapidTimer += deltaTime;
		if (rapidTimer < rapidInterval)
		{
			return;
		}
		rapidTimer -= rapidInterval;

		bullet->Spawn();

		--rapidCount;
		if (rapidCount <= 0)
		{
			isFire = false;
		}
	}

};


#endif // !ATTACHENEMY1_H_INCLUDE