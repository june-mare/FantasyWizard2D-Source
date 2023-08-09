#pragma once
#ifndef MAINGAMESCENE_H_INCLUDE
#define MAINGAMESCENE_H_INCLUDE

#include "Scene.h"
#include "TileMap.h"
#include "DebugImGui.h"
#include <math.h>
#include "TileMapTexcoord.h"

class ScoreManager;
using ScoreManagerPtr = std::shared_ptr<ScoreManager>;
class Text;
class Gage;
class MoveTimer;
class WeaponUI;
class FadeController;

/*メインゲーム画面*/
class MainStage1 :public Scene
{	
	void ExecutePlayer(Engine& engine, float deltaTime);
	void SpawnEnemy(Engine& engine);
	void LoadMapObject(Engine& engine);
	TileMapScrollPtr scrolldata;
	TileMap objectMap;
	GameObjectPtr player;//プレイヤー
	float restartTimer = 0;//再スタートまでの時間をはかるタイマー
	ScoreManagerPtr scoreManager;
	GameObjectPtr uiScore;
	std::shared_ptr<Text> uiLevel;
	std::shared_ptr<Text> uiHealth;
	std::shared_ptr<Gage> uigage;
	std::shared_ptr<MoveTimer> moveTimer;
	std::vector<DebugImGui> debug;
	std::shared_ptr<FadeController> fades[9];
	float stageNum = 1;
	float maxStage = 5;
	float interval = 3;
	float Time;
	bool fadein = false;
	float fadeinterval = 2;
	float fadeallInterval = 0;
	bool spawnFlg = false;
public:
	
	MainStage1() = default;
	virtual ~MainStage1() = default;
	virtual bool Initialize(Engine& engine)override;
	virtual void Execute(Engine& engine,float deltaTime)override;
};

#endif //MAINGAMESCENE_H_INCLUDE