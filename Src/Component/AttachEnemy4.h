#pragma once
#ifndef ATTACHENEMY4_H_INCLUDE
#define ATTACHENEMY4_H_INCLUDE
#include "../Component.h"
#include "ComponentInclude.h"
#include "../GameObject.h"

#include "../CharacterList.h"
#include "BulletList.h"
#include "../EasyAudio.h"
#include "../AudioSettings.h"

class AttachEnemy4
	:public Component
{
public:
	std::shared_ptr<Animator2D> anim;
	std::shared_ptr<BulletWays> bullet;
	side vec = side::up;
	MovePram move;
	GameObjectPtr target;
	float runningTimer = 2.0;
	float runningTime = 2.0;
	float interval = 1.0f;
	bool isFire = false;
	AttachEnemy4(GameObjectPtr gameObject)noexcept
		:Component(gameObject) {
		name = "AttachEnemy4";
	}
	virtual ~AttachEnemy4() = default;

	virtual void Start()override
	{
		//基本処理
		{
			target = gameObject->engine->FindWithGameObjectName("player");
			bullet = gameObject->AddComponent<BulletWays>();
			gameObject->layer = prioEnemy;
			gameObject->tag = GameObjectTag::enemy;
			auto vx = target->position.x - gameObject->position.x;
			auto vy = target->position.y - gameObject->position.y;
			if (vx > 0)vec = side::right;
			else vec = side::left;
			if (vy > 0)vec = side::down;
			else vec = side::up;
			bullet->move.moveVector = MyMath::vec3(vx, vy, 0);
		}

		auto tcList = CharacterTCList<Enemy4>();

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

		auto helth = gameObject->AddComponent<Health>();
		helth->health = 3;

		auto explosion = gameObject->AddComponent<EndEffect>();
		explosion->count = 2;
		float collisionSize = (IMG_OBJECTS_SIZE * 0.4f);
		auto collition = gameObject->AddComponent<Box2DCollision>();
		collition->AddCollision({ -tcList[0].sx * collisionSize, -tcList[0].sy * collisionSize,
			tcList[0].sx * collisionSize, tcList[0].sy * collisionSize
			});

		auto audio = gameObject->AddComponent<AudioEvent>();
		audio->files.onTakeDamage = AudioPlayer::SE::enemyExplosionS;

		auto damage = gameObject->AddComponent<DamageSource>();
		damage->interval = 2.0f;
		damage->targetName.push_back("player");
		damage->targetName.push_back("object");

		gameObject->AddComponent<Shadow>();

		move.speed = 600.0f;
		move.acc = 50.0f;
		move.accIncrease = 0.5f;

		Reload();
	}

	virtual void Update(float deltaTime)override
	{
		auto vx = target->position.x - gameObject->position.x;
		auto vy = target->position.y - gameObject->position.y;
		//移動
		{
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
		
		}
		const float viewSizeX = 40 * 32;
		const float viewSizeY = 22 * 32;
		gameObject->position.x = std::clamp(static_cast<float>(gameObject->position.x), 16.0f, viewSizeX - 16.0f);
		gameObject->position.y = std::clamp(static_cast<float>(gameObject->position.y), 16.0f, viewSizeY - 16.0f);


		runningTimer -= deltaTime;/*キーが押されている間はタイマーを減らす*/
		if (runningTimer <= 0) {
			isFire = true;
			move.moveVector = MyMath::vec3(vx, vy, 0);
			bullet->move.moveVector = move.moveVector;
			bullet->move.speed = 500;
			bullet->type = 1;
			bullet->count = 1;
			bullet->vec = vec;
			bullet->color = 0;
			bullet->EnemyBullet();
			bullet->AngleSet();
			runningTimer += runningTime;/*弾の発射間隔を設定(秒)*/
		}

		if (runningTimer > runningTime - interval && isFire)
		{
			gameObject->position += move.IncreasePos(deltaTime);
		}
		if (runningTimer < runningTime - interval)
			isFire = false;
		
	}

};


#endif // !ATTACHENEMY1_H_INCLUDE