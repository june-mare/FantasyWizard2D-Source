#pragma once
#ifndef SPRITECOMPONENT_H_INCLUDE
#define SPRITECOMPONENT_H_INCLUDE
#include "../Component.h"
#include "../GameObject.h"

class SpriteComponent :public Component
{
public:
	SpriteComponent(GameObjectPtr gameObject)
		:Component(gameObject) {
		name = "SpriteComponent";
	}
	SpriteList spriteList;
	
};

#endif // !SPRITECOMPONENT_H_INCLUDE
