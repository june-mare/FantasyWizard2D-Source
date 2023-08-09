#include "AssimpModel.h"


AssimpModel::AssimpModel(char* path)
{
	textures_loaded.resize(1);
	LoadModel(path);
}

void AssimpModel::Draw(AssimpShader& shader)
{
	for (unsigned int i = 0; i < meshes.size(); i++)
	{
		meshes[i].Draw(shader);
	}
}

void AssimpModel::LoadModel(std::string path)
{
	Assimp::Importer importer;
	const aiScene* scene = importer.ReadFile(path,
		aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_CalcTangentSpace);
	if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)//nullでないかどうか
	{
		std::cout << "ERROR::ASSIMP" << importer.GetErrorString() << std::endl;
		return;
	}
	//開始から終点まで
	directory = path.substr(0, path.find_last_of('/'));
	//読み込んだモデルのノードを作成
	ProcessNode(scene->mRootNode, scene);
}


void AssimpModel::ProcessNode(aiNode* node, const aiScene* scene)
{
	for (unsigned int i = 0; i < node->mNumMeshes; i++)
	{
		//ノードオブジェクトには、シーン内の実際のオブジェクトにインデックスをつけるためのインデックスのみが含まれます
		//シーンにはすべてのデータが含まれておりノードは整理されたものを保持するための物
		aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
		meshes.push_back(ProcessMesh(mesh, scene));
	}
	for (unsigned int i = 0; i < node->mNumChildren; i++)
	{
		ProcessNode(node->mChildren[i], scene);
	}

}

AssimpMeth AssimpModel::ProcessMesh(aiMesh* mesh, const aiScene* scene)
{
	std::vector<AsVertex> vertices;
	std::vector<unsigned int> indices;
	std::vector<AsTexture> textures;

	for (unsigned int i = 0; i < mesh->mNumVertices; i++)
	{
		AsVertex vertex;
		MyMath::vec3 vector;

		vector.x = mesh->mVertices[i].x;
		vector.y = mesh->mVertices[i].y;
		vector.z = mesh->mVertices[i].z;
		vertex.position = vector;
		//ノーマルがあるかどうか調べる
		if (mesh->HasNormals())
		{
			vector.x = mesh->mNormals[i].x;
			vector.y = mesh->mNormals[i].y;
			vector.z = mesh->mNormals[i].z;
			vertex.normal = vector;
		}

		if (mesh->mTextureCoords[0])
		{
			MyMath::vec2 vec;
			vec.x = mesh->mTextureCoords[0][i].x;
			vec.y = mesh->mTextureCoords[0][i].y;
			vertex.texcoord = vec;
			if (mesh->mTangents != NULL) {
				vector.x = mesh->mTangents[i].x;
				vector.y = mesh->mTangents[i].y;
				vector.z = mesh->mTangents[i].x;
				vertex.rotate = vector;
			}
			if (mesh->mBitangents != NULL) {
				vector.x = mesh->mBitangents[i].x;
				vector.y = mesh->mBitangents[i].y;
				vector.z = mesh->mBitangents[i].z;
				vertex.bitrotate = vector;
			}
		}
		else
		{
			vertex.texcoord = MyMath::vec2(0.0f, 0.0f);
		}
		vertices.push_back(vertex);
	}

	//メッシュの各面（面はメッシュの三角形）を調べて、対応する頂点インデックスを取得します
	for (unsigned int i = 0; i < mesh->mNumFaces; i++)
	{
		aiFace face = mesh->mFaces[i];
		//表面にあるindexをを取得し格納
		for (unsigned int j = 0; j < face.mNumIndices; j++) {
			indices.push_back(face.mIndices[j]);
		}
	}

	//マテリアル処理
	aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];
	//シェーダでのサンプラー名の規則を想定している。各拡散テクスチャの名前を付ける必要があり
	//texture_deffuseNとしてNは１からMAX_SAMPLER_NUMBER迄の連番です
	//次のリストに要約されているように同じことが他のテクスチャにも当てはまります
	//texture_diffuseN texture_specularN texture normalN



	//1 texture_diffuse maps
	std::vector<AsTexture> diffuseMaps = LoadMaterialTextures(material, aiTextureType_DIFFUSE, "texture_diffuse");
	textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());
	//2 texture_samplar maps
	std::vector<AsTexture> specularMaps = LoadMaterialTextures(material, aiTextureType_SPECULAR, "texture_specular");
	textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());
	//3 texture_normal maps
	std::vector<AsTexture> normalMaps = LoadMaterialTextures(material, aiTextureType_HEIGHT, "texture_normal");
	textures.insert(textures.end(), normalMaps.begin(), normalMaps.end());
	//4 texture_height
	std::vector<AsTexture> heightMaps = LoadMaterialTextures(material, aiTextureType_AMBIENT, "texture_height");
	textures.insert(textures.end(), heightMaps.begin(), heightMaps.end());

	return AssimpMeth(vertices, indices, textures);
}

std::vector<AsTexture> AssimpModel::LoadMaterialTextures(aiMaterial* mat, aiTextureType type, std::string typeName)
{
	std::vector<AsTexture> textures;
	for (unsigned int i = 0; i < mat->GetTextureCount(type); i++)
	{
		aiString str;
		mat->GetTexture(type, i, &str);
		bool skip = false;
		for (unsigned int j = 0; j < textures_loaded.size(); j++)
		{
			if (std::strcmp(textures_loaded[j].path.data(), str.C_Str()) == 0)
			{
				textures.push_back(textures_loaded[j]);
				skip = true;
				break;
			}
		}
		if (!skip)
		{
			AsTexture texture;			
			texture.id = TextureFromFile(str.C_Str(), this->directory);
			texture.type = typeName;
			texture.path = str.C_Str();
			textures.push_back(texture);
			textures_loaded.push_back(texture);
		}
	}

	return textures;
}

unsigned int TextureFromFile(const char* path, const std::string& directory, bool gamma)
{
	std::string filename = std::string(path);

	for (unsigned int i = 0; i < filename.size(); i++)
	{
		if (filename[i] == '.')
		{
			filename[i + 1] = 't';
			filename[i + 2] = 'g';
			filename[i + 3] = 'a';
			break;
		}
	}

	filename = directory + '/' + filename;

	unsigned int textureID;
	glGenTextures(1, &textureID);

	int width, height, nrComponents;
	auto data = Texture(filename.c_str(), GL_NEAREST);

	if (data)
	{
		GLenum format;
		if (data.header[2] == 2)
			format = GL_RGB;
		else if (data.header[2] == 3)
			format = GL_RGBA;
		else
			format = GL_RGBA;

		/*glBindTexture(GL_TEXTURE_2D, textureID);
		glTexImage2D(GL_TEXTURE_2D, 0,
			format, data.width, data.height, 0, format, GL_UNSIGNED_BYTE, data.header);*/

		//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	}
	else
	{
		std::cout << "Texture failed to load at path; " << path << std::endl;
	}

	return textureID;
}
