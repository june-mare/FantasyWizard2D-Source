#pragma once
#ifndef BULLETCOMPONENT_H_INCLUDE
#define BULLETCOMPONENT_H_INCLUDE
#include "../Component.h"
#include "../GameObject.h"
#include "ComponentInclude.h"
#include "BulletList.h"
#include "../EffectList.h"
#include "../AudioSettings.h"

class BulletType1
	:public Component
{
public:
	std::vector<std::string>
		targets;
	int type = 0;
	float interval = 1;
	float angle = 0.0f;
	MovePram move = {};
	MyMath::vec4 startPos = {};
	MyMath::vec4 targetPos = {};

	BulletType1(GameObjectPtr gameObject)
		:Component(gameObject) {
		name = "BulletComponent1";
	}
	virtual ~BulletType1() = default;

	virtual void Start()override
	{
		//スプライト
		{
			std::vector<Texcoord> tcList;
			switch (type)
			{
			case 0:
				tcList = BulletTCList<RedBullet1>();
				break;
			case 1:
				tcList = BulletTCList<BuleBullet1>();
				break;
			case 2:
				tcList = BulletTCList<GreenBullet1>();
				break;
			case 3:
				tcList = BulletTCList<YellowBullet1>();
				break;
			case 4:
				tcList = BulletTCList<PurpleBullet1>();
				break;
			case 5:
				tcList = BulletTCList<LightBlueBullet1>();
				break;
			}
			gameObject->AddSpriteTexture(*tcList.begin(),
				gameObject->engine->FindWithTexture("Img/objects.tga"));
			auto anim = gameObject->AddComponent<Animator2D>();
			anim->clip.resize(tcList.size());
			for (int i = 0; i < tcList.size(); i++) {
				anim->AddSpriteTexture(i, tcList[i], 2.0f);
			}
		}

		//コリジョン
		{
			auto collitionList = gameObject->AddComponent<Box2DCollision>();
			collitionList->AddCollision({ -10,-10,10,10 });
		}

		gameObject->AddComponent<OutOfScreen>();

		//ダメージソース
		{
			auto damageSource = gameObject->AddComponent<DamageSource>();
			for (auto i = 0; i < targets.size(); i++) {
				damageSource->targetName.push_back(targets[i]);
			}
			damageSource->isOnce = true;
		}

		Reload();
	}

	virtual void Update(float deltaTime) override
	{
		move.Acceleration();
		gameObject->position += move.IncreasePos(deltaTime);
	}

};

class BulletType2
	:public Component
{
public:
	std::vector<std::string>
		targets;
	int type = 0;
	side vec = side::up;
	float interval = 1;
	float angle = 0.0f;
	MovePram move = {};
	MyMath::vec4 startPos = {};
	MyMath::vec4 targetPos = {};
	float startTime = 0;

	BulletType2(GameObjectPtr gameObject)
		:Component(gameObject) {
		name = "BulletComponent2";
	}
	virtual ~BulletType2() = default;

	virtual void Start()override
	{
		//スプライト
		{
			std::vector<Texcoord> tcList;
			switch (type)
			{
			case 0:
				tcList = BulletTCList<RedBullet2>();
				break;
			case 1:
				tcList = BulletTCList<BuleBullet2>();
				break;
			case 2:
				tcList = BulletTCList<GreenBullet2>();
				break;
			case 3:
				tcList = BulletTCList<YellowBullet2>();
				break;
			case 4:
				tcList = BulletTCList<PurpleBullet2>();
				break;
			case 5:
				tcList = BulletTCList<LightBlueBullet2>();
				break;
			}
			gameObject->AddSpriteTexture(*tcList.begin(),
				gameObject->engine->FindWithTexture("Img/objects.tga"));
			auto anim = gameObject->AddComponent<Animator2D>();
			anim->clip.resize(tcList.size());
			for (int i = 0; i < tcList.size(); i++) {
				anim->AddSpriteTexture(i, tcList[i], 2.0f);
			}
		}

		//コリジョン
		{
			auto collitionList = gameObject->AddComponent<Box2DCollision>();
			collitionList->AddCollision({ -10,-10,10,10 });
		}

		gameObject->AddComponent<OutOfScreen>();

		//ダメージソース
		{
			auto damageSource = gameObject->AddComponent<DamageSource>();
			for (auto i = 0; i < targets.size(); i++) {
				damageSource->targetName.push_back(targets[i]);
			}
			damageSource->isOnce = true;
		}

		Reload();
	}

	virtual void Update(float deltaTime) override
	{
		move.Acceleration();

		auto ranble = move.IncreasePos(deltaTime);
		angle += (20 * deltaTime);
		if (angle > 360)angle = 0;
		if (vec == side::down)ranble.y *= -1;
		if (vec == side::left)ranble.x *= -1;

		if (vec == side::up || vec == side::down) {
			gameObject->position += MyMath::vec3(ranble.x * cos(angle), ranble.y, 0.0f);
		}
		else
		{
			gameObject->position += MyMath::vec3(ranble.x, ranble.y * cos(angle), 0.0f);
		}

	}

};

class BulletType3
	:public Component
{
public:
	std::vector<std::string>
		targets;
	int type = 0;
	side vec = side::up;
	float interval = 1;
	float angle = 0.0f;
	MovePram move = {};
	MyMath::vec4 startPos = {};
	MyMath::vec4 targetPos = {};
	float startTime = 0;

	BulletType3(GameObjectPtr gameObject)
		:Component(gameObject) {
		name = "BulletType3";
	}
	virtual ~BulletType3() = default;

	virtual void Start()override
	{
		gameObject->layer = prioLaser;
		std::vector<Texcoord> tcList;
		LaserBlueSmall data;
		//スプライト
		{
			switch (type)
			{
			case 0:
				tcList = BulletTCList<LaserRedSmall>();
				break;
			case 1:
				tcList = BulletTCList<LaserBlueSmall>();
				break;
			}
			gameObject->AddSpriteTexture(*tcList.begin(),
				gameObject->engine->FindWithTexture("Img/Effects.tga"));
			auto anim = gameObject->AddComponent<Animator2D>();
			anim->killWhenEnd = true;
			anim->loopTime = false;
			anim->clip.resize(tcList.size());
			if (vec == side::down) {
				for (int i = 0; i < tcList.size(); i++) {
					anim->AddSpriteTexture(i, tcList[i], 2.0f, 0, -data.size.y);
				}
			}
			if (vec == side::up) {
				for (int i = 0; i < tcList.size(); i++) {
					anim->AddSpriteTexture(i, tcList[i], 2.0f, 0, data.size.y, MyMath::Radian2Degree(180));
				}
			}
			if (vec == side::right) {
				for (int i = 0; i < tcList.size(); i++) {
					anim->AddSpriteTexture(i, tcList[i], nullptr, data.size.y * 1.5,
						0, 3, 1, MyMath::Radian2Degree(-90));
				}
			}
			if (vec == side::left) {
				for (int i = 0; i < tcList.size(); i++) {
					anim->AddSpriteTexture(i, tcList[i], nullptr,
						-data.size.y * 1.5, 0, 3, 1, MyMath::Radian2Degree(90));
				}
			}
		}

		//コリジョン
		{
			if (vec == side::down) {
				auto collitionList = gameObject->AddComponent<Box2DCollision>();
				collitionList->AddCollision(
					{ -static_cast<float>(data.size.x / 2),
					-720,
					static_cast<float>(data.size.x / 2),
					0
					}
				);
			}
			if (vec == side::up) {
				auto collitionList = gameObject->AddComponent<Box2DCollision>();
				collitionList->AddCollision(
					{ -static_cast<float>(data.size.x / 2),
					0,
					static_cast<float>(data.size.x / 2),
					720
					}
				);
			}
			if (vec == side::right) {
				auto collitionList = gameObject->AddComponent<Box2DCollision>();
				collitionList->AddCollision(
					{
					0,
					-static_cast<float>(data.size.x / 2),
					1280,
					static_cast<float>(data.size.x / 2),
					}
				);
			}
			if (vec == side::left) {
				auto collitionList = gameObject->AddComponent<Box2DCollision>();
				collitionList->AddCollision(
					{
					-1280,
					-static_cast<float>(data.size.x / 2),
					0,
					static_cast<float>(data.size.x / 2),
					}
				);
			}
		}

		//ダメージソース
		{
			auto damageSource = gameObject->AddComponent<DamageSource>();
			for (auto i = 0; i < targets.size(); i++) {
				damageSource->targetName.push_back(targets[i]);
			}
			damageSource->isLaser = true;
			damageSource->interval = 0.08;
		}

		Reload();
	}
};

class BulletType4
	:public Component
{
public:
	std::vector<std::string>
		targets;
	int type = 0;
	side vec = side::up;
	float interval = 1;
	float angle = 0.0f;
	MovePram move = {};
	MyMath::vec4 startPos = {};
	MyMath::vec4 targetPos = {};
	float startTime = 0;

	BulletType4(GameObjectPtr gameObject)
		:Component(gameObject) {
		name = "BulletType4";
	}
	virtual ~BulletType4() = default;

	virtual void Start()override
	{
		std::vector<Texcoord> tcList;
		LaserPinkBig data;
		gameObject->layer = prioLaser;
		//スプライト
		{
			tcList = BulletTCList<LaserPinkBig>();

			gameObject->AddSpriteTexture(*tcList.begin(),
				gameObject->engine->FindWithTexture("Img/Effects.tga"));
			auto anim = gameObject->AddComponent<Animator2D>();
			anim->killWhenEnd = true;
			anim->loopTime = false;
			anim->clip.resize(tcList.size());
			if (vec == side::down) {
				for (int i = 0; i < tcList.size(); i++) {
					anim->AddSpriteTexture(i, tcList[i], 2.0f, 0, -data.size.y);
				}
			}
			if (vec == side::up) {
				for (int i = 0; i < tcList.size(); i++) {
					anim->AddSpriteTexture(i, tcList[i], 2.0f, 0, data.size.y, MyMath::Radian2Degree(180));
				}
			}
			if (vec == side::right) {
				for (int i = 0; i < tcList.size(); i++) {
					anim->AddSpriteTexture(i, tcList[i], nullptr, data.size.y * 1.5,
						0, 3, 1, MyMath::Radian2Degree(-90));
				}
			}
			if (vec == side::left) {
				for (int i = 0; i < tcList.size(); i++) {
					anim->AddSpriteTexture(i, tcList[i], nullptr,
						-data.size.y * 1.5, 0, 3, 1, MyMath::Radian2Degree(90));
				}
			}
		}

		//コリジョン
		{
			if (vec == side::down) {
				auto collitionList = gameObject->AddComponent<Box2DCollision>();
				collitionList->AddCollision(
					{ -static_cast<float>(data.size.x / 2),-720,
					static_cast<float>(data.size.x / 2),0 });
			}
			if (vec == side::up) {
				auto collitionList = gameObject->AddComponent<Box2DCollision>();
				collitionList->AddCollision(
					{ -static_cast<float>(data.size.x / 2),
					0,
					static_cast<float>(data.size.x / 2),
					720
					}
				);
			}
			if (vec == side::right) {
				auto collitionList = gameObject->AddComponent<Box2DCollision>();
				collitionList->AddCollision(
					{
					0,
					-static_cast<float>(data.size.x / 2),
					1280,
					static_cast<float>(data.size.x / 2),
					}
				);
			}
			if (vec == side::left) {
				auto collitionList = gameObject->AddComponent<Box2DCollision>();
				collitionList->AddCollision(
					{
					-1280,
					-static_cast<float>(data.size.x / 2),
					0,
					static_cast<float>(data.size.x / 2),
					}
				);
			}
		}

		//ダメージソース
		{
			auto damageSource = gameObject->AddComponent<DamageSource>();
			for (auto i = 0; i < targets.size(); i++) {
				damageSource->targetName.push_back(targets[i]);
			}
			damageSource->isLaser = true;
			damageSource->interval = 0.075;
		}

		Reload();
	}
};

class BulletType5
	:public Component
{
public:
	std::vector<std::string>targets;
	int type = 0;
	side vec = side::up;
	float angle = 0.0f;
	MovePram move = {};

	MyMath::vec4 targetPos = {};
	float startTime = 40;
	float activTime = 7;

	BulletType5(GameObjectPtr gameObject)
		:Component(gameObject) {
		name = "BulletType5";
	}
	virtual ~BulletType5() = default;

	virtual void Start()override
	{
		//スプライト
		{
			std::vector<Texcoord> tcList;
			switch (type)
			{
			case 0:
				tcList = BulletTCList<RedBullet3>();
				break;
			case 1:
				tcList = BulletTCList<BuleBullet3>();
				break;
			case 2:
				tcList = BulletTCList<GreenBullet3>();
				break;
			case 3:
				tcList = BulletTCList<YellowBullet3>();
				break;
			case 4:
				tcList = BulletTCList<PurpleBullet3>();
				break;
			case 5:
				tcList = BulletTCList<LightBlueBullet3>();
				break;
			}
			gameObject->AddSpriteTexture(*tcList.begin(),
				gameObject->engine->FindWithTexture("Img/objects.tga"));
			auto anim = gameObject->AddComponent<Animator2D>();
			anim->clip.resize(tcList.size());
			for (int i = 0; i < tcList.size(); i++) {
				anim->AddSpriteTexture(i, tcList[i], 2.0f);
			}
		}

		//コリジョン
		{
			auto collitionList = gameObject->AddComponent<Box2DCollision>();
			collitionList->AddCollision({ -10,-10,10,10 });
		}

		//ダメージソース
		{
			auto damageSource = gameObject->AddComponent<DamageSource>();
			for (auto i = 0; i < targets.size(); i++) {
				damageSource->targetName.push_back(targets[i]);
			}
			damageSource->isOnce = true;
		}
		targetPos = gameObject->position;
		Reload();
	}

	virtual void Update(float deltaTime) override
	{
		if (activTime <= 0) {
			gameObject->isDead = true; return;
		}
		angle += (move.accIncrease * deltaTime); if (angle > 360)angle = 0;
		gameObject->position = targetPos + (MyMath::vec3(startTime * cos(angle), startTime * sin(angle), 0.0f));
		startTime += move.speed * deltaTime;
		activTime -= deltaTime;
	}

};

class BulletType6
	:public Component
{
public:
	std::vector<std::string>
		targets;
	int type = 0;
	side vec;
	float interval = 1;
	float angle = 0.0f;
	MovePram move = {};
	MyMath::vec4 startPos = {};
	MyMath::vec4 targetPos = {};

	BulletType6(GameObjectPtr gameObject)
		:Component(gameObject) {
		name = "BulletComponent1";
	}
	virtual ~BulletType6() = default;

	virtual void Start()override
	{

		float rad = rand() % 20 - 10;
		int base = vec == side::up
			? 180 : vec == side::down
			? 0 : vec == side::right
			? 270 : 90;

		move.speed = 900;
		move.accIncrease = 5;
		move.accMax = 200;

		//スプライト
		{
			std::vector<Texcoord> tcList;
			switch (type)
			{
			case 0:
				tcList = BulletTCList<RedBullet8>();
				break;
			case 1:
				tcList = BulletTCList<BuleBullet8>();
				break;
			case 2:
				tcList = BulletTCList<GreenBullet8>();
				break;
			case 3:
				tcList = BulletTCList<YellowBullet8>();
				break;
			case 4:
				tcList = BulletTCList<PurpleBullet8>();
				break;
			case 5:
				tcList = BulletTCList<LightBlueBullet8>();
				break;
			}
			gameObject->AddSpriteTexture(*tcList.begin(),
				gameObject->engine->FindWithTexture("Img/objects.tga"));
			auto anim = gameObject->AddComponent<Animator2D>();
			anim->clip.resize(tcList.size());
			for (int i = 0; i < tcList.size(); i++) {
				if (vec == side::up || vec == side::left)
					anim->AddSpriteTexture(i, tcList[i], 1.0f, 0, 0, MyMath::Radian2Degree(base + rad));
				if (vec == side::down || vec == side::right)
					anim->AddSpriteTexture(i, tcList[i], 1.0f, 0, 0, MyMath::Radian2Degree(base + -rad));
			}
		}

		rad = MyMath::Radian2Degree(rad);

		//コリジョン
		{
			auto collitionList = gameObject->AddComponent<Box2DCollision>();
			collitionList->AddCollision({ -10,-10,10,10 });
		}

		gameObject->AddComponent<OutOfScreen>();

		//ダメージソース
		{
			auto damageSource = gameObject->AddComponent<DamageSource>();
			for (auto i = 0; i < targets.size(); i++) {
				damageSource->targetName.push_back(targets[i]);
			}
			damageSource->isOnce = true;
		}

		//移動ベクトル
		{
			if (vec == side::up)
			{
				move.moveVector = MyMath::vec3(1 * sin(rad), 1, 0);
			}
			if (vec == side::down)
			{
				move.moveVector = MyMath::vec3(1 * sin(rad), -1, 0);
			}
			if (vec == side::right)
			{
				move.moveVector = MyMath::vec3(1, 1 * sin(rad), 0);
			}
			if (vec == side::left)
			{
				move.moveVector = MyMath::vec3(-1, 1 * sin(rad), 0);
			}
		}
		Reload();
	}

	virtual void Update(float deltaTime) override
	{
		move.Acceleration();
		gameObject->position += move.IncreasePos(deltaTime);
	}

};

class BulletType7
	:public Component
{
public:
	std::vector<std::string>
		targets;
	int type = 0;
	side vec;
	float interval = 1;
	float elapse = 0;
	float angle = 0.0f;
	MovePram move = {};
	MyMath::vec4 startPos = {};
	MyMath::vec4 targetPos = {};

	BulletType7(GameObjectPtr gameObject)
		:Component(gameObject) {
		name = "BulletType7";
	}
	virtual ~BulletType7() = default;

	virtual void Start()override
	{

		startPos = gameObject->position;

		auto enemy = gameObject->engine->FindWithGameObjectName("enemy");
		if (enemy == nullptr)
			targetPos = MyMath::vec3(startPos.x, startPos.y + 1, startPos.z);
		else
			targetPos = enemy->position;

		move.speed = 5;
		/*move.accIncrease = 5;
		move.accMax = 200;*/

		//スプライト
		{
			std::vector<Texcoord> tcList;
			switch (type)
			{
			case 0:
				tcList = BulletTCList<RedBullet7>();
				break;
			case 1:
				tcList = BulletTCList<BuleBullet7>();
				break;
			case 2:
				tcList = BulletTCList<GreenBullet7>();
				break;
			case 3:
				tcList = BulletTCList<YellowBullet7>();
				break;
			case 4:
				tcList = BulletTCList<PurpleBullet7>();
				break;
			case 5:
				tcList = BulletTCList<LightBlueBullet7>();
				break;
			}
			gameObject->AddSpriteTexture(*tcList.begin(),
				gameObject->engine->FindWithTexture("Img/objects.tga"));
			auto anim = gameObject->AddComponent<Animator2D>();
			anim->clip.resize(tcList.size());
			for (int i = 0; i < tcList.size(); i++) {
				anim->AddSpriteTexture(i, tcList[i], 2.0f);
			}
		}

		//コリジョン
		{
			auto collitionList = gameObject->AddComponent<Box2DCollision>();
			collitionList->AddCollision({ -10,-10,10,10 });
		}

		gameObject->AddComponent<OutOfScreen>();

		//ダメージソース
		{
			auto damageSource = gameObject->AddComponent<DamageSource>();
			for (auto i = 0; i < targets.size(); i++) {
				damageSource->targetName.push_back(targets[i]);
			}
			damageSource->isOnce = true;
		}

		Reload();
	}

	virtual void Update(float deltaTime) override
	{
		elapse += deltaTime / move.speed;
		gameObject->position = MyMath::Lerp(startPos, targetPos, elapse);
	}

};


class BulletWays
	:public Component
{
public:
	int count = 1;
	float baseAngle = 0.0f;
	std::vector<std::string>
		targets;
	int type = 0;
	int color = 0;
	float interval = 1;
	float angle = 0.0f;
	float maxAngle = 0;
	float angleIncress = 10;
	float shotTimer;
	float shotInterval = 0.2;
	float exp = 0;
	int level = 1;
	MovePram move = {};
	side vec = side::up;
	MyMath::vec4     startPos = {};
	MyMath::vec4     targetPos = {};


	virtual void Start()override
	{
		move.speed = 700;
		move.accMax = 100;
		move.accIncrease = 0.5f;
	}

	BulletWays(GameObjectPtr gameObject)
		:Component(gameObject) {
		name = "BulletWays";
	}
	virtual ~BulletWays() = default;

	void Spawn()
	{
		for (int i = 0; i < count; i++)
		{
			if (type == 0)
			{
				GameObjectPtr bullet = gameObject->engine->Create<GameObject>("bullet",
					gameObject->position.x, gameObject->position.y);
				auto data = bullet->AddComponent<BulletType1>();
				data->move = move;
				data->type = color;
				data->targets = targets;
				if (count != 1)
				{
					if (vec == side::up)
						data->move.moveVector = MyMath::vec3(MyMath::Radian2Degree(angle), 1, 0);
					else if (vec == side::down)
						data->move.moveVector = MyMath::vec3(MyMath::Radian2Degree(angle), -1, 0);
					else if (vec == side::left)
						data->move.moveVector = MyMath::vec3(-1, MyMath::Radian2Degree(angle), 0);
					else if (vec == side::right)
						data->move.moveVector = MyMath::vec3(1, MyMath::Radian2Degree(angle), 0);
					else
						data->move.moveVector = MyMath::vec3(
							move.moveVector.x * MyMath::Radian2Degree(angle),
							move.moveVector.y * MyMath::Radian2Degree(angle),
							0.0f
						);

					angle += angleIncress;
				}
				else
				{
					data->move.moveVector = move.moveVector;
				}
				//data->Start();
			}
			else if (type == 1)
			{
#pragma region Create
				GameObjectPtr bullet1 = gameObject->engine->Create<GameObject>("bullet",
					gameObject->position.x
					, gameObject->position.y);
				GameObjectPtr bullet2 = gameObject->engine->Create<GameObject>("bullet",
					gameObject->position.x
					, gameObject->position.y);
				bullet1->layer = prioPlayerBullet;

				auto type1 = bullet1->AddComponent<BulletType2>();
				type1->move = move;
				type1->type = color;
				type1->targets = targets;
				type1->targets.resize(targets.size());
				type1->vec = vec;

#pragma endregion
				float which = i % 2;
				if (count != 1) {
					float distance = count % 2 == 0 ? 30 : 60;
					float inccres = (i % 2) + i / 2;
					if (vec == side::up || vec == side::down) {

						bullet1->position.x += (((count / 2) * distance) * -1) + (inccres * 60);
					}
					else if (vec == side::left || vec == side::right)
					{
						bullet1->position.y += (((count / 2) * distance) * -1) + (inccres * 60);
					}
				}
				if (vec == side::down || vec == side::up)
				{
					if (which == 0)
					{
						type1->move.moveVector = MyMath::vec3(-1, 1, 0).normal();
					}
					else
					{
						type1->move.moveVector = MyMath::vec3(1, 1, 0).normal();
					}
				}
				else
				{
					if (which == 0)
					{
						type1->move.moveVector = MyMath::vec3(1, 1, 0).normal();
					}
					else
					{
						type1->move.moveVector = MyMath::vec3(1, -1, 0).normal();
					}
				}
			}
			else if (type == 2)
			{
				GameObjectPtr bullet = gameObject->engine->Create<GameObject>("bullet",
					gameObject->position.x, gameObject->position.y);
				auto data = bullet->AddComponent<BulletType3>();
				data->move = move;
				data->type = color;
				data->targets = targets;
				data->vec = vec;
				data->type = 0;
			}
			else if (type == 3)
			{
				GameObjectPtr bullet = gameObject->engine->Create<GameObject>("bullet",
					gameObject->position.x, gameObject->position.y);
				auto data = bullet->AddComponent<BulletType4>();
				data->move = move;
				data->type = color;
				data->targets = targets;
				data->vec = vec;
				data->type = 0;
				Audio::PlayOneShot(AudioPlayer::SE::laser);
			}
			else if (type == 4)
			{
				GameObjectPtr bullet = gameObject->engine->Create<GameObject>("bullet",
					gameObject->position.x, gameObject->position.y);
				auto data = bullet->AddComponent<BulletType5>();
				data->move = move;
				data->targets = targets;
				data->vec = vec;
				data->type = 0;
			}
			else if (type == 5)
			{
				GameObjectPtr bullet = gameObject->engine->Create<GameObject>("bullet",
					gameObject->position.x, gameObject->position.y);
				auto data = bullet->AddComponent<BulletType6>();
				data->move = move;
				data->type = color;
				data->targets = targets;
				data->vec = vec;
			}
			else if (type == 6)
			{
				GameObjectPtr bullet = gameObject->engine->Create<GameObject>("bullet",
					gameObject->position.x, gameObject->position.y);
				bullet->AddComponent<BulletType7>();
			}
		}
	}
	void EnemyBullet() {
		targets.push_back("player");
		targets.push_back("object");
		targets.resize(2);
	}
	void PlayerBullet() {
		targets.push_back("enemy");
		targets.push_back("object");
		targets.resize(2);
	}
	void AngleSet()
	{
		maxAngle = ((count - 1) * angleIncress) / 2.0f;
		angle = maxAngle * -1;
	}
	void LevelCalc(int other = 888, bool started = false)
	{
		if (other == 888) {
			switch (level)
			{
			case 5:case 10:case 15:
			case 20:case 25:
				count++;
				break;
			default:
				shotInterval -= 0.008;
				break;
			}
		}
		else
		{
			switch (other)
			{
			case 5:case 10:case 15:
			case 20:case 25:
				count++;
				break;
			default:
				shotInterval -= 0.008;
				break;
			}
		}
		if (!started) {
			auto pos = gameObject->engine->FindWithGameObjectName("player")->position;
			auto object = gameObject->engine->Create<GameObject>("effect", pos.x, pos.y + 32);
			object->AddComponent<LevelUpLogo>();
			object = gameObject->engine->Create<GameObject>("effect", pos.x + 48, pos.y + 32);
			object->AddComponent<LevelUpEffect>();
		}
	}
	void LevelBohnusReset()
	{
		shotInterval = 0.2;
		count = 1;
	}
};

class BulletMagic
	:public Component
{
public:
	int type = 0;
	side vec;
	bool enemyMagic = true;
	float activityTime = 3.0f;
	float shotInterval = 0.5f; float shotTimer = 0.0f;
	int count = 1; int color = 0;
	GameObjectPtr target;
	MyMath::vec4 firstPos = {};
	MovePram move;
	std::shared_ptr<BulletWays> bullet;

	BulletMagic(GameObjectPtr gameObject)
		:Component(gameObject) {
		name = "BulletMagic";
	}
	virtual ~BulletMagic() = default;

	virtual void Start()override
	{
		if (enemyMagic)
		{
			target = gameObject->engine->FindWithGameObjectName("player");
		}
		else
		{
			target = gameObject->engine->FindWithGameObjectName("enemy");
		}
		if (target != nullptr)firstPos = target->position;
		bullet = gameObject->AddComponent<BulletWays>();
		Reload();
		if (type == 3 || type == 2)
		{
			activityTime = 0.4;
		}
	}

	virtual void Update(float deltaTime)override
	{
		if (shotTimer <= 0)
		{
			if (target == nullptr) {
				return;
			}

			auto objectVector = (target->position - this->gameObject->position).normal();
			bullet->move.moveVector = {
				objectVector.x,
				objectVector.y,
				objectVector.z
			};
			bullet->targetPos = target->position;
			bullet->type = type;
			bullet->vec = vec;
			bullet->color = color;
			bullet->count = count;
			bullet->move = move;
			bullet->startPos = this->gameObject->position;
			bullet->AngleSet();
			if (enemyMagic)bullet->EnemyBullet();
			else bullet->PlayerBullet();

			bullet->Spawn();
			shotTimer = shotInterval;
		}

		if (activityTime <= 0)
		{
			this->gameObject->isDead = true;
		}
		activityTime -= deltaTime;
		shotTimer -= deltaTime;
	}
};


#endif // !BULLETCOMPONENT_H_INCLUDE