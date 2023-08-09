#pragma once
#ifndef SELECTSTAGESCENE_H_INCLUDE
#define SELECTSTAGESCENE_H_INCLUDE

#include "Scene.h"
#include <memory>
#include "TileMap.h"
#include "DebugImGui.h"
#include <math.h>
#include "TileMapTexcoord.h"

class SelectStageScene :
    public Scene
{
public:
	int sel = 0;
	TileMapScrollPtr scrolldata;
	std::vector<GameObjectPtr> stageObjectUI;
	GameObjectPtr player;
	SelectStageScene() = default;
	virtual ~SelectStageScene() = default;
	virtual bool Initialize(Engine & engine)override;
	virtual void Execute(Engine & engine, float deltaTime)override;

};

#endif // !SELECTSTAGESCENE_H_INCLUDE