#pragma once
#ifndef COMPONENT_H_INCLUDE
#define COMPONENT_H_INCLUDE
#include <string>
#include <vector>
#include <memory>

#include "Component/MovePram.h"

#include "ImGui/imgui.h"
#include "ImGui/imgui_impl_opengl3.h"
#include "ImGui/imgui_impl_glfw.h"


class GameObject;
class Component;
using ComponentPtr = std::shared_ptr<Component>;/*�R���|�[�l���g�̃|�C���^*/
using ComponentList = std::vector<ComponentPtr>;/*�R���|�[�l���g�|�C���^�̔z��*/

/*�_���[�W���*/
struct Damage
{
	float amount = 0;		/*�_���[�W��*/
	int type = 0;			/*�_���[�W�̎��*/
};

/*�R���|�[�l���g�̊��N���X*/
class Component 	
{
public:
	Component(std::shared_ptr<GameObject> gameObject)
		:gameObject(gameObject) {};
	virtual ~Component() = default;

	/*���������Update���O�ŌĂяo�����*/
	virtual void Start(){}
	
	/*���t���[���Ăяo�����*/
	virtual void Update(float deltaTime){}

	/*�Փ˂��N�����Ƃ��ɌĂяo�����*/
	virtual void OnCollision( GameObject& other){}

	/*�_���[�W���󂯂��Ƃ��ɌĂяo�����*/
	virtual void OnTakeDamage(GameObject& other,
	const Damage& damage){}

	/*�Q�[���I�u�W�F�N�g���G���W������폜���ꂽ�Ƃ��ɌĂяo�����*/
	virtual void OnDestroy(){}

	virtual void CustomEvent(float deltaTime){}
	
	

	virtual void Debug() {}

	void Reload()noexcept;
	
	

	void ThisStart()noexcept;
	std::string name; /*�R���|�[�l���g��*/
	bool isStart = false;
	std::shared_ptr<GameObject> gameObject;
};

#endif