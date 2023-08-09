#pragma once
#ifndef MOVEPRAM_H_INCLUDE
#define MOVEPRAM_H_INCLUDE
#include "../MyMath.h"


class MovePram 	
{
public:
	MovePram() = default;
	virtual ~MovePram() = default;
	
	float speed = 1.0f;
	float acc = 0.0f;
	float accIncrease = 0.2f;
	float accMax = 5.0f;
	MyMath::vec3 moveVector = {};

	void Acceleration()noexcept
	{
		if (accMax > acc)acc += accIncrease;
		if (accMax < acc)acc = accMax;
	}
	void Reset()noexcept
	{
		acc = 0.0f;
	}
	MyMath::vec3 IncreasePos(float deltaTime)noexcept
	{
		return moveVector.normal() * (acc + speed) * deltaTime;
	}
};


#endif