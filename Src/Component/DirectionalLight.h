#pragma once
#ifndef DIRECTIONALLIGHT_H_INCLUDE
#define DIRECTIONALLIGHT_H_INCLUDE
#include "../Component.h"
#include "../MyMath.h"


struct DirectionalLight
{
	MyMath::vec3 color;
	MyMath::vec3 direction;
};



#endif //!DIRECTIONALLIGHT_H_INCLUDE