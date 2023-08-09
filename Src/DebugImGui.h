#pragma once
#ifndef DEBUGIMGUI_H_INCLUDE
#define DEBUGIMGUI_H_INCLUDE
#include "ImGui/imgui.h"
#include "ImGui/imgui_impl_opengl3.h"
#include "ImGui/imgui_impl_glfw.h"
#include "GameObject.h"

class DebugImGui
{
public:
	GameObjectPtr gameObject;

	void SetUp(GameObjectPtr _gameObject)
	{
		gameObject = _gameObject;
	}
	void Execute()
	{
		ImGui::Begin(gameObject->name.c_str());

		ImGui::InputFloat("posX", &gameObject->position.x);
		ImGui::InputFloat("posY", &gameObject->position.y);

		if (gameObject->spriteList.size() != 0)
			ImGui::DragFloat("angle", &gameObject->spriteList.begin()->spriteList.begin()->angle);
		if (ImGui::CollapsingHeader("componentList")) {
			if (gameObject->componentList.size() != 0) {
				for (auto itr : gameObject->componentList)
				{
					itr->Debug();
				}
			}
		}

		ImGui::End();
	}
};

#endif //!DEBUGIMGUI_H_INCLUDE