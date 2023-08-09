#pragma once
#ifndef TESTSCENE_H_INCLUDE
#define TESTSCENE_H_INCLUDE
#include "Scene.h"
#include <memory>
#include "TileMap.h"
#include "DebugImGui.h"
#include <math.h>
#include "TileMapTexcoord.h"

class AttachPlayer;

class TestScene :
    public Scene
{
public:
	TileMapScrollPtr scrolldata;
	char name[256] = {};
	TileMap gridMap;
	std::vector<DebugImGui> debug;
	MyMath::vec3 spawnPos;
	int effectType = 0;
	int enemyType = 0;
	GameObjectPtr player;//ÉvÉåÉCÉÑÅ[
	std::shared_ptr<AttachPlayer> playerpram;

public:
	TestScene() = default;
	virtual ~TestScene() = default;
	virtual bool Initialize(Engine & engine)override;
	virtual void Execute(Engine & engine, float deltaTime)override;
};


#endif // !TESTSCENE_H_INCLUDE