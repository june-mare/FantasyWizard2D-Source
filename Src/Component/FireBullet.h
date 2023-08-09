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

/*�G�e���˃R���|�[�l���g*/
class FireBullet :public Component
{
public:
	/*���������ɐݒ肷��ϐ�*/
	GameObjectPtr target; //�W�I
	Texcoord tc = MakeTexcoord(336, 448, 16, 16);
	float intervalA = 1.0f;	//���ˊԊu
	float intervalB = 0.1f;	//�A�ˊԊu
	int numberOfFire = 3;	//�A�ː�
	float speed = 200;		//�e�̑��x
	MyMath::vec2 position;

	/*���s���ɐݒ肷��ϐ�*/
	bool isFiring;	//���˃t���O
	float timerA = 0;	//���˃^�C�}�[
	float timerB = 0;   //�A�˃^�C�}�[
	int fireCounter = 0;//�A�ː��J�E���^�[
	MyMath::vec2 fireVec;
public:
	FireBullet(GameObjectPtr gameObject)
		:Component(gameObject) { name = "FireBullet"; };
	virtual ~FireBullet() = default;

	virtual void Update(float deltaTime)override
	{
		/*���˗p�^�C�}�[�����ˊԊu�𒴂�����A���˗p�ϐ���������*/
		timerA += deltaTime;
		if (timerA >= intervalA)
		{
			isFiring = true;
			timerA -= intervalA;
			timerB = intervalB;
			fireCounter = numberOfFire;
			/*�^�[�Q�b�g���w��ł���΁A���˃x�N�g�����^�[�Q�b�g�Ɍ�����*/
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
		/*���˃��[�h�łȂ���Ή������Ȃ�*/
		if (!isFiring)
		{
			return;
		}

		/*�A�˗p�^�C�}�[���A�ˊԊu�𒴂�����e�𔭎�*/
		timerB += deltaTime;
		if (timerB < intervalB)
		{
			return;
		}
		timerB -= intervalB;

		/*�e�𔭎�*/
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

		/*�A�˃J�E���^�����炵�@0�ȉ��ɂȂ����甭�˃��[�h������*/
		--fireCounter;
		if (fireCounter <= 0)
		{
			isFiring = false;
		}
	}
};

#endif // !
