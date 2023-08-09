#pragma once
#ifndef ASSIMPMODEL_H_INCLUDE
#define ASSIMPMODEL_H_INCLUDE
#include "AssimpMesh.h"
#include "AssimpShader.h"


#include "assimp/Importer.hpp"
#include "assimp/scene.h"
#include "assimp/postprocess.h"
#include "../Texture.h"
#include "../Sprite.h"

class AssimpModel
{
public:
	std::vector<AsTexture> textures_loaded;
	std::vector<AssimpMeth> meshes;
	std::string directory;
	bool gammaCorrection;
public:
	AssimpModel(char* path);
	virtual ~AssimpModel() = default;
	void Draw(AssimpShader& shader);

private:
	void LoadModel(std::string path);
	void ProcessNode(aiNode* node, const aiScene* scene);
	AssimpMeth ProcessMesh(aiMesh* mesh, const aiScene* scene);
	std::vector<AsTexture> LoadMaterialTextures
	(aiMaterial* mat, aiTextureType type, std::string typeName);

};

unsigned int TextureFromFile
(const char* path, const std::string& directory, bool gamma = false);


#endif // !ASSIMPMODEL_H_INCLUDE