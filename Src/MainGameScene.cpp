#include "MainGameScene.h"
#include "Engine.h"
#include "GameObject.h"
#include "Component/ComponentInclude.h"
#include "Component/FadeController.h"
#include "AudioSettings.h"
#include "Map1ClearScene.h"
#include "Sprite.h"
#include "GameOverScene.h"
#include "ClearTimeManager.h"
#include <fstream>
#include <filesystem>

const int WorldOneObject[] =
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

const SpriteSize WorldOneObjectSize[] =
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

const int GameClear[] =
{
	0,//1822,    1x2 やり立て
	214,//1360,    4x5 大きい杉の木
	428,//1891,    1x3 街灯
	738,//1916,    2x2 剣立て

	930,//154,     2x2 たる三つ
	1166,//1741,    2x4 テント
	1268,//240,     3x3 女神像
	1440,//2256,    4x5 大きいまるい木	
	1676,//1717,    1x3 石の柱
	 //			1x1 宝箱
};

const SpriteSize GameClearSize[] =
{
	{214,326},    //1822,    1x2 やり立て
	{214,326},	//1360,    4x5 大きい杉の木
	{310,326},	//1891,    1x3 街灯

	{214,326},	//1916,    2x2 剣立て
	{236,326},	//154,     2x2 たる三つ

	{102,326},	//1741,    2x4 テント
	{172,326},	//240,     3x3 女神像

	{236,326},	//2256,    4x5 大きいまるい木	
	{157,326},	//1717,    1x3 石の柱
};

/*自機の状態の取得*/
void MainStage1::ExecutePlayer(Engine& engine, float deltaTime)
{
	/*自機が破壊されたらリスタート*/
	if (player->isDead)
	{
		restartTimer += deltaTime;
		if (restartTimer >= 2)
		{
			engine.SetNextScene<GameOverScene>();
		}
		std::string a;
		a += std::to_string(0);
		a += std::to_string(0);
		uiHealth->SetText(a, 2);
		return;
	}
	else
	{
		auto health = player->GetConponent<Health>();
		auto bullet = player->GetConponent<BulletWays>();
		if (health != nullptr && bullet != nullptr) {
			std::string a;
			if (health->health < 10)
				a += std::to_string(0);
			a += std::to_string((int)health->health);

			std::string b;
			if (bullet->level < 10)
				b += std::to_string(0);
			b += std::to_string((int)bullet->level);
			uiHealth->SetText(a, 2);
			uiLevel->SetText(b, 2);

			uigage->baseData = bullet->exp;
		}
	}
}

void MainStage1::SpawnEnemy(Engine& engine)
{
	if ((int)scrolldata->stage == 1)
	{
		auto data =
			engine.Create<GameObject>("enemy", 624, 354)->AddComponent<EndSpawn>();
		data->type = 0; data->startEffectType = 4;
		data = engine.Create<GameObject>("enemy", 312, 619.5)->AddComponent<EndSpawn>();
		data->type = 0; data->startEffectType = 4;
		data = engine.Create<GameObject>("enemy", 936, 619.5)->AddComponent<EndSpawn>();
		data->type = 0; data->startEffectType = 4;
		data = engine.Create<GameObject>("enemy", 1092, 177)->AddComponent<EndSpawn>();
		data->type = 1; data->startEffectType = 6;
		data = engine.Create<GameObject>("enemy", 156, 648)->AddComponent<EndSpawn>();
		data->type = 1; data->startEffectType = 6;
	}	
	if ((int)scrolldata->stage == 5)
	{
		auto data = engine.Create<GameObject>("enemy", 640, 600)
			->AddComponent<EndSpawn>();
		data->startEffectType = 15; data->type = 5;
		Audio::Stop(AudioPlayer::bgm);
		Audio::Play(AudioPlayer::bgm, AudioPlayer::BGM::stage1Boss, 1, true);
	}	
	if ((int)scrolldata->stage == 2)
	{
		auto data =
			engine.Create<GameObject>("enemy", 624, 354)->AddComponent<EndSpawn>();
		data->type = 3; data->startEffectType = 5;
		data = engine.Create<GameObject>("enemy", 156, 177)->AddComponent<EndSpawn>();
		data->type = 3; data->startEffectType = 5;
		data = engine.Create<GameObject>("enemy", 312, 531)->AddComponent<EndSpawn>();
		data->type = 3; data->startEffectType = 5;
		data = engine.Create<GameObject>("enemy", 468, 708)->AddComponent<EndSpawn>();
		data->type = 3; data->startEffectType = 5;
		data = engine.Create<GameObject>("enemy", 780, 708)->AddComponent<EndSpawn>();
		data->type = 3; data->startEffectType = 5;
		data = engine.Create<GameObject>("enemy", 966, 531)->AddComponent<EndSpawn>();
		data->type = 3; data->startEffectType = 5;
		data = engine.Create<GameObject>("enemy", 1092, 177)->AddComponent<EndSpawn>();
		data->type = 3; data->startEffectType = 5;

	}
	if ((int)scrolldata->stage == 3)
	{
		auto data =
			engine.Create<GameObject>("enemy", 400, 400)->AddComponent<EndSpawn>();
		data->type = 2; data->startEffectType = 1;
		data = engine.Create<GameObject>("enemy", 880, 400)->AddComponent<EndSpawn>();
		data->type = 2; data->startEffectType = 1;
		data = engine.Create<GameObject>("enemy", 354, 786)->AddComponent<EndSpawn>();
		data->type = 3; data->startEffectType = 5;
		data = engine.Create<GameObject>("enemy", 845, 351)->AddComponent<EndSpawn>();
		data->type = 3; data->startEffectType = 5;
		data = engine.Create<GameObject>("enemy", 155, 864)->AddComponent<EndSpawn>();
		data->type = 3; data->startEffectType = 5;
		data = engine.Create<GameObject>("enemy", 624, 354)->AddComponent<EndSpawn>();
		data->type = 4; data->startEffectType = 14;

	}
	if ((int)scrolldata->stage == 4)
	{
		auto data = engine.Create<GameObject>("enemy", 624, 354)
			->AddComponent<EndSpawn>();
		data->startEffectType = 14; data->type = 4;
		data = engine.Create<GameObject>("enemy", 1100, 650)
			->AddComponent<EndSpawn>();
		data->startEffectType = 14; data->type = 4;
		data = engine.Create<GameObject>("enemy", 700, 220)
			->AddComponent<EndSpawn>();
		data->startEffectType = 14; data->type = 4;
		data = engine.Create<GameObject>("enemy", 300, 650)
			->AddComponent<EndSpawn>();
		data->startEffectType = 14; data->type = 4;
		data = engine.Create<GameObject>("enemy", 1100, 220)
			->AddComponent<EndSpawn>();
		data->startEffectType = 14; data->type = 4;
	}
}

void MainStage1::LoadMapObject(Engine& engine)
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

			for (int i = 0; i < sizeof(WorldOneObject) / sizeof(int); i++)
			{
				if (tileNo == WorldOneObject[i])
				{
					int a = rand() % 7;
					auto object = engine.Create<GameObject>("object",
						static_cast<float>(x * 32 + scrolldata->baseX),
						static_cast<float>(y * 32 + scrolldata->baseY)
						);
					object->layer = prioObject1;
					object->AddSpriteTexture(
						MakeTexcoord(
							u, v, WorldOneObjectSize[i].x, WorldOneObjectSize[i].y,
							texture->width, texture->height), texture);
					auto helth = object->AddComponent<Health>();
					helth->health = 2 * (WorldOneObjectSize[i].x / 32) + (WorldOneObjectSize[i].y / 32);
					auto collision = object->AddComponent<Box2DCollision>();
					collision->AddCollision({
							-(WorldOneObjectSize[i].x / 2),-(WorldOneObjectSize[i].y / 2),
							WorldOneObjectSize[i].x / 2,WorldOneObjectSize[i].y / 2
						});
					auto effect = object->AddComponent<EndEffect>();
					effect->size = 2; effect->count = 3;
					auto audio = object->AddComponent<AudioEvent>();

					audio->files.onDestroy = AudioPlayer::SE::enemyExplosionL;
					auto move = object->AddComponent<ScrollObject>();
					move->scrolldata = scrolldata->shared_from_this();
					if (i == 11)
					{
						auto endSpawn = object->AddComponent<EndSpawn>();
						endSpawn->isStartUp = false;
						endSpawn->spawnType = weaponspawn;
						endSpawn->scrolldata = scrolldata->shared_from_this();
					}
					else if (a == 0)
					{
						auto endSpawn = object->AddComponent<EndSpawn>();
						endSpawn->isStartUp = false;
						endSpawn->spawnType = recoveryspawn;
						endSpawn->scrolldata = scrolldata->shared_from_this();
					}
				}
			}
		}
	}

}

/*シーンの初期化
@retval true	初期化成功
@retval false	初期化失敗
*/
bool MainStage1::Initialize(Engine& engine)
{
	//auto& window = GLFW_SPACE::Window::Instance();
	std::mt19937& rg = engine.GetRandomGenerator();
	//const auto [bgSizeX, bgSizeY, bgBaseX, bgBaseY] = engine.GetBgSize();

	engine.ClearGameObjectList();
	engine.ClearUILayers();

	/*スクロールデータ初期化*/
	{
		scrolldata = std::make_shared<TileMapScroll>();
		scrolldata->sizeX = 40;
		scrolldata->sizeY = 140;
		scrolldata->type = upScroll;
		scrolldata->speed = 96;
		scrolldata->baseX = 16;
		scrolldata->baseY = 16;
		scrolldata->stage = 1;
		scrolldata->maxStage = 5;
		scrolldata->CalcEndPosition();
		scrolldata->CalcNextStopPosition();
		//scrolldata->endPosision = scrolldata->endPosision * (stageNum / maxStage);
	}
	/*乱数を初期化*/
	std::random_device rd;	/*真の乱数を生成するオブジェクト*/

	rg.seed(rd());			/*疑似乱数を「真の乱数」で初期化*/

	/*自機スプライトを追加*/
	player = engine.Create<GameObject>("player",
		(static_cast<float>(scrolldata->sizeX) * 32) / 2, 64);
	auto a = player->AddComponent<AttachPlayer>();
	a->scrolldata = scrolldata;
	player->tag = GameObjectTag::player;

#ifdef DEBUG

	DebugImGui playerDebug;
	playerDebug.SetUp(player);
	debug.push_back(playerDebug);

#endif // DEBUG

	/*背景スプライトを作成(タイルサイズ = 32*32)*/
	TileMap tileMap = engine.LoadTileMap("Res/bgmap1.csv", *scrolldata);
	engine.SetSpriteList(tileMap, engine.FindWithTexture("Img/bg0.tga"), *scrolldata);
	objectMap = engine.LoadTileMap("Res/mapObject1.csv", *scrolldata);

	LoadMapObject(engine);

	/*UIレイヤーを追加*/
	const size_t gage = engine.AddUILayer("Img/expGage.tga", GL_NEAREST, 10);
	auto uiga = engine.CreateUI<GameObject>(gage, "gage", 670, 30);
	uiga->AddSpriteTexture({ 0,0,1,1 }, engine.FindWithTexture("Img/menuber.tga"));
	uigage = uiga->AddComponent<Gage>();

	const size_t menuber = engine.AddUILayer("Img/menuber.tga", GL_NEAREST, 10);
	auto uimenu = engine.CreateUI<GameObject>(menuber, "menu", 640, 50);
	uimenu->AddSpriteTexture({ 0,0,1,1 }, engine.FindWithTexture("Img/menuber.tga"));

	const size_t textLayer = engine.AddUILayer("Img/font.tga", GL_NEAREST, 10);

	uiScore = engine.CreateUI<GameObject>(textLayer, "text", 1480, 680);
	scoreManager = uiScore->AddComponent<ScoreManager>();
	scoreManager->textComponent = uiScore->AddComponent<Text>();
	moveTimer = uiScore->AddComponent<MoveTimer>();




	auto Health = engine.CreateUI<GameObject>(textLayer, "text", 270, 20);
	uiHealth = Health->AddComponent<Text>();

	auto Level = engine.CreateUI<GameObject>(textLayer, "text", 520, 20);
	uiLevel = Level->AddComponent<Text>();

	const size_t weaponLayer = engine.AddUILayer("Img/WeaponUI.tga", GL_NEAREST, 10);
	auto weapon = engine.CreateUI<GameObject>(weaponLayer, "text", 1050, 20);
	weapon->AddComponent<WeaponUI>();

	const size_t titletextLayer = engine.AddUILayer("Img/ClearFont.tga", GL_LINEAR, 10);
	float upbase = 128;
	for (int i = 0; i < 9; i++)
	{
		auto uiFont = engine.CreateUI<GameObject>(
			titletextLayer, "title_font",
			upbase, 500);
		uiFont->AddSpriteTexture(
			MakeTexcoord(GameClear[i], 0, GameClearSize[i].x, GameClearSize[i].y, 1833, 326),
			0, 0, 0, 0.5, 0, 0);

		fades[i] = uiFont->AddComponent<FadeController>();
		fades[i]->fadeWhich = true; fades[i]->fadeSpeed = 1;
		fades[i]->fadeObject = uiFont;
		if (i < 4)upbase += 128;
		else upbase += 96;
	}

	Audio::Stop(AudioPlayer::bgm);
	Audio::Play(AudioPlayer::bgm, AudioPlayer::BGM::stage1, 1, true);

	return true;//初期化成功
}

/*メインゲーム画面の状態を更新する*/
void MainStage1::Execute(Engine& engine, float deltaTime)
{
	ExecutePlayer(engine, deltaTime);
	if (scrolldata->isEndPosision() && !spawnFlg)
	{		
		SpawnEnemy(engine);
		spawnFlg = true;
	}

#ifdef DEBUG

	if (!ImGui::CollapsingHeader("ScrollData"))
	{
		ImGui::DragFloat("scrollspeed", &scrolldata->speed);
		ImGui::DragFloat("endX", &scrolldata->endPosisionX);
		ImGui::DragFloat("endY", &scrolldata->endPosisionY);
		auto oldType = scrolldata->type;
		ImGui::DragInt("ScrollType", (int*)&scrolldata->type);
		if (oldType != scrolldata->type)
			scrolldata->CalcNextStopPosition();
		int oldnum = scrolldata->stage;
		ImGui::DragFloat("StageNum", &scrolldata->stage);
		std::floor(scrolldata->stage);
		if (oldnum != scrolldata->stage)
			scrolldata->CalcNextStopPosition();
	}

	for (auto itr : debug)
	{
		itr.Execute();
	}
	debug.erase(
		std::remove_if(debug.begin(), debug.end(), [](DebugImGui _debug) {
			return _debug.gameObject->isDead;
			})
		, debug.end());

#endif // DEBUG

	engine.BgScroll(*scrolldata);
	moveTimer->CustomEvent(deltaTime);

	auto enemys = engine.FindWithGameObjectsName("enemy");
	if (enemys.size() == 0 && spawnFlg)
	{
		if (!scrolldata->CheckFinalStage()) {
			if (scrolldata->type == upScroll || scrolldata->type == leftScroll)
				scrolldata->stage++;
			else
				if (scrolldata->stage > 1)
					scrolldata->stage--;
			scrolldata->CalcNextStopPosition();
			spawnFlg = false;
			moveTimer->trriger = true;
		}
		else if (scrolldata->CheckFinalStage())
		{
			if (engine.GetKey(GLFW_KEY_ENTER))
			{
				engine.SetNextScene<Map1ClearScene>();
				ClearTimeManager& manager = ClearTimeManager::Instance();
				manager.ClearTime(1,Time);
				manager.SaveTimes();
			}
			uiScore->position = { 500,225,0 };
			scoreManager->scale = 5;;
			fadeinterval -= deltaTime;
			if (fadeallInterval > 0) {
				fadeinterval -= deltaTime;
				if (fadeinterval < 0) {
					int a = rand() % 9;

					if (fades[a]->trriger != true)
					{
						fades[a]->trriger = true;
					}
					fadeinterval = 0.3;
				}
				for (auto itr : fades)
				{
					itr->CustomEvent2(deltaTime);
				}
			}
			else
			{
				if (fadein == false) {
					int count = 0;
					for (int i = 0; i < 9; i++)
					{
						fades[i]->trriger = false;
						fades[i]->FadeOut(deltaTime);
						if (fades[i]->fadeObject->spriteList[0].spriteList[0].alpha <= 0.00f)
							count++;
					}
					if (count == 9)
					{
						fadein = true;
					}
				}
				else
				{
					int count = 0;
					for (int i = 0; i < 9; i++)
					{
						fades[i]->trriger = false;
						fades[i]->FadeIn(deltaTime);
						if (fades[i]->fadeObject->spriteList[0].spriteList[0].alpha >= 1.00f)
							count++;
					}
					if (count == 9)
					{
						fadein = false;
						fadeallInterval = 10;
					}
				}
			}
			fadeallInterval -= deltaTime;
		}
	}
	else if (enemys.size() != 0)
	{
		Time += deltaTime;
		uiScore->GetConponent<ScoreManager>()->score = Time;
	}
}
