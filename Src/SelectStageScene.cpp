#include "SceneInclude.h"
#include "GLFWWindow.h"
#include "MainGameScene.h"
#include "Component/text.h"
#include "GameObject.h"
#include "Engine.h"
#include "AudioSettings.h"
#include "Component/AudioEvent.h"
#include "Component/FadeController.h"
#include "ClearTimeManager.h"

const MyMath::vec3 stagePoint[] =
{
	{300,511,0},
	{300,511,0},
	{300,511,0},
	{300,511,0},
};


bool SelectStageScene::Initialize(Engine& engine)
{
	/*ゲームオブジェクトを削除*/
	engine.ClearGameObjectList();
	engine.ClearUILayers();
	/*スクロールデータ初期化*/
	{
		scrolldata = std::make_shared<TileMapScroll>();
		scrolldata->sizeX = 0;
		scrolldata->sizeY = 0;
		scrolldata->type = nonScroll;
		scrolldata->speed = 32;
		scrolldata->baseX = 640;
		scrolldata->baseY = 410;
		scrolldata->stage = 1;
		scrolldata->maxStage = 1;
		scrolldata->CalcEndPosition();
		scrolldata->CalcNextStopPosition();
		engine.SetBackGroundTexture(engine.FindWithTexture("Img/worldMap.tga"), *scrolldata);
		engine.CalcStartUpBgLocation(*scrolldata);
	}

	ClearTimeManager& manager = ClearTimeManager::Instance();
	manager.LoadTimes();
	
	stageObjectUI.resize(sizeof(stagePoint) / sizeof(MyMath::vec3));
	for (int i = 0; i < sizeof(stagePoint) / sizeof(MyMath::vec3); i++)
	{
		//stageObjectUI[i] = engine.CreateUI("stageName",)
	}


	return true;
}

void SelectStageScene::Execute(Engine& engine, float deltaTime)
{
}
