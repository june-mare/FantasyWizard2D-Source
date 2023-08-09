#pragma once
#ifndef TILEMAPSCROLL_H_INCLUDE
#define TILEMAPSCROLL_H_INCLUDE
#include <memory>

enum ScrollType
{
	nonScroll,
	upScroll,
	downScroll,
	rightScroll,
	leftScroll
};

class TileMapScroll
	:public std::enable_shared_from_this<TileMapScroll>
{
	inline bool isEndPosisionX()noexcept
	{
		auto x = std::abs(std::abs(endPosisionX) - std::abs(baseX));

		return x < 16;
	}
	inline bool isEndPosisionY()noexcept
	{
		auto y = std::abs(std::abs(endPosisionY) - std::abs(baseY));

		return y < 16;
	}
public:
	ScrollType type;
	float speed;
	float baseX;
	float baseY;
	float sizeX;
	float sizeY;
	//float endPosision;
	float endPosisionX = -16;
	float endPosisionY = -16;
	float maxStage;
	float stage;
	float tileSize = 32;
public:
	inline bool CheckFinalStage()
	{
		return maxStage == stage;
	}
	void CalcNextStopPosition()
	{
		switch (type)
		{
		case upScroll:
			endPosisionY = -(sizeY * tileSize) * (stage / maxStage)
				+ 720 + 16;
			break;
		case downScroll:
			endPosisionY = (sizeY * tileSize) * (stage / maxStage)
				- 720 - 16 + (720 - (sizeY * tileSize));
			break;
		case leftScroll:
			endPosisionX = (sizeX * tileSize) * (stage / maxStage)
				- 1280 - 16 + (1280 - (sizeX * tileSize));
			break;
		case rightScroll:
			endPosisionX = -(sizeX * tileSize) * (stage / maxStage)
				+ 1280 + 16;
			break;
		case nonScroll:
			break;
		}
		
	}
	void CalcEndPosition()
	{
		switch (type)
		{
		case upScroll:
			endPosisionY = -static_cast<float>(sizeY * tileSize) +
				720 + 16;
			break;
		case downScroll:
			endPosisionY = -16;
			break;
		case rightScroll:
			endPosisionX = -16;//�X�N���[���I�[���Ws
			break;
		case leftScroll:
			endPosisionX = -static_cast<float>(sizeX * tileSize) + 1280 + 16;
			break;
		case nonScroll:
			break;
		}
	}
	inline bool isEndPosision()noexcept
	{
		if (type == upScroll || type == downScroll)
		{
			return isEndPosisionY();
		}
		else
		{
			return isEndPosisionX();
		}
	}
};
using TileMapScrollPtr = std::shared_ptr<TileMapScroll>;

#endif // !TILEMAPSCROLL_H_INCLUDE