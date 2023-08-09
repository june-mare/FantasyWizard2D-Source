#pragma once
#ifndef EXPLOSION_H_INCLUDE
#define EXPLOSION_H_INCLUDE
#include "../Component.h"
#include "../GameObject.h"
#include "../SpritePriority.h"
#include "../Engine.h"
#include "Animator2D.h"

class Explosion :public Component
{
public:
	int count = 3;
public:
	Explosion(GameObjectPtr gameObject)
		:Component(gameObject) { name = "Explosion"; };
	virtual void Debug()override
	{
		if (ImGui::CollapsingHeader(name.c_str()))
		{
			ImGui::SliderInt("count", &count, 1, 5);
		}

	}
	virtual ~Explosion() = default;
	virtual void OnDestroy()override
	{
		/*�����A�j���[�V�����f�[�^*/
		const Texcoord tc[] =
		{
			MakeTexcoord(320,368,32,32),
			MakeTexcoord(352,368,32,32),
			MakeTexcoord(384,368,32,32),
			MakeTexcoord(416,368,32,32),
			MakeTexcoord(448,368,32,32),
			MakeTexcoord(480,368,32,32),
		};
		/*���W�f�[�^*/
		const float x[] = { 0,12,20,-16,12 };
		const float y[] = { 0,20,12,-16,-20 };

		/*������\��*/
		int i = static_cast<int>(std::size(x));
		if (count < i) {
			i = count;
		}
		for (--i; i >= 0; --i)
		{		
			auto explosion = gameObject->engine->Create<GameObject>(
				"explosion",gameObject->position.x,gameObject->position.y);
			explosion->AddSpriteTexture(tc[0],gameObject->engine->FindWithTexture("Img/objects.tga"));
			explosion->layer = prioExplosion;
			auto anim = explosion->AddComponent<Animator2D>();
			anim->loopTime = false;
			anim->killWhenEnd = true;
			anim->clip.resize(std::size(tc));
			for (size_t frameNo = 0; frameNo < std::size(tc); ++frameNo) {
				anim->AddSpriteTexture(frameNo, tc[frameNo],1, x[i], y[i]);
			}
		}
	}
};

#endif // !EXPLOSION_H_INCLUDE