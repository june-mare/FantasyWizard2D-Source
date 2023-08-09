#pragma once
#ifndef ASSIMPCAMERA_H_INCLUDE
#define ASSIMPCAMERA_H_INCLUDE
#include "glad/glad.h"
#include "../MyMath.h"
#include <iostream>
enum Camera_Movement
{
	FORWARD,
	BACKWARD,
	LEFT,
	RIGHT,
};

const float YAW = -90.0f;
const float PITCH = 0.0f;
const float SPEED = 2.5f;
const float SENSITIVITY = 0.1f;
const float ZOOM = 45.0f;


class AssimpCamera
{
public:
	
	MyMath::vec3 Position;
	MyMath::vec3 Front;
	MyMath::vec3 Up;
	MyMath::vec3 Right;
	MyMath::vec3 WorldUp;

	float Yaw;
	float Pitch;
	float MovementSpeed;
	float MouseSensitivity;
	float Zoom;

	AssimpCamera(
		MyMath::vec3 position = MyMath::vec3(0.0f, 0.0f, 0.0f),
		MyMath::vec3 up = MyMath::vec3(0.0f, 0.0f, 0.0f),
		float yaw = YAW, float pitch = PITCH) :
		Front(MyMath::vec3(0.0f, 0.0f, -1.0f)),
		MovementSpeed(SPEED), MouseSensitivity(SENSITIVITY), Zoom(ZOOM)
	{
		Position = position;
		WorldUp = up;
		Yaw = yaw;
		Pitch = pitch;
		UpdateCameraVector();
	}
	AssimpCamera(
		float posx,float posy,float posz,
		float upx,float upy,float upz,
		float yaw, float pitch) :
		Front(MyMath::vec3(0.0f, 0.0f, -1.0f)),
		MovementSpeed(SPEED), MouseSensitivity(SENSITIVITY), Zoom(ZOOM)
	{
		Position = MyMath::vec3(posx,posy,posz);
		WorldUp = MyMath::vec3(upx,upy,upz);
		Yaw = yaw;
		Pitch = pitch;
		UpdateCameraVector();
	}

	MyMath::mat4 GetViewMatrix()
	{
		return MyMath::LookAt(Position, Position + Front, Up);
	}

	void ProcessKeyboard(Camera_Movement direction, float deltaTime)
	{
		float velocity = MovementSpeed * deltaTime;
		if (direction == Camera_Movement::FORWARD)
			Position += Front * velocity;
		if (direction == Camera_Movement::BACKWARD)
			Position -= Front * velocity;
		if (direction == Camera_Movement::RIGHT)
			Position += Right * velocity;
		if (direction == Camera_Movement::LEFT)
			Position -= Right * velocity;
	}

	void ProcessMouseMovement(float xoffset, float yoffset, GLboolean constrainPitch = true)
	{
		xoffset *= MouseSensitivity;
		yoffset *= MouseSensitivity;

		Yaw		+= xoffset;
		Pitch	+= yoffset;

		if (constrainPitch)
		{
			if (Pitch > 89.0f)
				Pitch = 89.0f;
			if (Pitch < -89.0f)
				Pitch = -89.0f;
		}
		UpdateCameraVector();
	}

	void ProcessMouseScroll(float yoffset)
	{
		Zoom -= static_cast<float>(yoffset);
		if (Zoom < 1.0f)
			Zoom = 1.0f;
		if (Zoom > 45.0f)
			Zoom = 45.0f;
	}

private:
	void UpdateCameraVector()
	{
		
		MyMath::vec3 front;
		front.x = cos(MyMath::Degree2Radian(Yaw)) * cos(MyMath::Degree2Radian(Pitch));
		front.y = sin(MyMath::Degree2Radian(Pitch));
		front.z = sin(MyMath::Degree2Radian(Yaw)) * cos(MyMath::Degree2Radian(Pitch));
		Front	= front.normal();

		Right	= Front.Cross(WorldUp).normal();
		Up		= Right.Cross(Front).normal();
	}
};

#endif // !ASSIMPCAMERA_H_INCLUDE