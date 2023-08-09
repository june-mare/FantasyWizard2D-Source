#pragma once
#ifndef GAMOVER_H_INCLUDE
#define GAMOVER_H_INCLUDE
#include "Scene.h"

class GameOverScene :public Scene
{
	int sel = 0;
	GameObjectPtr uisel1, uisel2,uiYes,uiNo;
public:
	GameOverScene() = default;
	virtual ~GameOverScene() = default;
	virtual bool Initialize(Engine& engine)override;
	virtual void Execute(Engine& engine, float deltaTime)override;
};

#endif // !GAMOVER_H_INCLUDE