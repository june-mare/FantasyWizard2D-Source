#include "GameOverScene.h"
#include "TitleScene.h"
#include "GLFWWindow.h"
#include "MainGameScene.h"
#include "Component/text.h"
#include "GameObject.h"
#include "Engine.h"
#include "Sprite.h"
#include "AudioSettings.h"
#include "Component/AudioEvent.h"

/*タイトル画面を初期化する*/

bool GameOverScene::Initialize(Engine& engine) {
	/*ゲームオブジェクトを削除*/
	engine.ClearGameObjectList();
	engine.ClearUILayers();

	/*UIレイヤーを作成*/
	const size_t bgLayer = engine.AddUILayer("Img/GameOver.tga", GL_LINEAR, 10);
	const size_t logoLayer = engine.AddUILayer("Img/logo.tga", GL_LINEAR, 16);
	const size_t selLayer = engine.AddUILayer("Img/SelectBox.tga", GL_LINEAR, 10);
	const size_t textLayer = engine.AddUILayer("Img/font.tga", GL_LINEAR, 10);

	/*背景画像*/
	auto uiBackground = engine.CreateUI<GameObject>(
		bgLayer, "title_bg", 640, 420);
	uiBackground->AddSpriteTexture({ 0,0,1,1 }, 0, 0, 0.0f, 1.0);

	uisel1 = engine.CreateUI<GameObject>(
		selLayer, "select_box", 500, 65);
	uisel1->AddSpriteTexture(MakeTexcoord(0, 0, 300, 50, 300, 100), 0, 0, 0.0f, 1.0);

	uisel2 = engine.CreateUI<GameObject>(
		selLayer, "select_box", 820, 65);
	uisel2->AddSpriteTexture(MakeTexcoord(0, 50, 300, 50, 300, 100), 0, 0, 0.0f, 1.0);

	/*ゲームの開始を示すテキスト*/
	const char strPressEnter[] = "Press Enter";
	const float fontSizeX = 16;
	const float x =
		640 - static_cast<float>(std::size(strPressEnter) - 1) * fontSizeX;
	auto uiPressEnter = engine.CreateUI<GameObject>(
		textLayer, "press enter", x + 10, 100);
	auto textPressEnter = uiPressEnter->AddComponent<Text>();
	textPressEnter->SetText(strPressEnter, 2);

	const char strYes[] = "Continue";
	uiYes = engine.CreateUI<GameObject>(
		textLayer, "Continue", 760, 50);
	auto yes = uiYes->AddComponent<Text>();
	yes->SetText(strYes, 1);

	const char strNo[] = "Title Back";
	uiNo = engine.CreateUI<GameObject>(
		textLayer, "Title Back", 420, 50);
	auto no = uiNo->AddComponent<Text>();
	no->SetText(strNo, 1);

	uisel1->layer = 2;
	uisel2->layer = 2;
	uiYes->layer = 3;
	uiNo->layer = 3;
	uiPressEnter->layer = 0;
	uiBackground->layer = 2;
	Audio::Stop(AudioPlayer::bgm);
	Audio::Play(AudioPlayer::bgm, AudioPlayer::BGM::gameOver, 1, true);

	return true;
}

void GameOverScene::Execute(Engine& engine, float deltaTime)
{
	auto choice = MakeTexcoord(0, 0, 300, 50, 300, 100);
	auto notchoice = MakeTexcoord(0, 50, 300, 50, 300, 100);

	if (engine.GetKey(GLFW_KEY_RIGHT)) {
		if (sel == 0) {
			sel++;
			Audio::PlayOneShot(AudioPlayer::SE::select);
			uisel1->spriteList.clear();
			uisel1->AddSpriteTexture(notchoice, 0, 0, 0.0f, 1.0);
			uisel2->spriteList.clear();
			uisel2->AddSpriteTexture(choice, 0, 0, 0.0f, 1.0);
		}
	}
	if (engine.GetKey(GLFW_KEY_LEFT)) {
		if (sel == 1) {
			sel--;
			Audio::PlayOneShot(AudioPlayer::SE::select);
			uisel1->spriteList.clear();
			uisel1->AddSpriteTexture(choice, 0, 0, 0.0f, 1.0);
			uisel2->spriteList.clear();
			uisel2->AddSpriteTexture(notchoice, 0, 0, 0.0f, 1.0);
		}
	}
	if (engine.GetKey(GLFW_KEY_ENTER)) {
		switch (sel)
		{
		case 0:
			engine.SetNextScene<TitleScene>();
			break;
		case 1:
			engine.SetNextScene<MainStage1>();
			break;
		}
		Audio::PlayOneShot(AudioPlayer::SE::enter);

	}
	uisel1->layer = 2;
	uisel2->layer = 2;
	uiYes->layer = 3;
	uiNo->layer = 3;
}