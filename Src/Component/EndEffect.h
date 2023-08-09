#pragma once
#ifndef ENDEFFECT_H_INCLUDE
#define ENDEFFECT_H_INCLUDE

#include "ComponentInclude.h"
#include "../GameObject.h"
#include "../EffectList.h"

class EndEffect:
	public Component
{
public:
	int count = 1;
	int size = 1;

	EndEffect(GameObjectPtr gameObject)
		:Component(gameObject) {
		name = "EndEffect";
	}
	virtual ~EndEffect() = default;

	virtual void OnDestroy()override
	{
		const float x[] = { 0,size * 16,size * 20,size * -24,size * 24 };
		const float y[] = { 0,size * 20,size * 12,size * -20,size * -30 };

		/*”š”­‚ð•\Ž¦*/
		int i = static_cast<int>(std::size(x));
		if (count < i) {
			i = count;
		}
		for (--i; i >= 0; --i)
		{
			auto tc = EffectTCList<Effect1>();

			auto explosion = gameObject->engine->Create<GameObject>(
				"explosion", gameObject->position.x, gameObject->position.y);
			explosion->AddSpriteTexture(tc[0], gameObject->engine->FindWithTexture("Img/objects.tga"));
			explosion->layer = prioExplosion;
			auto anim = explosion->AddComponent<Animator2D>();
			anim->loopTime = false;
			anim->killWhenEnd = true;
			anim->clip.resize(std::size(tc));
			for (size_t frameNo = 0; frameNo < std::size(tc); ++frameNo) {
				anim->AddSpriteTexture(frameNo, tc[frameNo], size, x[i], y[i]);
			}
		}		
	}

};


#endif // !ENDEFFECT_H_INCLUDE