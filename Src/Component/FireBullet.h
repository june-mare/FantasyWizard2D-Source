#pragma once
#ifndef FIREBULLET_H_INCLUDE
#define FIREBULLET_H_INCLUDE
#include "../Component.h"
#include "../GameObject.h"
#include "../SpritePriority.h"
#include "../Engine.h"
#include "Move.h"
#include "OutOfScreen.h"
#include "DamageSource.h"
#include "Box2DCollision.h"

/*敵弾発射コンポーネント*/
class FireBullet :public Component
{
public:
	/*初期化時に設定する変数*/
	GameObjectPtr target; //標的
	Texcoord tc = MakeTexcoord(336, 448, 16, 16);
	float intervalA = 1.0f;	//発射間隔
	float intervalB = 0.1f;	//連射間隔
	int numberOfFire = 3;	//連射数
	float speed = 200;		//弾の速度
	MyMath::vec2 position;

	/*実行中に設定する変数*/
	bool isFiring;	//発射フラグ
	float timerA = 0;	//発射タイマー
	float timerB = 0;   //連射タイマー
	int fireCounter = 0;//連射数カウンター
	MyMath::vec2 fireVec;
public:
	FireBullet(GameObjectPtr gameObject)
		:Component(gameObject) { name = "FireBullet"; };
	virtual ~FireBullet() = default;

	virtual void Update(float deltaTime)override
	{
		/*発射用タイマーが発射間隔を超えたら、発射用変数を初期化*/
		timerA += deltaTime;
		if (timerA >= intervalA)
		{
			isFiring = true;
			timerA -= intervalA;
			timerB = intervalB;
			fireCounter = numberOfFire;
			/*ターゲットが指定であれば、発射ベクトルをターゲットに向ける*/
			if (target)
			{
				fireVec.x = target->position.x - gameObject->position.x;
				fireVec.y = target->position.y - gameObject->position.y;
				const float v = fireVec.Magnitude();
				fireVec.x = fireVec.x / v * speed;
				fireVec.y = fireVec.y / v * speed;
			}
			else
			{
				fireVec.x = 0;
				fireVec.y = -speed;
			}
		}
		/*発射モードでなければ何もしない*/
		if (!isFiring)
		{
			return;
		}

		/*連射用タイマーが連射間隔を超えたら弾を発射*/
		timerB += deltaTime;
		if (timerB < intervalB)
		{
			return;
		}
		timerB -= intervalB;

		/*弾を発射*/
		auto bullet = gameObject->engine->Create<GameObject>(
			"enemy bullet", gameObject->position.x + position.x, 
			gameObject->position.y + position.y
			);
		bullet->AddSpriteTexture(tc,gameObject->engine->FindWithTexture("Img/objects.tga"));
		bullet->layer = prioEnemyBullet;
		auto collition = bullet->AddComponent<Box2DCollision>();
		collition->AddCollision({ -4,-4,4,4 });
		auto move = bullet->AddComponent<Move>();
		move->vx = fireVec.x;
		move->vy = fireVec.y;
		bullet->AddComponent<OutOfScreen>();
		auto damageSource = bullet->AddComponent<DamageSource>();
		damageSource->targetName.push_back("player");
		damageSource->isOnce = true;

		/*連射カウンタを減らし　0以下になったら発射モードを解除*/
		--fireCounter;
		if (fireCounter <= 0)
		{
			isFiring = false;
		}
	}
};

#endif // !
