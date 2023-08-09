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

/*��s�錾*/
class Engine;
class GameObject;
using GameObjectPtr = std::shared_ptr<GameObject>;//�Q�[���I�u�W�F�N�g�̃|�C���^
using GameObjectList = std::vector<GameObjectPtr>;//�Q�[���I�u�W�F�N�g�̂ۂ񂽔z��


struct Rotate
{
	MyMath::vec4 rotation;
	MyMath::vec3 forward = {0.0f,1.0f,0.0f};
};

/*�Q�[�����ɓo�ꂷ��l�X�ȃI�u�W�F�N�g��\����{�N���X*/
class GameObject
	:public std::enable_shared_from_this<GameObject>
{
	bool isStarted = false;
public:
	Engine* engine = nullptr;
	
	std::string name;
	int layer = 0;	/*�\���D�揇��(����@�D��x)*/
	int tag = 0;	/*�擾���邽�߂̃^�O*/

	MyMath::vec4 position;
	Rotate rotation;
	MyMath::vec3 scale;
	
	ObjectSpriteList spriteList;
	ComponentList componentList;
	bool isDead = false;	

public:
	GameObject()noexcept = default;
	virtual ~GameObject()noexcept = default;

	/*�C�x���g�̑���*/
	virtual void Start()noexcept;
	virtual void Update(float deltaTime)noexcept;
	virtual void OnCollision(GameObject& other)noexcept;
	virtual void OnDestroy()noexcept;
	virtual void TakeDamage(GameObject& other, const Damage& damage)noexcept;
	bool IsStarted()const { return isStarted; }

	/*�X�v���C�g�̑���*/
	void AddSpriteTexture(const Texcoord& tc, TexturePtr tex,
		float x = 0, float y = 0, float scale = 1, float rotation = 0,float alpha = 1)noexcept;
	void AddSprite(const Texcoord& tc, TexturePtr tex,
		float x = 0, float y = 0, float scale = 1, float rotation = 0)noexcept;
	void AddSprite(const Texcoord& tc, std::string name, float x, float y, float scale, float rotation) noexcept;
	const std::vector<ObjectSprite>& GetSprite()const {return spriteList;}

	/*�R���|�[�l���g�̑���*/
	/*�R���|�[�l���g�̔h���N���X�����邽�߂Ƀe���v���[�g���g�p����*/
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