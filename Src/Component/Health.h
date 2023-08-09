#pragma once
#ifndef HEALTH_H_INCLUDE
#define HEALTH_H_INCLUDE
#include "../Component.h"
#include "../GameObject.h"

/*耐久値コンポーネント*/
class Health :public Component
{
public:
	Health(GameObjectPtr gameObject)
		:Component(gameObject) { name = "Health"; }
	virtual ~Health() = default;
	virtual void Debug()override
	{
		if (ImGui::CollapsingHeader(name.c_str()))
		{
			ImGui::DragFloat("Health", &health);
		}
	}
	virtual void OnTakeDamage(GameObject& other,
		const Damage& damage)override
	{
		health -= damage.amount;
		if (health <= 0)
		{
			gameObject->isDead = true;
		}
	}
	float health = 1;	//耐久値
};

#endif // !HEALTH_H_INCLUDE

