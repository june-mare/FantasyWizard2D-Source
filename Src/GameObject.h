#pragma once
#define _CRT_SECURE_NO_WARNINGS

#ifndef GAMEOBJECT_H_INCLUDE
#define GAMEOBJECT_H_INCLUDE
#include "ObjectSprite.h"
#include "Component.h"

#include "MyMath.h"

#include "SpritePriority.h"
#include "GameObjectTag.h"

#include <string>
#include <vector>
#include <memory>
#include <algorithm>

/*先行宣言*/
class Engine;
class GameObject;
using GameObjectPtr = std::shared_ptr<GameObject>;//ゲームオブジェクトのポインタ
using GameObjectList = std::vector<GameObjectPtr>;//ゲームオブジェクトのぽんた配列


struct Rotate
{
	MyMath::vec4 rotation;
	MyMath::vec3 forward = {0.0f,1.0f,0.0f};
};

/*ゲーム内に登場する様々なオブジェクトを表す基本クラス*/
class GameObject
	:public std::enable_shared_from_this<GameObject>
{
	bool isStarted = false;
public:
	Engine* engine = nullptr;
	
	std::string name;
	int layer = 0;	/*表示優先順位(直訳　優先度)*/
	int tag = 0;	/*取得するためのタグ*/

	MyMath::vec4 position;
	Rotate rotation;
	MyMath::vec3 scale;
	
	ObjectSpriteList spriteList;
	ComponentList componentList;
	bool isDead = false;	

public:
	GameObject()noexcept = default;
	virtual ~GameObject()noexcept = default;

	/*イベントの操作*/
	virtual void Start()noexcept;
	virtual void Update(float deltaTime)noexcept;
	virtual void OnCollision(GameObject& other)noexcept;
	virtual void OnDestroy()noexcept;
	virtual void TakeDamage(GameObject& other, const Damage& damage)noexcept;
	bool IsStarted()const { return isStarted; }

	/*スプライトの操作*/
	void AddSpriteTexture(const Texcoord& tc, TexturePtr tex,
		float x = 0, float y = 0, float scale = 1, float rotation = 0,float alpha = 1)noexcept;
	void AddSprite(const Texcoord& tc, TexturePtr tex,
		float x = 0, float y = 0, float scale = 1, float rotation = 0)noexcept;
	void AddSprite(const Texcoord& tc, std::string name, float x, float y, float scale, float rotation) noexcept;
	const std::vector<ObjectSprite>& GetSprite()const {return spriteList;}

	/*コンポーネントの操作*/
	/*コンポーネントの派生クラスを入れるためにテンプレートを使用する*/
	template<typename T>
	std::shared_ptr<T> AddComponent()noexcept
	{
		std::shared_ptr<T> p = std::make_shared<T>(shared_from_this());
		componentList.push_back(p);
		return p;
	}
	
	template <typename T>
	std::shared_ptr<T> GetConponent()noexcept
	{
		auto p = std::make_shared<T>(shared_from_this());

		for (auto itr : componentList) {
			if (itr->name == p->name)
			{	
				p.reset();
				return std::dynamic_pointer_cast<T>(itr);
			}
		}

		return nullptr;
	}

	template <typename T>
	void RemoveConponent()noexcept
	{
		auto p = std::make_shared<T>(shared_from_this());
		for (auto itr : componentList) {
			if (itr->name == p->name)
			{
				componentList.erase(
					std::remove(componentList.begin(), componentList.end(), itr),
					componentList.end());
				p.reset();
				itr.reset();
				
				break;
			}
		}
	}
};
#endif // !GAMEOBJECT_H_INCLUDE