#include "GameObject.h"

void GameObject::Start()noexcept
{
	if (isStarted) {
		return;
	}
	if (componentList.size() == 0)return;
	for (auto& itr : componentList)
	{
		if (itr->isStart)
			continue;
		
		itr->Start();		
	}
	for (auto& itr : componentList)
	{
		itr->ThisStart();
	}
	isStarted = true;
}

void GameObject::Update(float deltaTime)noexcept
{
	for (auto& itr : componentList) {
		itr->Update(deltaTime);
	}
}

void GameObject::OnCollision(GameObject& other)noexcept
{
	for (auto& itr : componentList) {
		itr->OnCollision(other);
	}
}

void GameObject::OnDestroy()noexcept
{
	for (auto& itr : componentList)
	{
		itr->OnDestroy();
	}
}

void GameObject::TakeDamage(GameObject& other, const Damage& damage)noexcept
{
	for (auto& itr : componentList)
	{
		itr->OnTakeDamage(other, damage);
	}
}

void GameObject::AddSpriteTexture(const Texcoord& tc, TexturePtr tex, float x, float y, float scale, float rotation,float alpha)noexcept
{
	spriteList.push_back(ObjectSprite(x, y, 0, 0, tc, tex));
	spriteList.back().spriteList.begin()->scale = scale;
	spriteList.back().spriteList.begin()->alpha = alpha;

}

void GameObject::AddSprite(const Texcoord& tc, TexturePtr tex, float x, float y, float scale, float rotation) noexcept
{
	for (auto sprite : spriteList)
	{
		if (sprite.tex == tex)
		{
			sprite.spriteList.push_back(
				{ x, y, 0, 0, tc }
			);
		}
	}
}

void GameObject::AddSprite(const Texcoord& tc, std::string name, float x, float y, float scale, float rotation) noexcept
{
	for (auto sprite : spriteList)
	{
		if (sprite.name == name)
		{
			sprite.spriteList.push_back(
				{ x, y, 0, 0, tc }
			);
		}
	}
}
