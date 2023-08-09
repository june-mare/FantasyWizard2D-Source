#pragma once
#ifndef DAMAGESOURCE_H_INCLUDE
#define DAMAGESOURCE_H_INCLUDE
#include "../Component.h"
#include "../GameObject.h"
#include <string>
#include<cassert>
#include<vector>

/*ダメージ源コンポーネント*/
class DamageSource :public Component
{
public:
	DamageSource(GameObjectPtr gameObject)
		:Component(gameObject) {
		name = "DamageSource";
	};
	virtual ~DamageSource() = default;
	virtual void OnCollision(GameObject& other) override
	{
		if (!isLaser) {
			if (count <= 0)
			{
				//*ターゲットメイト同じ名前のゲームオブジェクトにダメージを与える
				for (auto itr : targetName) {

					if (other.name == itr)
					{
						other.TakeDamage(*gameObject, damageInfo);
						count = interval;
						if (isOnce) { gameObject->isDead = true; }
					}
				}
			}
		}
		else
		{
			for (auto itr : targetName) {

				if (other.name == "object")
				{
					other.TakeDamage(*gameObject, damageInfo);									
				}
				else if (other.name == itr && count <= 0)
				{
					other.TakeDamage(*gameObject, damageInfo);
					count = interval;
				}
			}
		}

	}
	virtual void Update(float deltaTime)override
	{
		count -= deltaTime;
		if (count < 0)count = 0;
	}
	virtual void Debug()override
	{
		ImGui::CollapsingHeader(name.c_str());
		for (auto itr : targetName) {
			ImGui::Text(itr.c_str());
		}
		ImGui::DragFloat("Damage", &damageInfo.amount);
	}

	std::vector<std::string> targetName = {};
	Damage damageInfo = { 1,0 };
	bool isOnce = false;
	bool isLaser = false;
	float interval = 0.0f;
	float count;
};

#endif // !DAMAGESOURCE_H_INCLUDE
