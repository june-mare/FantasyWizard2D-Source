#pragma once
#define _CRT_SECURE_NO_WARNINGS

#ifndef ATTACHPLAYER_H_INCLUDE
#define ATTACHPLAYER_H_INCLUDE
#include "../Component.h"
#include "ComponentInclude.h"
#include "../GameObject.h"
#include <stdio.h>
#include "../EasyAudio.h"
#include "../AudioSettings.h"
#include "BulletList.h"
#include "../CharacterList.h"
#include "BulletComponent.h"

class AttachPlayer :public Component
{
public:
	std::shared_ptr<Animator2D> anim;
	std::shared_ptr<BulletWays> bullet;
	TileMapScrollPtr scrolldata;
	side vec = side::up;
	bool testMode = false;
	AttachPlayer(GameObjectPtr gameObject)
		:Component(gameObject) {
		name = "AttachPlayer";
		this->gameObject->tag = GameObjectTag::player;
	}
	virtual ~AttachPlayer() = default;
	virtual void Debug()override
	{
		int a = bullet->type;
		float b = bullet->angleIncress;
		int c = bullet->count;
		int d = bullet->level;
		int tmp = d;
		if (ImGui::CollapsingHeader(name.c_str()))
		{
			ImGui::DragFloat("shotTimer", &bullet->shotTimer);
			ImGui::DragFloat("shotInterval", &bullet->shotInterval);
			ImGui::DragInt("type", &a);
			bullet->type = a;
			ImGui::DragFloat("angleIncress", &b);
			bullet->angleIncress = b;
			ImGui::DragInt("count", &c);
			bullet->count = c;
			ImGui::DragInt("level", &d);
			bullet->level = d;
		}
		for (int i = 1; i <= d - tmp; i++)
		{
			bullet->LevelCalc(i, true);
		}
	}

	virtual void Start()override
	{
		gameObject->layer = prioPlayer;
		gameObject->AddSpriteTexture(MakeTexcoord(96, 512, 32, 32, 1024, 1024),
			gameObject->engine->FindWithTexture("Img/objects.tga"));
		auto tcList = CharacterTCList<Chara1>();
		anim = gameObject->AddComponent<Animator2D>();
		anim->clip.resize(tcList.size());
		for (int i = 0; i < tcList.size(); i++)
		{
			anim->AddSpriteTexture(i, tcList[i], 1.5);
		}
		anim->interval = 0.2f;
		anim->endCount = static_cast<int>(side::up);
		anim->startCount = static_cast<int>(side::up) - 3;

		bullet = gameObject->AddComponent<BulletWays>();
		bullet->type = 0;
		auto helth = gameObject->AddComponent<Health>();
		helth->health = 3;

		auto explosion = gameObject->AddComponent<EndEffect>();
		explosion->count = 5;

		if (!testMode) {
			auto collition = gameObject->AddComponent<Box2DCollision>();
			collition->AddCollision({ -3.0f,-4.0f,3.0f,4.0f });
		}
		auto audio = gameObject->AddComponent<AudioEvent>();
		audio->files.onTakeDamage = AudioPlayer::SE::enemyExplosionS;
		audio->files.onDestroy = AudioPlayer::SE::playerExplosion;

		gameObject->AddComponent<Shadow>();

		/*FILE* fp;
		int level = 0;		
		fopen_s(&fp,"xxx.dat", "rb");
		fread(&level, sizeof(int), 1, fp);
		fclose(fp);
		*/
		/*gameObject->GetConponent<AttachPlayer>()->bullet->level = 20;
		for (int i = 1; i <= 20; i++)
		{
			gameObject->GetConponent<AttachPlayer>()->bullet->LevelCalc(i,true);
		}*/
		Reload();
	}
	virtual void Update(float deltaTime)override
	{
		/*キー入力を移動方向に変換*/
		const float vx = static_cast<float>(
			gameObject->engine->GetKey(GLFW_KEY_D) - gameObject->engine->GetKey(GLFW_KEY_A));
		const float vy = static_cast<float>(
			gameObject->engine->GetKey(GLFW_KEY_W) - gameObject->engine->GetKey(GLFW_KEY_S));
		const float lock = 
			static_cast<float>(gameObject->engine->GetKey(GLFW_KEY_RIGHT_SHIFT))
			|| static_cast<float>(gameObject->engine->GetKey(GLFW_KEY_LEFT_SHIFT));
		if (lock == 0) {
			if (vx == -1) {
				vec = side::left;
				bullet->move.moveVector = MyMath::vec3(-1, 0, 0);
			}
			if (vx == 1) {
				vec = side::right;
				bullet->move.moveVector = MyMath::vec3(1, 0, 0);
			}
			if (vy == -1) {
				vec = side::down;
				bullet->move.moveVector = MyMath::vec3(0, -1, 0);
			}
			if (vy == 1) {
				vec = side::up;
				bullet->move.moveVector = MyMath::vec3(0, 1, 0);
			}
			if (vy != 0 || vx != 0)
			{
				anim->endCount = static_cast<int>(vec);
				anim->startCount = static_cast<int>(vec) - 3;
			}
			if (vy != 0 && vx != 0)bullet->move.moveVector = MyMath::vec3(vx, vy, 0);
		}
		else
		{
			if (vec == side::left) {
				bullet->move.moveVector = MyMath::vec3(-1, 0, 0);
			}
			if (vec == side::right) {
				bullet->move.moveVector = MyMath::vec3(1, 0, 0);
			}
			if (vec == side::down) {
				bullet->move.moveVector = MyMath::vec3(0, -1, 0);
			}
			if (vec == side::up) {
				bullet->move.moveVector = MyMath::vec3(0, 1, 0);
			}
		}
		/*移動方向の長さが0以上んら入力があったとみなす（０除算を回避するため）*/
		const float v = std::sqrt(vx * vx + vy * vy);	/*移動方向の長さvを求める*/
		if (v > 0)
		{
			const float playerSpeed = 300 * deltaTime / v;
			if (!gameObject->engine->CheckCollideObject(gameObject, MyMath::vec3(MyMath::vec2(
				0,
				vy * playerSpeed
			))))
			{
				gameObject->position.y += vy * playerSpeed;
			}
			if (!gameObject->engine->CheckCollideObject(gameObject, MyMath::vec3(MyMath::vec2(
				vx * playerSpeed,
				0
			))))
			{
				gameObject->position.x += vx * playerSpeed;
			}
		}


		/*自機が画面買いに行かないように、値の範囲を制限する*/
		const float viewSizeX = 1248;
		const float viewSizeY = 704;
		gameObject->position.x = std::clamp(static_cast<float>(gameObject->position.x), 16.0f, viewSizeX - 16.0f);
		gameObject->position.y = std::clamp(static_cast<float>(gameObject->position.y), 16.0f, viewSizeY - 16.0f);	

		int ran1 = rand() % 6;

		const bool shotKey = gameObject->engine->GetKey(GLFW_KEY_SPACE);
		if (shotKey)
		{
			bullet->shotTimer -= deltaTime;/*キーが押されている間はタイマーを減らす*/
			if (bullet->shotTimer <= 0) {
				bullet->vec = vec;
				bullet->color = ran1;
				bullet->PlayerBullet();
				bullet->AngleSet();
				bullet->Spawn();
				bullet->shotTimer += bullet->shotInterval;/*弾の発射間隔を設定(秒)*/
			}
		}
		else
		{
			/*次にキーが押されたら、すぐ弾を発射されるようにする*/
			bullet->shotTimer = 0;
		}
	}

	virtual void OnDestroy()override
	{
		FILE* fp;
		int level = gameObject->GetConponent<AttachPlayer>()->bullet->level;
		fopen_s(&fp,"xxx.dat", "wb");
		fwrite(&level, sizeof(int), 1, fp);
		fclose(fp);
	}
};
#endif // !ATTACHPLAYER_H_INCLUDE