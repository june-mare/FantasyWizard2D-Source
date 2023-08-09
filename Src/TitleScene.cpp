#include "SceneInclude.h"
#include "GLFWWindow.h"
#include "MainGameScene.h"
#include "Component/text.h"
#include "GameObject.h"
#include "Engine.h"
#include "AudioSettings.h"
#include "Component/AudioEvent.h"
#include "Component/FadeController.h"

const int TitleFont[] =
{
	0,//1822,    1x2 やり立て
	144,//1360,    4x5 大きい杉の木
	288,//1891,    1x3 街灯
	432,//1916,    2x2 剣立て
	530 ,//154,     2x2 たる三つ
	683,//1741,    2x4 テント
	817 ,//240,     3x3 女神像
	991,//2256,    4x5 大きいまるい木	
	1192,//1717,    1x3 石の柱
	1282,//1736,    5x5 でかいテント
	1432,//         1x1 たる一つ
	1592, //			1x1 宝箱
	1698, //			1x1 宝箱	

};

const SpriteSize TitleFontSize[] =
{
	{144,283},      //F,    1x2 やり立て
	{144,283},	    //a,    4x5 大きい杉の木
	{144,283},	    //n,    1x3 街灯
	{98, 283 },	    //t,    2x2 剣立て
	{153, 283 },    //a,     2x2 たる三つ
	{134, 283 },    //s,    2x4 テント
	{174, 283 },    //y,     3x3 女神像
	{210,283},	    //w,    4x5 大きいまるい木		
	{90,283},	    //,    1x3 石の柱
	{150,283},	    //1736,    5x5 でかいテント
	{160, 283 },	//         1x1 たる一つ
	{106 ,283 },	//			1x1 宝箱
	{134 ,283 },	//			1x1 宝箱
	{0 ,283 },	//			1x1 宝箱
};

/*タイトル画面を初期化する*/

bool TitleScene::Initialize(Engine& engine) {
	/*ゲームオブジェクトを削除*/
	engine.ClearGameObjectList();
	engine.ClearUILayers();

	/*UIレイヤーを作成*/
	const size_t bgLayer = engine.AddUILayer("Img/title.tga", GL_LINEAR, 10);
	const size_t textLayer = engine.AddUILayer("Img/font.tga", GL_LINEAR, 10);
	const size_t titletextLayer = engine.AddUILayer("Img/TitleFont.tga", GL_LINEAR, 10);

	/*背景画像*/
	auto uiBackground = engine.CreateUI<GameObject>(
		bgLayer, "title_bg", 640, 420);
	uiBackground->AddSpriteTexture({ 0,0,1,1 }, 0, 0, 0.0f, 1.0);
	/*タイトルロゴ*/
	
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
	uiBackground->layer = 2;

	float upbase = 50;
	float downbase = 300;

	for (int i = 0; i < 13; i++)
	{
		if (i < 7) {
			auto uiFont = engine.CreateUI<GameObject>(
				titletextLayer, "title_font",
				upbase + TitleFontSize[i].x / 2, 600);
			uiFont->AddSpriteTexture(
				MakeTexcoord(TitleFont[i], 0, TitleFontSize[i].x, TitleFontSize[i].y, 1832, 283),
				0, 0, 0, 0.5);
			fades[i] = uiFont->AddComponent<FadeController>();
			fades[i]->fadeWhich = false; fades[i]->fadeSpeed = 0.5;
			upbase += TitleFontSize[i].x / 2; fades[i]->fadeObject = uiFont;
		}
		else
		{
			if (i == 7) {
				auto uiFont = engine.CreateUI<GameObject>(
					titletextLayer, "title_font", downbase + TitleFontSize[i].x / 2 - 20, 400);
				uiFont->AddSpriteTexture(
					MakeTexcoord(TitleFont[i], 0, TitleFontSize[i].x, TitleFontSize[i].y, 1832, 283),
					0, 0, 0, 0.5);
				fades[i] = uiFont->AddComponent<FadeController>();
				fades[i]->fadeSpeed = 0.5;
				fades[i]->fadeWhich = false; fades[i]->fadeObject = uiFont;
				downbase += TitleFontSize[i].x / 2;
			}
			else
			{
				auto uiFont = engine.CreateUI<GameObject>(
					titletextLayer, "title_font", downbase + TitleFontSize[i].x / 2, 400);
				uiFont->AddSpriteTexture(
					MakeTexcoord(TitleFont[i], 0, TitleFontSize[i].x, TitleFontSize[i].y, 1832, 283),
					0, 0, 0, 0.5);
				fades[i] = uiFont->AddComponent<FadeController>();
				fades[i]->fadeSpeed = 0.5;
				fades[i]->fadeWhich = false; fades[i]->fadeObject = uiFont;
				downbase += TitleFontSize[i].x / 2;
			}
		}
	}

	Audio::Play(AudioPlayer::bgm, AudioPlayer::BGM::title, 1.5, true);

	return true;
}

void TitleScene::Execute(Engine& engine, float deltaTime)
{
	if (allInterval > 0) {
		interval -= deltaTime;
		if (interval < 0) {
			int a = rand() % 13;

			if (fades[a]->trriger != true)
			{
				fades[a]->trriger = true;
			}
			interval = 0.3;
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
			for (int i = 0; i < 13; i++)
			{
				fades[i]->trriger = false;
				fades[i]->FadeOut(deltaTime);
				if (fades[i]->fadeObject->spriteList[0].spriteList[0].alpha <= 0.00f)
					count++;
			}
			if (count == 13)
				fadein = true;
		}
		else
		{
			int count = 0;
			for (int i = 0; i < 13; i++)
			{
				fades[i]->trriger = false;
				fades[i]->FadeIn(deltaTime);
				if (fades[i]->fadeObject->spriteList[0].spriteList[0].alpha >= 1.00f)
					count++;
			}
			if (count == 13)
			{
				fadein = false;
				allInterval = 10;
			}
		}
	}
	allInterval -= deltaTime;
	if (engine.GetKey(GLFW_KEY_ENTER)) {
		Audio::PlayOneShot(AudioPlayer::SE::enter);
		engine.SetNextScene<MainStage1>();
	}

	if (engine.GetKey(GLFW_KEY_LEFT_CONTROL)
		&& engine.GetKey(GLFW_KEY_D)
		&& engine.GetKey(GLFW_KEY_B)
		&& engine.GetKey(GLFW_KEY_G)
		)
	{
		engine.SetNextScene<TestScene>();
	}
}