#pragma once
#define _CRT_SECURE_NO_WARNINGS

#ifndef ENGINE_H_INCLUDE
#define ENGINE_H_INCLUDE


//#define DEBUG

#include <Windows.h>
#include <string>
#include <random>
#include <memory>
#include "ObjectSprite.h"
#include "GLFWWindow.h"
#include "GLContext.h"
#include "Scene.h"
#include "GameObject.h"
#include "TileMap.h"
#include "Time.h"
#include "Collision.h"
#include "GLTextFont.h"

class ProgramPipeline;
using ProgramPipelinePtr = std::shared_ptr<ProgramPipeline>;
class Texture;
using TexturePtr = std::shared_ptr<Texture>;

namespace Mesh
{
	class PrimitiveBuffer;
	using PrimitiveBufferPtr = std::shared_ptr<PrimitiveBuffer>;
	class MeshBuffer;
	using MeshBufferPtr = std::shared_ptr<MeshBuffer>;
}

/*�Q�[���G���W��*/
class Engine
{
public:
	int Run();

	std::mt19937& GetRandomGenerator() { return rg; }
	
	/*�w�i�X�v���C�g�̈ʒu���*/
	
	template<typename T>
	void SetNextScene() { nextScene = std::make_shared<T>(); }
	bool GetKey(int key)const {
		GLFW_SPACE::Window& windows = GLFW_SPACE::Window::Instance();
		return glfwGetKey(windows.GetGLWindow(), key);
	}
	TileMap LoadTileMap(const char* filename, TileMapScroll scrolldata);
	void SetSpriteList(const TileMap& tileMap,TexturePtr tex, TileMapScroll scrolldata);
	void SetBackGroundTexture(TexturePtr tex, TileMapScroll scrolldata);
	/*�Q�[���I�u�W�F�N�g���쐬����*/
	template<typename T>
	std::shared_ptr<T> Create(const std::string& name, float x = 0, float y = 0)
	{
		std::shared_ptr<T> p = std::make_shared<T>();
		p->engine = this;
		p->name = name;
		p->position.x = x;
		p->position.y = y;
		gameObjectList.push_back(p);
		return p;
	}
	void ClearGameObjectList() { gameObjectList.clear();  collitionList.clear(); }
	void CameraDebug();

	void RegisterCollitionObject(GameObjectPtr gameObject, std::shared_ptr<Box2DCollision> collision)noexcept;
	
	bool CheckCollideObject(GameObjectPtr gameObject, MyMath::vec3 vector);

	size_t AddUILayer(const char* image, GLenum filferMode, size_t reserve);

	void ClearUILayers();
	/*UI�p�Q�[���I�u�W�F�N�g���쐬����
	@param layerNo �ǉ���̃��C���[�ԍ�
	@param name	   �Q�[���I�u�W�F�N�g�̖��O
	@param x	�@ �Q�[���I�u�W�F�N�g��z�u����x���W
	@param y	   �Q�[���I�u�W�F�N�g��z�u����y���W

	@return �쐬����UI�p�Q�[���I�u�W�F�g
	*/
	template<typename T>
	std::shared_ptr<T> CreateUI(size_t layerNo, const std::string& name,
		float x = 0, float y = 0) {
		if (layerNo >= uiLayerList.size()) {
			return nullptr;
		}
		std::shared_ptr<T> p = std::make_shared<T>();
		p->engine = this;
		p->name = name;
		p->position.x = x;
		p->position.y = y;
		uiLayerList[layerNo].gameObjectList.push_back(p);
		return p;
	}
	void RegisterUIObject(size_t layerNo, GameObjectPtr uiObject)
	{
		uiLayerList[layerNo].gameObjectList.push_back(uiObject);
	}

	TexturePtr FindWithTexture(std::string filename)noexcept;
	
	GameObjectPtr FindWithTagGameObject(GameObjectTag tag)noexcept;
	GameObjectList FindWithTagGameObjects(GameObjectTag tag)noexcept;
	GameObjectPtr FindWithGameObjectName(std::string name)noexcept;
	GameObjectList FindWithGameObjectsName(std::string name)noexcept;

	void BgScroll(TileMapScroll& scrolldata)noexcept;
	void CalcStartUpBgLocation(TileMapScroll& scrolldata)noexcept;
private:

	int Initialize();
	int MainLoop();
	int Finalize();

	void UpdateGameObject(float deltaTime);
	void CollideGameObject();
	void RemoveGameObject(GameObjectList& gameObjectList);
	void RemoveCollider();
	void MakeSpriteListObj(GameObjectList& gameObjectList, SpriteList& spriteList);
	void MakeSpriteListObj(GameObjectList& gameObjectList, std::vector<ObjectSprite>& spriteList);
	void DrawSpriteList(const SpriteList& spriteList,
		GLuint tex, size_t& spriteSsboOffset);

	std::string title = "FantasyWizard";
	//�V�F�[�_�[
	ProgramPipelinePtr progSprite;

	//���_�o�b�t�@
	GLuint vbo = 0; /*���_�f�[�^���o�[�e�b�N�X�o�b�t�@�ɃR�s�[*/
	GLuint ibo = 0; /*�C���f�b�N�X�o�b�t�@�̊Ǘ��ԍ�*/
	GLuint vao = 0; /*�\�����̊Ǘ��ԍ�*/
	//�e�L�X�`��
	std::vector<TexturePtr> tex;
	//�X�v���C�g
	const size_t maxSpriteCount = 100'000;/*�X�v���C�g�̍ő吔*/
	
	ObjectSpriteList spriteListBg;
	ObjectSpriteList spriteListObj;
	
	//SSBO
	size_t spriteSsboSize = 0;		//�X�v���C�g�pSSBO�̃o�C�g�T�C�Y
	GLuint ssboSprite = 0;			//�X�v���C�g�p�̊Ǘ��ԍ�
	size_t spriteSsboIndex = 0;	//�R�s�[��o�b�t�@�̃C���f�b�N�X
	uint8_t* pSpriteSsbo = nullptr;//�X�v���C�g�f�[�^�̎q�C�[��A�h���X

	Time time;
	/*�X�v���C�g�pSSBO�̓����I�u�W�F�N�g*/
	GLsync syncSpriteSsbo[2] = { 0,0 };/*GLsync == �����I�u�W�F�N�g�̊Ǘ��ԍ�*/

	/*3D���f���p�̕ϐ�*/
	ProgramPipelinePtr progStandard3D;
	Mesh::PrimitiveBufferPtr primitiveBuffer;
	Mesh::MeshBufferPtr meshBuffer;

	ProgramPipelinePtr progAssimp3D;



	GameObjectPtr cameraObject;
	//����
	std::mt19937 rg;		/*�^�������𐶐�����I�u�W�F�N�g*/

	ScenePtr scene;//���s���̃V�[��
	ScenePtr nextScene;//���̃V�[��

	GameObjectList gameObjectList;/*�Q�[���I�u�W�F�N�g�z��*/
	ColliderList collitionList;
private:
	/*UI�p�Q�[���I�u�W�F�N�g�̃��C���[�\��*/
	struct UILayer {
		TexturePtr tex;/*���C���[�̉�ʂɎg�p����e�N�X�`��*/
		GameObjectList gameObjectList;
		std::vector<Sprite> spriteList;
	};
	std::vector<UILayer> uiLayerList;/*UI���C���[�z��*/	
};

#endif // !ENGINE_H_INCLUDE


