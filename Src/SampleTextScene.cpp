#include "SampleTextScene.h"
#include "GLTextFont.h"
#include "Engine.h"
#include "GameObject.h"

bool SampleTextScene::Initialize(Engine& engine)
{
	engine.ClearGameObjectList();
	engine.ClearUILayers();

	GLText text;
	text.Initialize("Res/Font/x12y16pxMaruMonica.ttf", 24);
	textObject = engine.Create<GameObject>("text", 640, 600);
	textObject->layer = 0;

	text.EventCall("HelloWorld");
	auto textext = std::make_shared<Texture>("", text.GetTex());
	textObject->AddSpriteTexture({ 0,0,1,1 }, textext);
	


	return true;
}