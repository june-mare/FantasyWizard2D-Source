#pragma once
#ifndef SPIRALBULLET_H_INCLUDE
#define SPIRALBULLET_H_INCLUDE
#include "../Component.h"

class SpiralBullet
	:public Component
{
public:
	float variability;
	float acc;
	float speed;
	float angle;

};

#endif //!SPIRALBULLET_H_INCLUDE