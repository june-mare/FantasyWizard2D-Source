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
using ComponentPtr = std::shared_ptr<Component>;/*コンポーネントのポインタ*/
using ComponentList = std::vector<ComponentPtr>;/*コンポーネントポインタの配列*/

/*ダメージ情報*/
struct Damage
{
	float amount = 0;		/*ダメージ量*/
	int type = 0;			/*ダメージの種類*/
};

/*コンポーネントの基底クラス*/
class Component 	
{
public:
	Component(std::shared_ptr<GameObject> gameObject)
		:gameObject(gameObject) {};
	virtual ~Component() = default;

	/*さいしょのUpdate直前で呼び出される*/
	virtual void Start(){}
	
	/*枚フレーム呼び出される*/
	virtual void Update(float deltaTime){}

	/*衝突が起きたときに呼び出される*/
	virtual void OnCollision( GameObject& other){}

	/*ダメージを受けたときに呼び出される*/
	virtual void OnTakeDamage(GameObject& other,
	const Damage& damage){}

	/*ゲームオブジェクトがエンジンから削除されたときに呼び出される*/
	virtual void OnDestroy(){}

	virtual void CustomEvent(float deltaTime){}
	
	

	virtual void Debug() {}

	void Reload()noexcept;
	
	

	void ThisStart()noexcept;
	std::string name; /*コンポーネント名*/
	bool isStart = false;
	std::shared_ptr<GameObject> gameObject;
};

#endif