#pragma once
#ifndef TITLESCENE_H_INCLUDE
#define TITLESCENE_H_INCLUDE
#include "Scene.h"
#include <memory>

class FadeController;

class TitleScene :public Scene
{
	bool fadein = false;
	float interval = 2;
	float allInterval = 10;
public:
	std::shared_ptr<FadeController> fades[13];
	TitleScene() = default;
	virtual ~TitleScene() = default;
	virtual bool Initialize(Engine& engine)override;
	virtual void Execute(Engine& engine, float deltaTime)override;
};

#endif // !TITLESCENE_H_INCLUDE
