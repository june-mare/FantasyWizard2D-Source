#include "Box2DCollision.h"
#include "../Engine.h"

void Box2DCollision::Debug()
{
	if (ImGui::CollapsingHeader(name.c_str()))
	{
		ImGui::DragFloat("x", &aabb.min[0]);
		ImGui::DragFloat("sx", &aabb.max[0]);
		ImGui::DragFloat("y", &aabb.min[1]);
		ImGui::DragFloat("sy", &aabb.max[1]);
	}
}
void Box2DCollision::Start()
{
	gameObject->engine->RegisterCollitionObject(gameObject, this->shared_from_this());
}

void Box2DCollision::AddCollision(const Square& r)
{
	collisionList.push_back(r);

	/*境界ボックスを更新*/
	aabb.min[0] = (std::min)(aabb.min[0], r.min[0]);
	aabb.min[1] = (std::min)(aabb.min[1], r.min[1]);
	aabb.max[0] = (std::max)(aabb.max[0], r.max[0]);
	aabb.max[1] = (std::max)(aabb.max[1], r.max[1]);
}
