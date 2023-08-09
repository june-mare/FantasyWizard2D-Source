#pragma once
#ifndef TILEMAP_H_INCLUDE
#define TILEMAP_H_INCLUDE
#include <vector>

/*�^�C���̔z�u�f�[�^*/
struct TileMap
{
	size_t sizex = 0;
	size_t sizey = 0;
	std::vector<int> data;
};

#include "glad/glad.h"
#include <string>
#include <fstream>
#include <filesystem>

TileMap LoadTileMap(const char* filename)noexcept;
#endif
