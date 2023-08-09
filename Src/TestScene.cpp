#include "SceneInclude.h"
#include "GLFWWindow.h"
#include "MainGameScene.h"
#include "Component/text.h"
#include "GameObject.h"
#include "Engine.h"
#include "AudioSettings.h"
#include "Component/AudioEvent.h"
#include "Component/FadeController.h"


bool TestScene::Initialize(Engine& engine)
{
	/*ゲームオブジェクトを削除*/
	engine.ClearGameObjectList();
	engine.ClearUILayers();
	/*スクロールデータ初期化*/
	{
		scrolldata = std::make_shared<TileMapScroll>();
		scrolldata->sizeX = 50;
		scrolldata->sizeY = 50;
		scrolldata->type = nonScroll;
		scrolldata->speed = 32;
		scrolldata->baseX = -16;
		scrolldata->baseY = -16;
		scrolldata->stage = 1;
		scrolldata->maxStage = 1;
		scrolldata->CalcEndPosition();
		scrolldata->CalcNextStopPosition();
		TileMap tileMap = engine.LoadTileMap("Res/gridTile.csv", *scrolldata);
		engine.SetSpriteList(tileMap, engine.FindWithTexture("Img/gridTile.tga"), *scrolldata);
		engine.CalcStartUpBgLocation(*scrolldata);
	}

	


	player = engine.Create<GameObject>("player", 624, 64);
	playerpram = player->AddComponent<AttachPlayer>();
	playerpram->scrolldata = scrolldata->shared_from_this(); playerpram->testMode = true;
	player->tag = GameObjectTag::player;

	return true;
}

void TestScene::Execute(Engine& engine, float deltaTime)
{
	if (scrolldata->isEndPosision())
	{
		if (!scrolldata->CheckFinalStage()) {
			if (scrolldata->type == upScroll || scrolldata->type == leftScroll)
				scrolldata->stage++;
			else
				if (scrolldata->stage > 1)
					scrolldata->stage--;

			scrolldata->CalcNextStopPosition();
		}
	}

	if (ImGui::CollapsingHeader("ScrollData"))
	{
		ImGui::DragFloat("scrollspeed", &scrolldata->speed);
		auto oldType = scrolldata->type;
		ImGui::InputInt("ScrollType", (int*)&scrolldata->type);
		if (oldType != scrolldata->type)
			scrolldata->CalcNextStopPosition();
		ImGui::DragFloat("endX", &scrolldata->endPosisionX);
		ImGui::DragFloat("endY", &scrolldata->endPosisionY);
		ImGui::InputFloat("MaxStageNum", &scrolldata->maxStage);
		std::floor(scrolldata->maxStage);
		int oldStage = scrolldata->stage;
		ImGui::InputFloat("StageNum", &scrolldata->stage);
		std::floor(scrolldata->stage);
		if (scrolldata->stage != oldStage)
		{
			scrolldata->CalcNextStopPosition();
		}
		int callScene = 0;
		ImGui::DragInt("SceneCall", &callScene);
		switch (callScene)
		{
		case 1:
			engine.SetNextScene<TitleScene>();
			break;
		case 2:
			engine.SetNextScene<SelectStageScene>();
			break;
		case 3:
			engine.SetNextScene<GameOverScene>();
			break;
		case 4:
			engine.SetNextScene<MainStage1>();
			break;
		case 5:
			engine.SetNextScene<Map1ClearScene>();
			break;
		case 6:
			engine.SetNextScene<SampleTextScene>();
			break;
		default:
			break;
		}
		if (scrolldata->maxStage < scrolldata->stage)
			scrolldata->stage = scrolldata->maxStage;
	}

	if (ImGui::CollapsingHeader("EnemySpawn"))
	{
		bool spawnFlg = false;
		float x = spawnPos.x;
		float y = spawnPos.y;
		ImGui::DragFloat("x", &x);
		ImGui::DragFloat("y", &y);
		spawnPos.x = x;
		spawnPos.y = y;
		ImGui::InputInt("effectType", &effectType);
		ImGui::InputInt("enemyType", &enemyType);
		ImGui::Checkbox("Spawn", &spawnFlg);

		if (spawnFlg)
		{
			auto data =
				engine.Create<GameObject>("enemy", spawnPos.x, spawnPos.y)->AddComponent<EndSpawn>();
			data->type = enemyType; data->startEffectType = effectType;
			data->scrolldata = scrolldata->shared_from_this();
		}
		bool clearEnemyFlg = false;
		ImGui::Checkbox("Clear", &clearEnemyFlg);
		if (clearEnemyFlg) {
			auto enemys = engine.FindWithGameObjectsName("enemy");
			for (auto itr : enemys)
			{
				itr->isDead = true;
			}
			auto weapons = engine.FindWithGameObjectsName("weapon");
			for (auto itr : weapons)
			{
				itr->isDead = true;
			}
			auto recoverys = engine.FindWithGameObjectsName("recovery");
			for (auto itr : recoverys)
			{
				itr->isDead = true;
			}
			auto bullets = engine.FindWithGameObjectsName("bullet");
			for (auto itr : bullets)
			{
				itr->isDead = true;
			}
			auto objects = engine.FindWithGameObjectsName("object");
			for (auto itr : objects)
			{
				itr->isDead = true;
			}
		}
	}

	if (ImGui::CollapsingHeader("PlayerPram"))
	{
		int oldlevel = playerpram->bullet->level;
		ImGui::InputInt("Level", &playerpram->bullet->level);
		if (oldlevel != playerpram->bullet->level)
		{
			playerpram->bullet->LevelBohnusReset();
			for (int i = 1; i <= playerpram->bullet->level; i++)
			{
				playerpram->bullet->LevelCalc(i, true);
			}
		}
		ImGui::InputInt("Count", &playerpram->bullet->count);
		ImGui::InputInt("Type", &playerpram->bullet->type);
		ImGui::DragFloat("ShootInterval", &playerpram->bullet->shotInterval);
	}

	engine.BgScroll(*scrolldata);
}
