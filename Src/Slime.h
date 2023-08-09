#pragma once
#include "Enemy.h"

class Slime :public Enemy
{
public:
	Slime(float x, float y = 0, float z = 0) { Pos(x, y, z); Initialize(); }
	Slime(Position p) { Pos(p); Initialize(); }
	Slime() { Initialize(); }
	virtual ~Slime(){}

	void Initialize()override
	{
		name = "Slime";
		if (StatusManager::GetInstance().GetStatus(name) == nullptr)
		{
			status->Set(StatusPramID::HP, 20);
			status->Set(StatusPramID::MP, 20);
			status->Set(StatusPramID::ATK, 20);
			status->Set(StatusPramID::DEF, 20);
			status->Set(StatusPramID::AGI, 20);
			StatusManager::GetInstance().Register(name, status);
		}
		else
		{
			status = StatusManager::GetInstance().GetStatus(name);
		}
	}
	void Execute()override
	{

	}
	void Terminate()override
	{

	}
};