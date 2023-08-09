#pragma once
#ifndef ASSIMPMETH_H_INCLUDE
#define ASSIMPMETH_H_INCLUDE

#include "../MyMath.h"
#include "../GLContext.h"
#include "AssimpShader.h"

#include <string>
#include <vector>

struct AsVertex
{
	MyMath::vec3 position;
	MyMath::vec3 normal;
	MyMath::vec2 texcoord;
	MyMath::vec3 rotate;
	MyMath::vec3 bitrotate;
	std::vector<int> boneIds;
	std::vector<float> weights;
};

struct AsTexture
{
	unsigned int id;
	std::string type;
	std::string path;
};

class AssimpMeth
{
private:
	unsigned int ibo, vbo;
public:
	std::vector<AsVertex> vertices;
	std::vector<unsigned int> indices;
	std::vector<AsTexture> textures;
	unsigned int vao;

private:
	void SetupMesh();
public:
	AssimpMeth(
		std::vector<AsVertex> vertices,
		std::vector<unsigned int> indices,
		std::vector<AsTexture> textures);
	virtual ~AssimpMeth() = default;

	void Draw(AssimpShader& shader);
};

#endif // !ASSIMPMETH_H_INCLUDE