#pragma once
#ifndef OBJECTSPRITE_H_INCLUDE
#define OBJECTSPRITE_H_INCLUDE
#include "Sprite.h"
#include "Texture.h"
#include <vector>
#include <string>

struct ObjectSprite
{
	SpriteList spriteList;
	TexturePtr tex;
	ObjectSprite(float x, float y, float sx, float sy,
		Texcoord tc,
		TexturePtr tex)
	{
		spriteList.push_back({
		x, y, sx, sy, tc});

		this->tex = tex;
	}
	ObjectSprite(TexturePtr tex)noexcept :tex(tex){};
	ObjectSprite()noexcept = default;
	Sprite GetSprite(Texcoord tc)
	{
		for (auto& sprite : spriteList)
		{
			if (sprite.texcoord == tc)
			{
				return sprite;
			}
		}
		return Sprite();
	}
	std::string name;
};
using ObjectSpriteList = std::vector<ObjectSprite>;

ObjectSprite FindObjectSprite(std::string name, std::vector<ObjectSprite>& list);

#endif //!OBJECTSPRITE_H_INCLUDE