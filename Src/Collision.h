#pragma once
#ifndef COLLISION_H_INCLUDE
#define COLLISION_H_INCLUDE
#include "GameObject.h"
#include "Component/Box2DCollision.h"
//#include "Rect.h"

class Collider
{
public:
	GameObjectPtr gameObject;
	std::shared_ptr<Box2DCollision> collition;
};

using ColliderList = std::vector<Collider>;

#endif // !COLLISION_H_INCLUDE