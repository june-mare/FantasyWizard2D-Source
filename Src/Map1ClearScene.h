#pragma once
#ifndef MAP1CLEARSCENE_H_INCLUDE
#define MAP1CLEARSCENE_H_INCLUDE
#include "Scene.h"
#include "TileMap.h"
#include <memory>

class TileMapScroll;
using TileMapScrollPtr = std::shared_ptr<TileMapScroll>;

class Map1ClearScene :public Scene
{
public:
	TileMap objectMap;
	TileMapScrollPtr scrolldata;

	Map1ClearScene() = default;
	virtual ~Map1ClearScene() = default;
	virtual bool Initialize(Engine& engine)override;
	virtual void Execute(Engine& engine, float deltaTime)override;
	void LoadMapObject(Engine& engine);
};

#endif // !MAP1CLEARSCENE_H_INCLUDE
