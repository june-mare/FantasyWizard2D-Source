#pragma once
#ifndef CAMERA_H_INCLUDE
#define CAMERA_H_INCLUDE
#include "../Component.h"
#include <cmath>

class Camera :public Component
{
public:
	float camNear = 0.5;		//�ߕ���
	float camFar = 1000;	//������
	float fovY = 60;	//����p
	float aspect = 9.0f / 16.0f;//�A�X�y�N�g��

	/*���j�t�H�[���ϐ��ɐݒ肷��p�����[�^*/
	float fovScaleX = 1;//����p�ɂ��g�嗦
	float fovScaleY = 1;//����p�ɂ��g�嗦
	float A = 0.01f;	//���ߖ@�p�����[�^A
	float B = 0.99f;	//���ߖ@�p�����[�^B;

public:
	Camera(std::shared_ptr<GameObject> gameObject)
		:Component(gameObject) { name = "camera"; }
	
	virtual ~Camera() = default;

	virtual void Update(float deltaTime)override
	{
		//���ߖ@�p�����[�^
		A = -2 * camFar * camNear / (camFar - camNear);
		B = (camFar + camNear) / (camFar - camNear);

		//����p�ɂ��g�嗦���v�Z
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