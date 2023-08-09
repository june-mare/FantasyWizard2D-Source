#pragma once
#ifndef SAMPLETEXTSCENE_H_INCLUDE
#define SAMPLETEXTSCENE_H_INCLUDE
#include "Scene.h"
#include "GLTextFont.h"


class SampleTextScene :
	public Scene
{
	GameObjectPtr textObject = nullptr;
public:
	SampleTextScene() = default;
	virtual ~SampleTextScene() = default;

	bool Initialize(Engine& engine)override;
	
	void Execute(Engine& engine, float deltaTime)override
	{
	}

};


#endif // !SAMPLETEXTSCENE_H_INCLUDE
