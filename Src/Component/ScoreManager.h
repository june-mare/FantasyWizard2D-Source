#pragma once
#ifndef SCOREMANAGER_H_INCLUDE
#define SCOREMANAGER_H_INCLUDE
#include "../Component.h"
#include "text.h"
#include <stdio.h>
#include <iostream>
/*特典を管理するコンポーネント*/
class ScoreManager :public Component 
{
public:
	int score = 0;
	float scale = 2;
	std::shared_ptr<Text> textComponent;
public:
	ScoreManager(GameObjectPtr gameObject)
		:Component(gameObject) { name = "ScoreManager"; };
	virtual ~ScoreManager() = default;
	virtual void Update(float deltaTime) override{
		int second = score % 60;
		int minute =(score / 60) % 60;
		int hour =  (score / 3600);
		std::string r;
		std::to_string(hour);
		if (hour < 10)
		{
			r += std::to_string(0);
		}
		r += std::to_string(hour); r += ":";
		if (minute < 10)
		{
			r += std::to_string(0);
		}
		r += std::to_string(minute); r += ":";
		if (second < 10)
		{
			r += std::to_string(0);
		}
		r += std::to_string(second);
		if (textComponent) {			
			
			textComponent->SetText(r, scale);
		}
	}
};
using ScoreManagerPtr = std::shared_ptr<ScoreManager>;
#endif // !SCOREMANAGER_H_INCLUDE
