#pragma once
#ifndef CAMERA_H_INCLUDE
#define CAMERA_H_INCLUDE
#include "../Component.h"
#include <cmath>

class Camera :public Component
{
public:
	float camNear = 0.5;		//近平面
	float camFar = 1000;	//遠平面
	float fovY = 60;	//視野角
	float aspect = 9.0f / 16.0f;//アスペクト比

	/*ユニフォーム変数に設定するパラメータ*/
	float fovScaleX = 1;//視野角による拡大率
	float fovScaleY = 1;//視野角による拡大率
	float A = 0.01f;	//遠近法パラメータA
	float B = 0.99f;	//遠近法パラメータB;

public:
	Camera(std::shared_ptr<GameObject> gameObject)
		:Component(gameObject) { name = "camera"; }
	
	virtual ~Camera() = default;

	virtual void Update(float deltaTime)override
	{
		//遠近法パラメータ
		A = -2 * camFar * camNear / (camFar - camNear);
		B = (camFar + camNear) / (camFar - camNear);

		//視野角による拡大率を計算
		const float rad = fovY * acos(-1.0f) / 180;
		fovScaleY = 1 / tan(rad / 2);
		fovScaleX = aspect * fovScaleY;
	}

	virtual void Debug()override
	{
		if (ImGui::CollapsingHeader(name.c_str()))
		{
			ImGui::DragFloat("cameraNear", &camNear);
			ImGui::DragFloat("cameraFar", &camFar);
			ImGui::DragFloat("aspect", &this->aspect);
			ImGui::DragFloat("fov", &fovY);
			ImGui::DragFloat("fovScaleX", &fovScaleX);
			ImGui::DragFloat("fovScaleY", &fovScaleY);
		}
	}
};

#endif // !CAMERA_H_INCLUDE