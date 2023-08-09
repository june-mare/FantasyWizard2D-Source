#include "Map1ClearScene.h"
#include "GLFWWindow.h"
#include "MainGameScene.h"
#include "Component/ComponentInclude.h"
#include "GameObject.h"
#include "Engine.h"
#include "AudioSettings.h"
#include "Component/AudioEvent.h"
#include "TitleScene.h"
#include "ClearTimeManager.h"
/*タイトル画面を初期化する*/

const int WorldOneObjectClear[] =
{
	1774,//1822,    1x2 やり立て
	1621,//1360,    4x5 大きい杉の木
	1795,//1891,    1x3 街灯
	1868,//1916,    2x2 剣立て
	106 ,//154,     2x2 たる三つ
	1597,//1741,    2x4 テント
	144 ,//240,     3x3 女神像
	2064,//2256,    4x5 大きいまるい木	
	1168,//1717,    1x3 石の柱
	1544,//1736,    5x5 でかいテント
	1764,//         1x1 たる一つ
	2053 //			1x1 宝箱
};

const SpriteSize WorldOneObjectClearSize[] =
{
	{32,64},    //1822,    1x2 やり立て
	{32,96},	//1360,    4x5 大きい杉の木
	{32,96},	//1891,    1x3 街灯
	{64,64},	//1916,    2x2 剣立て
	{64,64},	//154,     2x2 たる三つ
	{64,128},	//1741,    2x4 テント
	{96,96},	//240,     3x3 女神像
	{128,160},	//2256,    4x5 大きいまるい木	
	{128,160},	//1717,    1x3 石の柱
	{160,160},	//1736,    5x5 でかいテント
	{32,32},	//         1x1 たる一つ
	{32,32},	//			1x1 宝箱
};

void Map1ClearScene::LoadMapObject(Engine& engine)
{
	for (size_t y = objectMap.sizey; y > 0;)
	{
		--y;/*マイナスにならないようにここで減算*/
		for (size_t x = 0; x < objectMap.sizex; ++x)
		{
			/*タイル番号を取得*/
			const int tileNo = objectMap.data[y * objectMap.sizex + x];
			if (tileNo == -1)continue;

			auto texture = engine.FindWithTexture("Img/mapObject.tga");

			const float u = static_cast<float>(tileNo % 48) * 32;
			const float v = static_cast<float>(tileNo / 48) * 32;

			for (int i = 0; i < sizeof(WorldOneObjectClear) / sizeof(int); i++)
			{
				if (tileNo == WorldOneObjectClear[i])
				{					
					auto object = engine.Create<GameObject>("object",
						static_cast<float>(x * 32 + scrolldata->baseX),
						static_cast<float>(y * 32 + scrolldata->baseY)
						);
					object->layer = prioObject1;
					object->AddSpriteTexture(
						MakeTexcoord(
							u, v, WorldOneObjectClearSize[i].x, WorldOneObjectClearSize[i].y,
							texture->width, texture->height), texture);
					auto move = object->AddComponent<ScrollObject>();
					move->scrolldata = scrolldata->shared_from_this();					
				}
			}
		}
	}

}

bool Map1ClearScene::Initialize(Engine& engine) {
	/*ゲームオブジェクトを削除*/
	engine.ClearGameObjectList();
	engine.ClearUILayers();

	/*UIレイヤーを作成*/
	const size_t bgLayer   = engine.AddUILayer("Img/Clear.tga",    GL_LINEAR, 10);
	const size_t logoLayer = engine.AddUILayer("Img/logo.tga",     GL_LINEAR, 16);
	const size_t textLayer = engine.AddUILayer("Img/font.tga",     GL_LINEAR, 10);
	const size_t rankLayer = engine.AddUILayer("Img/RankLogo.tga", GL_LINEAR, 10);
	const size_t underLayer = engine.AddUILayer("Img/ClearUnder.tga", GL_LINEAR, 10);

	/*背景画像*/
	/*auto uiBackground = engine.CreateUI<GameObject>(
		bgLayer, "title_bg", 640, 360);
	uiBackground->AddSpriteTexture({ 0,0,1,1 }, 0, 0, 0.0f, 1.0);*/
	
	scrolldata = std::make_shared<TileMapScroll>();
	scrolldata->sizeX = 40;
	scrolldata->sizeY = 140;
	scrolldata->type = upScroll;
	scrolldata->speed = 32;
	scrolldata->baseX = 16;
	scrolldata->baseY = 16;
	scrolldata->stage = 1;
	scrolldata->maxStage = 1;
	scrolldata->CalcEndPosition();
	scrolldata->CalcNextStopPosition();

	TileMap tileMap = engine.LoadTileMap("Res/bgmap1.csv", *scrolldata);
	engine.SetSpriteList(tileMap, engine.FindWithTexture("Img/bg0.tga"), *scrolldata);
	objectMap = engine.LoadTileMap("Res/mapObject1.csv", *scrolldata);

	LoadMapObject(engine);

	auto uiunder = engine.CreateUI<GameObject>(
		underLayer, "under_logo", 640, 50);
	uiunder->AddSpriteTexture({0,0,1,1},0);

	int rank = 0;
	float time = ClearTimeManager::Instance().ClearTime(1);
	if		(time > 360)rank = 0;
	else if (time > 330)rank = 1;
	else if (time > 300)rank = 2;
	else if (time > 240)rank = 3;
	else				rank = 4;

	auto uiRank = engine.CreateUI<GameObject>(
		rankLayer, "rank_logo", 820, 440);
	uiRank->AddSpriteTexture(MakeTexcoord(rank * 90,0,90,90,450,90),
		0, 0, 0, 5);

	auto uiTime = engine.CreateUI<GameObject>(
		textLayer, "clearTime", 300, 460);
	auto timeManager = uiTime->AddComponent<ScoreManager>();
	timeManager->textComponent = uiTime->AddComponent<Text>();
	timeManager->score = time;
	timeManager->textComponent->scale = 4.0f;

	/*ゲームの開始を示すテキスト*/
	const char strPressEnter[] = "Press Enter";
	const float fontSizeX = 16;
	const float x =
		640 - static_cast<float>(std::size(strPressEnter) - 1) * fontSizeX;
	
	auto uiPressEnter = engine.CreateUI<GameObject>(
		textLayer, "press enter", x, 100);	
	auto textPressEnter = uiPressEnter->AddComponent<Text>();
	textPressEnter->SetText(strPressEnter, 2);	
	
	uiPressEnter->layer = 0;
	//uiBackground->layer = 2;

	Audio::Stop(AudioPlayer::bgm);
	Audio::Play(AudioPlayer::bgm, AudioPlayer::BGM::clear, 1, true);

	return true;
}

void Map1ClearScene::Execute(Engine& engine, float deltaTime)
{
	if (engine.GetKey(GLFW_KEY_ENTER)) {
		engine.SetNextScene<TitleScene>();
		Audio::PlayOneShot(AudioPlayer::SE::enter);
	}	
	engine.BgScroll(*scrolldata);
}