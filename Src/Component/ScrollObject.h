#pragma once
#ifndef SCROLLOBJECT_H_INCLUDE
#define SCROLLOBJECT_H_INCLUDE
#include "../Component.h"
#include "../TileMapScroll.h"

class ScrollObject :
	public Component
{
public:
	TileMapScrollPtr scrolldata;
	ScrollObject(GameObjectPtr gameObject)
		:Component(gameObject) {
		name = "ScrollObject";
	}
	virtual void Update(float deltaTime)override
	{
		switch (scrolldata->type)
		{
		case upScroll:
			if (scrolldata->baseY > scrolldata->endPosisionY)
				gameObject->position.y -= scrolldata->speed * deltaTime;
			break;
		case downScroll:
			if (scrolldata->baseY < scrolldata->endPosisionY)
				gameObject->position.y += scrolldata->speed * deltaTime;
			break;
		case rightScroll:
			if (scrolldata->baseX < scrolldata->endPosisionX)
				gameObject->position.x += scrolldata->speed * deltaTime;
			break;
		case leftScroll:
			if (scrolldata->baseX > scrolldata->endPosisionX)
				gameObject->position.x -= scrolldata->speed * deltaTime;
			break;
		}

	}
};

#endif // !SCROLLOBJECT_H_INCLUDE