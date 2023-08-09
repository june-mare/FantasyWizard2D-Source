#pragma once
#ifndef BOX2DCOLLISION_H_INCLUDE
#define BOX2DCOLLISION_H_INCLUDE
#include "../Component.h"
#include "../GameObject.h"
#include "../Rect.h"
#include <algorithm>

class Box2DCollision :public Component,
	public std::enable_shared_from_this<Box2DCollision>
{
	SquareList collisionList;	//コリジョン配列
	Square aabb = { 0,0,0,0 };//コリジョン境界線ボックス
public:
	Box2DCollision(std::shared_ptr<GameObject> gameObject)
		:Component(gameObject) { name = "Box2DCollision"; }
	virtual ~Box2DCollision() = default;

	virtual void Debug()override;
	
	void Start()override;
	
	void AddCollision(const Square& r);

	SquareList& GetCollision(){ return collisionList; }
	Square& GetAabb(){ return aabb; }

};

#endif // !BOX2DCOLLISION_H_INCLUDE